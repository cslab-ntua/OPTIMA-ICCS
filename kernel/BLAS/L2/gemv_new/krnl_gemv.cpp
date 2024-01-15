#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
#define CHANNELS 8

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void read_a(hls::stream<v_dt>& Astream,v_dt *A,int M,int N){


	for(int i=0;i<M;i++){
		for(int j=0;j<N/VDATA_SIZE;j++){
			#pragma HLS pipeline II=1
			Astream << A[i*N/VDATA_SIZE + j];
		}
	}
}

void read_x(hls::stream<v_dt>& Xstream, v_dt *X,int M,int N){

	for(int i=0; i<M;i++){
		for(int j=0;j<N/VDATA_SIZE;j++){
			#pragma HLS pipeline II=1
			Xstream << X[j];
		}
	}
}

void read_y(hls::stream<float>& Ystream,float *Y,int M){

	read_y:	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Ystream <<Y[i];
	}
}

void scal_accum(hls::stream<v_dt>& Astream, hls::stream<v_dt>& Xstream, hls::stream<v_dt> Yaccum[CHANNELS], float alpha, int M, int N){

	int range = N/VDATA_SIZE;
    non_pipelined: for(int i=0;i<M;i++){
    	v_dt Y_temp[CHANNELS];

    	for(int k=0;k<VDATA_SIZE;k++){
    		#pragma HLS unroll
    	    Y_temp[0].data[k]=0.0f;
    	    Y_temp[1].data[k]=0.0f;
    	    Y_temp[2].data[k]=0.0f;
    	    Y_temp[3].data[k]=0.0f;
    	    Y_temp[4].data[k]=0.0f;
    	    Y_temp[5].data[k]=0.0f;
    	    Y_temp[6].data[k]=0.0f;
    	    Y_temp[7].data[k]=0.0f;
    	}

    	pipelined: for(int j=0; j<range; j++){
			#pragma hls pipeline II=1
			int channel = j%CHANNELS;
			v_dt A_temp = Astream.read();
			v_dt X_temp = Xstream.read();

			for(int k=0;k<VDATA_SIZE;k++){
				#pragma HLS unroll
				Y_temp[channel].data[k] += A_temp.data[k] * X_temp.data[k] * alpha;
			}
		}

    	for(int k=0;k<CHANNELS;k++){
    		#pragma HLS unroll
    		Yaccum[k] << Y_temp[k];
    	}
    }
}


void reduction_1(hls::stream<v_dt> Yaccum[CHANNELS],hls::stream<v_dt>& Yreduc,int M){

	reduction:
	for(int i=0;i<M;i++){
		#pragma hls pipeline II=1
		v_dt Ytemp[CHANNELS];
		v_dt temp1;
		v_dt temp2;
		v_dt temp3;
		v_dt temp4;
		v_dt temp_final;

		for(int k=0; k<CHANNELS; k++){
			#pragma hls unroll
			Ytemp[k] = Yaccum[k].read();
		}

		for(int k=0; k<VDATA_SIZE; k++){
			#pragma hls unroll
			temp1.data[k] = Ytemp[0].data[k] + Ytemp[1].data[k];
			temp2.data[k] = Ytemp[2].data[k] + Ytemp[3].data[k];
			temp3.data[k] = Ytemp[4].data[k] + Ytemp[5].data[k];
			temp4.data[k] = Ytemp[6].data[k] + Ytemp[7].data[k];

			temp_final.data[k] = temp1.data[k] + temp2.data[k] + temp3.data[k] + temp4.data[k];
		}

		Yreduc << temp_final;
	}
}

void reduction_2(hls::stream<v_dt>& Yaccum,hls::stream<float>& Yreduc,int M){

	reduction:
	for(int i=0;i<M;i++){
		#pragma hls pipeline II=1
		v_dt temp = Yaccum.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;
		Yreduc << tmp15;

	}
}




void write_y(hls::stream<float>& Yaccum, hls::stream<float>& Yin, float *Y, float beta, int M){

	write: for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Y[i] = Yaccum.read() + Yin.read()*beta;
	}
}

extern "C" {
void krnl_gemv(const char TransA, const int M, const int N, const float alpha, v_dt  *A, const int lda, v_dt  *X, const int incX,
		const float beta, float  *Y, const int incY) {

	#pragma HLS INTERFACE m_axi port = X offset = slave bundle = mem0 //HBM 0
	#pragma HLS INTERFACE m_axi port = A offset = slave bundle = mem1 //HBM 1
	#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = mem2 //HBM 0

	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = M
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = incX
	#pragma HLS INTERFACE s_axilite port = incY
	#pragma HLS INTERFACE s_axilite port = alpha
	#pragma HLS INTERFACE s_axilite port = beta

	hls::stream<v_dt> A_temp("Atemp");
	hls::stream<v_dt> X_temp("Xtemp");
	hls::stream<float> Y_temp("Y_in");
	hls::stream<v_dt> Y_accum[CHANNELS];
	hls::stream<v_dt> Y_reduc1("Y_reduc1");
	hls::stream<float> Y_reduc2("Y_reduc2");

	#pragma HLS STREAM variable = A_temp depth = 2048
	#pragma HLS STREAM variable = X_temp depth = 2048
	#pragma HLS STREAM variable = Y_temp depth = 2048
	#pragma HLS STREAM variable = Y_accum[0] depth = 2048
	#pragma HLS STREAM variable = Y_accum[1] depth = 2048
	#pragma HLS STREAM variable = Y_accum[2] depth = 2048
	#pragma HLS STREAM variable = Y_accum[3] depth = 2048
	#pragma HLS STREAM variable = Y_accum[4] depth = 2048
	#pragma HLS STREAM variable = Y_accum[5] depth = 2048
	#pragma HLS STREAM variable = Y_accum[6] depth = 2048
	#pragma HLS STREAM variable = Y_accum[7] depth = 2048
	#pragma HLS STREAM variable = Y_reduc1 depth = 2048
	#pragma HLS STREAM variable = Y_reduc2 depth = 2048


	#pragma HLS dataflow

	read_a(A_temp, A, M, N);

	read_x(X_temp, X, M, N);

	read_y(Y_temp, Y, M);

   	scal_accum(A_temp, X_temp , Y_accum, alpha, M, N);

	reduction_1(Y_accum, Y_reduc1, M);

	reduction_2(Y_reduc1, Y_reduc2, M);

	write_y(Y_reduc2, Y_temp, Y, beta, M);

}}
