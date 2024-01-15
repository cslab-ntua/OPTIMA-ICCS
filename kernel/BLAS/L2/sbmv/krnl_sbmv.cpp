#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
#define BRAM_SIZE 1024
#define CUs 10

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void read_y(hls::stream<float>& Yin, float *Y, const int N){

    read_y: for(int i=0;i<N;i++){
		#pragma HLS pipeline II=1
		Yin << Y[i];
	}
}

void write_nosym(float *Y, hls::stream<float>& Yin, hls::stream<float>& Y_nosym, const float beta, const int N){

	write_y: for(int i=0;i<N;i++){
		#pragma HLS pipeline II=1
		Y[i]=beta*Yin.read()+Y_nosym.read();
	}
}

void write_sym(float *Y, hls::stream<float> &Y_sym, const int N, const int K, const int start_offset){

	int length = 0;
	N+start_offset <= N*CUs - K  ? length = N : length = N-K;

	write_y: for(int i=0;i<length+K;i++){
		#pragma HLS pipeline II=1
		float y_temp = Y_sym.read();
		Y[i+start_offset]=y_temp;
	}
}

void read_x(hls::stream<v_dt>& X_nosym, hls::stream<float>& X_sym, const v_dt *X, const int N, const int K){

	int i=0,j=0;
	int N_cols = K/16 + 1;

	read_X: while(i<N){
		#pragma HLS pipeline II=1
		v_dt Xtemp = X[i*N_cols+j];

		float Xtemp_sym[VDATA_SIZE];
		#pragma HLS array_partition variable=Xtemp_sym complete
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			Xtemp_sym[k] = Xtemp.data[k];
		}

		int cond = K/VDATA_SIZE;

		X_nosym << Xtemp;

		if (j==0)
			X_sym << Xtemp_sym[0];

		j== cond ? i++ : i+=0;
		j== cond ? j=0 : j++;

	}
}

void read_matrix(const v_dt *A, hls::stream<v_dt>& A_nosym, hls::stream<v_dt>& A_sym, const int N, const int K){

	int i=0,j=0;
	int N_cols = K/16 + 1;

	wide_read:
	while(i<N){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i*N_cols+j];
		A_nosym << Atemp;
		A_sym << Atemp;

		int cond = K/VDATA_SIZE;
		j == cond ? i++ : i+=0;
		j == cond ? j=0 : j++;

	}
}

void scal_nosym(hls::stream<v_dt>& A, hls::stream<v_dt>& X, hls::stream<v_dt>& Y, const float alpha, const int N, const int K){

	int i=0,j=0;

	scal_nosym: while(i<N){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		int cond = K/VDATA_SIZE;

		for (int k=0;k<VDATA_SIZE;k++){
			#pragma HLS UNROLL
			if (k<hls::fmin(VDATA_SIZE,K+1-j*VDATA_SIZE)){
				temp.data[k] = alpha*X_temp.data[k]*A_temp.data[k];
		    }
		    else{
		    	temp.data[k]=0;
		    }
		}

		Y << temp;

		j== cond ? i++ : i+=0;
		j== cond ? j=0 : j++;
	}
}

void scal_sym(hls::stream<v_dt>& A, hls::stream<float>& X, hls::stream<v_dt>& Y,const float alpha,const int N, const int K){

	int i=0,j=0;
	int N_cols = K/16 + 1;

	float X_in=0;

	scal_sym: while(i<N){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt temp;


		int cond = K/VDATA_SIZE;

		if(j==0)
			X_in = X.read();

		for (int k=0;k<VDATA_SIZE;k++){
			#pragma HLS UNROLL
			if (k<hls::fmin(VDATA_SIZE,K+1-j*VDATA_SIZE) and !(k==0 and j==0)){
				temp.data[k] = alpha*X_in*A_temp.data[k];
		    }
		    else{
		    	temp.data[k]=0;
		    }
		}
		Y << temp;

		j== cond ? i++ : i+=0;
		j== cond ? j=0 : j++;
	}
}

void accum_nosym(hls::stream<v_dt>& In, hls::stream<v_dt>& Out, const int N, const int K){

    non_pipelined: for(int i=0;i<N;i++){
    	v_dt Y_temp;

        for(int k=0;k<VDATA_SIZE;k++){
        	#pragma HLS unroll
            Y_temp.data[k]=0.0f;
        }

        pipelined: for(int j=0;j<K+1;j+=VDATA_SIZE){
        	#pragma hls pipeline II=1
            v_dt temp = In.read();

            for(int k=0;k<VDATA_SIZE;k++){
            	#pragma HLS unroll
                Y_temp.data[k] +=temp.data[k];
            }

        }
        Out << Y_temp;
    }
}

void reduction(hls::stream<v_dt>& In, hls::stream<float>& Out, const int N){

	reduction: for(int i=0;i<N;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

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

		Out << tmp15;

	}
}

void accum_sym(hls::stream<v_dt>& In,hls::stream<float>& Out, const int N, const int K, const int start_offset){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	non_pipelined_nosym: for(int i=0;i<N;i++){

		pipelined_nosym: for(int j=0; j<N_cols; j+=VDATA_SIZE){
			#pragma HLS pipeline II=1
			v_dt temp = In.read();

			float scal[VDATA_SIZE];
			#pragma HLS array_partition variable=scal complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				scal[k]=temp.data[k];
			}

			if (i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + scal[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + scal[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + scal[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + scal[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + scal[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + scal[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + scal[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + scal[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + scal[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + scal[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + scal[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + scal[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + scal[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + scal[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + scal[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + scal[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + scal[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + scal[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + scal[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + scal[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + scal[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + scal[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + scal[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + scal[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + scal[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + scal[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + scal[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + scal[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + scal[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + scal[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + scal[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + scal[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

		}
		if (hls::fmod(i,2)==0)
			Out << bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		else
			Out << bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
	}

	int offset;
	offset = (N+start_offset <= N*CUs - K) ?  N : N-K;

	rest_sym:for(int i=N; i<offset+K; i++){
		#pragma HLS pipeline II=1
		if (hls::fmod(N-1,2)==0)
			Out << bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		else
			Out << bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
	}
}

//layout is for specific FPGA to check available sizes
extern "C" {
void krnl_sbmv (const char Uplo, const int N, const int K, const float alpha, const v_dt *A1,  const v_dt *A2, const v_dt *A3, const v_dt *A4,
		const v_dt *A5, const v_dt *A6, const v_dt *A7, const v_dt *A8, const v_dt *A9, const v_dt *A10, const int lda, const v_dt *X1, 
		const v_dt *X2, const v_dt *X3, const v_dt *X4, const v_dt *X5, const v_dt *X6, const v_dt *X7, const v_dt *X8, const v_dt *X9, 
		const v_dt *X10, const int incX, const float beta, float *Y1_nosym, float *Y2_nosym, float *Y3_nosym, float *Y4_nosym, float *Y5_nosym, 
		float *Y6_nosym, float *Y7_nosym, float *Y8_nosym, float *Y9_nosym, float *Y10_nosym, const int incY, float *Y1_sym, float *Y2_sym, 
		float *Y3_sym, float *Y4_sym, float *Y5_sym, float *Y6_sym, float *Y7_sym, float *Y8_sym, float *Y9_sym, float *Y10_sym){

	#pragma HLS INTERFACE m_axi port = X1 offset = slave bundle = ddr0
	#pragma HLS INTERFACE m_axi port = X2 offset = slave bundle = ddr1
	#pragma HLS INTERFACE m_axi port = X3 offset = slave bundle = ddr2
	#pragma HLS INTERFACE m_axi port = X4 offset = slave bundle = ddr3
	#pragma HLS INTERFACE m_axi port = X5 offset = slave bundle = ddr4
	#pragma HLS INTERFACE m_axi port = X6 offset = slave bundle = ddr5
	#pragma HLS INTERFACE m_axi port = X7 offset = slave bundle = ddr6
	#pragma HLS INTERFACE m_axi port = X8 offset = slave bundle = ddr7
	#pragma HLS INTERFACE m_axi port = X9 offset = slave bundle = ddr8
	#pragma HLS INTERFACE m_axi port = X10 offset = slave bundle = ddr9
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = ddr10
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = ddr11
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = ddr12
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = ddr13
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = ddr14
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = ddr15
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = ddr16
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = ddr17
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = ddr18
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = ddr19
	#pragma HLS INTERFACE m_axi port = Y1_nosym offset = slave bundle = ddr20
	#pragma HLS INTERFACE m_axi port = Y2_nosym offset = slave bundle = ddr21
	#pragma HLS INTERFACE m_axi port = Y3_nosym offset = slave bundle = ddr22
	#pragma HLS INTERFACE m_axi port = Y4_nosym offset = slave bundle = ddr23
	#pragma HLS INTERFACE m_axi port = Y5_nosym offset = slave bundle = ddr24
	#pragma HLS INTERFACE m_axi port = Y6_nosym offset = slave bundle = ddr25
	#pragma HLS INTERFACE m_axi port = Y7_nosym offset = slave bundle = ddr26
	#pragma HLS INTERFACE m_axi port = Y8_nosym offset = slave bundle = ddr27
	#pragma HLS INTERFACE m_axi port = Y9_nosym offset = slave bundle = ddr28
	#pragma HLS INTERFACE m_axi port = Y10_nosym offset = slave bundle = ddr29
	#pragma HLS INTERFACE m_axi port = Y1_sym offset = slave bundle = ddr0
	#pragma HLS INTERFACE m_axi port = Y2_sym offset = slave bundle = ddr1
	#pragma HLS INTERFACE m_axi port = Y3_sym offset = slave bundle = ddr2
	#pragma HLS INTERFACE m_axi port = Y4_sym offset = slave bundle = ddr3
	#pragma HLS INTERFACE m_axi port = Y5_sym offset = slave bundle = ddr4
	#pragma HLS INTERFACE m_axi port = Y6_sym offset = slave bundle = ddr5
	#pragma HLS INTERFACE m_axi port = Y7_sym offset = slave bundle = ddr6
	#pragma HLS INTERFACE m_axi port = Y8_sym offset = slave bundle = ddr7
	#pragma HLS INTERFACE m_axi port = Y9_sym offset = slave bundle = ddr8
	#pragma HLS INTERFACE m_axi port = Y10_sym offset = slave bundle = ddr9

	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = Uplo
	#pragma HLS INTERFACE s_axilite port = incX
	#pragma HLS INTERFACE s_axilite port = incY
	#pragma HLS INTERFACE s_axilite port = alpha
	#pragma HLS INTERFACE s_axilite port = beta

	static hls::stream<float> Y1_in("Y1_in");
	static hls::stream<v_dt> A1_nosym("A1_nosym");
	static hls::stream<v_dt> X1_nosym("X1_nosym");
	static hls::stream<v_dt> A1_sym("A1_sym");
	static hls::stream<float> X1_sym("X1_sym");
	static hls::stream<v_dt> Y1_noscal("Y1_noscal");
	static hls::stream<v_dt> Y1_noaccum("Y1_noaccum");
	static hls::stream<float> Y1_reduct("Y1_reduct");
	static hls::stream<v_dt> Y1_scal("Y1_scal");
    static hls::stream<float> Y1_accum("Y1_accum");

	static hls::stream<float> Y2_in("Y2_in");
	static hls::stream<v_dt> A2_nosym("A2_nosym");
	static hls::stream<v_dt> X2_nosym("X2_nosym");
	static hls::stream<v_dt> A2_sym("A2_sym");
	static hls::stream<float> X2_sym("X2_sym");
	static hls::stream<v_dt> Y2_noscal("Y2_noscal");
	static hls::stream<v_dt> Y2_noaccum("Y2_noaccum");
	static hls::stream<float> Y2_reduct("Y2_reduct");
	static hls::stream<v_dt> Y2_scal("Y2_scal");
    static hls::stream<float> Y2_accum("Y2_accum");

	static hls::stream<float> Y3_in("Y3_in");
	static hls::stream<v_dt> A3_nosym("A3_nosym");
	static hls::stream<v_dt> X3_nosym("X3_nosym");
	static hls::stream<v_dt> A3_sym("A3_sym");
	static hls::stream<float> X3_sym("X3_sym");
	static hls::stream<v_dt> Y3_noscal("Y3_noscal");
	static hls::stream<v_dt> Y3_noaccum("Y3_noaccum");
	static hls::stream<float> Y3_reduct("Y3_reduct");
	static hls::stream<v_dt> Y3_scal("Y3_scal");
    static hls::stream<float> Y3_accum("Y3_accum");

	static hls::stream<float> Y4_in("Y4_in");
	static hls::stream<v_dt> A4_nosym("A4_nosym");
	static hls::stream<v_dt> X4_nosym("X4_nosym");
	static hls::stream<v_dt> A4_sym("A4_sym");
	static hls::stream<float> X4_sym("X4_sym");
	static hls::stream<v_dt> Y4_noscal("Y4_noscal");
	static hls::stream<v_dt> Y4_noaccum("Y4_noaccum");
	static hls::stream<float> Y4_reduct("Y4_reduct");
	static hls::stream<v_dt> Y4_scal("Y4_scal");
    static hls::stream<float> Y4_accum("Y4_accum");

	static hls::stream<float> Y5_in("Y5_in");
	static hls::stream<v_dt> A5_nosym("A5_nosym");
	static hls::stream<v_dt> X5_nosym("X5_nosym");
	static hls::stream<v_dt> A5_sym("A5_sym");
	static hls::stream<float> X5_sym("X5_sym");
	static hls::stream<v_dt> Y5_noscal("Y5_noscal");
	static hls::stream<v_dt> Y5_noaccum("Y5_noaccum");
	static hls::stream<float> Y5_reduct("Y5_reduct");
	static hls::stream<v_dt> Y5_scal("Y5_scal");
    static hls::stream<float> Y5_accum("Y5_accum");

	static hls::stream<float> Y6_in("Y6_in");
	static hls::stream<v_dt> A6_nosym("A6_nosym");
	static hls::stream<v_dt> X6_nosym("X6_nosym");
	static hls::stream<v_dt> A6_sym("A6_sym");
	static hls::stream<float> X6_sym("X6_sym");
	static hls::stream<v_dt> Y6_noscal("Y6_noscal");
	static hls::stream<v_dt> Y6_noaccum("Y6_noaccum");
	static hls::stream<float> Y6_reduct("Y6_reduct");
	static hls::stream<v_dt> Y6_scal("Y6_scal");
    static hls::stream<float> Y6_accum("Y6_accum");

   	static hls::stream<float> Y7_in("Y7_in");
	static hls::stream<v_dt> A7_nosym("A7_nosym");
	static hls::stream<v_dt> X7_nosym("X7_nosym");
	static hls::stream<v_dt> A7_sym("A7_sym");
	static hls::stream<float> X7_sym("X7_sym");
	static hls::stream<v_dt> Y7_noscal("Y7_noscal");
	static hls::stream<v_dt> Y7_noaccum("Y7_noaccum");
	static hls::stream<float> Y7_reduct("Y7_reduct");
	static hls::stream<v_dt> Y7_scal("Y7_scal");
    static hls::stream<float> Y7_accum("Y7_accum");

   	static hls::stream<float> Y8_in("Y8_in");
	static hls::stream<v_dt> A8_nosym("A8_nosym");
	static hls::stream<v_dt> X8_nosym("X8_nosym");
	static hls::stream<v_dt> A8_sym("A8_sym");
	static hls::stream<float> X8_sym("X8_sym");
	static hls::stream<v_dt> Y8_noscal("Y8_noscal");
	static hls::stream<v_dt> Y8_noaccum("Y8_noaccum");
	static hls::stream<float> Y8_reduct("Y8_reduct");
	static hls::stream<v_dt> Y8_scal("Y8_scal");
    static hls::stream<float> Y8_accum("Y8_accum");

    static hls::stream<float> Y9_in("Y9_in");
	static hls::stream<v_dt> A9_nosym("A9_nosym");
	static hls::stream<v_dt> X9_nosym("X9_nosym");
	static hls::stream<v_dt> A9_sym("A9_sym");
	static hls::stream<float> X9_sym("X9_sym");
	static hls::stream<v_dt> Y9_noscal("Y9_noscal");
	static hls::stream<v_dt> Y9_noaccum("Y9_noaccum");
	static hls::stream<float> Y9_reduct("Y9_reduct");
	static hls::stream<v_dt> Y9_scal("Y9_scal");
    static hls::stream<float> Y9_accum("Y9_accum");

    static hls::stream<float> Y10_in("Y10_in");
	static hls::stream<v_dt> A10_nosym("A10_nosym");
	static hls::stream<v_dt> X10_nosym("X10_nosym");
	static hls::stream<v_dt> A10_sym("A10_sym");
	static hls::stream<float> X10_sym("X10_sym");
	static hls::stream<v_dt> Y10_noscal("Y10_noscal");
	static hls::stream<v_dt> Y10_noaccum("Y10_noaccum");
	static hls::stream<float> Y10_reduct("Y10_reduct");
	static hls::stream<v_dt> Y10_scal("Y10_scal");
    static hls::stream<float> Y10_accum("Y10_accum");

	#pragma HLS STREAM variable = Y1_in depth = 2048
	#pragma HLS STREAM variable = A1_nosym depth = 2048
	#pragma HLS STREAM variable = X1_nosym depth = 2048
	#pragma HLS STREAM variable = A1_sym depth = 2048
	#pragma HLS STREAM variable = X1_sym depth = 2048
	#pragma HLS STREAM variable = Y1_noscal depth = 2048
	#pragma HLS STREAM variable = Y1_noaccum depth = 2048
	#pragma HLS STREAM variable = Y1_reduct depth = 2048
	#pragma HLS STREAM variable = Y1_scal depth = 2048
	#pragma HLS STREAM variable = Y1_accum depth = 2048

	#pragma HLS STREAM variable = Y2_in depth = 2048
	#pragma HLS STREAM variable = A2_nosym depth = 2048
	#pragma HLS STREAM variable = X2_nosym depth = 2048
	#pragma HLS STREAM variable = A2_sym depth = 2048
	#pragma HLS STREAM variable = X2_sym depth = 2048
	#pragma HLS STREAM variable = Y2_noscal depth = 2048
	#pragma HLS STREAM variable = Y2_noaccum depth = 2048
	#pragma HLS STREAM variable = Y2_reduct depth = 2048
	#pragma HLS STREAM variable = Y2_scal depth = 2048
	#pragma HLS STREAM variable = Y2_accum depth = 2048

	#pragma HLS STREAM variable = Y3_in depth = 2048
	#pragma HLS STREAM variable = A3_nosym depth = 2048
	#pragma HLS STREAM variable = X3_nosym depth = 2048
	#pragma HLS STREAM variable = A3_sym depth = 2048
	#pragma HLS STREAM variable = X3_sym depth = 2048
	#pragma HLS STREAM variable = Y3_noscal depth = 2048
	#pragma HLS STREAM variable = Y3_noaccum depth = 2048
	#pragma HLS STREAM variable = Y3_reduct depth = 2048
	#pragma HLS STREAM variable = Y3_scal depth = 2048
	#pragma HLS STREAM variable = Y3_accum depth = 2048

	#pragma HLS STREAM variable = Y4_in depth = 2048
	#pragma HLS STREAM variable = A4_nosym depth = 2048
	#pragma HLS STREAM variable = X4_nosym depth = 2048
	#pragma HLS STREAM variable = A4_sym depth = 2048
	#pragma HLS STREAM variable = X4_sym depth = 2048
	#pragma HLS STREAM variable = Y4_noscal depth = 2048
	#pragma HLS STREAM variable = Y4_noaccum depth = 2048
	#pragma HLS STREAM variable = Y4_reduct depth = 2048
	#pragma HLS STREAM variable = Y4_scal depth = 2048
	#pragma HLS STREAM variable = Y4_accum depth = 2048

	#pragma HLS STREAM variable = Y5_in depth = 2048
	#pragma HLS STREAM variable = A5_nosym depth = 2048
	#pragma HLS STREAM variable = X5_nosym depth = 2048
	#pragma HLS STREAM variable = A5_sym depth = 2048
	#pragma HLS STREAM variable = X5_sym depth = 2048
	#pragma HLS STREAM variable = Y5_noscal depth = 2048
	#pragma HLS STREAM variable = Y5_noaccum depth = 2048
	#pragma HLS STREAM variable = Y5_reduct depth = 2048
	#pragma HLS STREAM variable = Y5_scal depth = 2048
	#pragma HLS STREAM variable = Y5_accum depth = 2048

	#pragma HLS STREAM variable = Y6_in depth = 2048
	#pragma HLS STREAM variable = A6_nosym depth = 2048
	#pragma HLS STREAM variable = X6_nosym depth = 2048
	#pragma HLS STREAM variable = A6_sym depth = 2048
	#pragma HLS STREAM variable = X6_sym depth = 2048
	#pragma HLS STREAM variable = Y6_noscal depth = 2048
	#pragma HLS STREAM variable = Y6_noaccum depth = 2048
	#pragma HLS STREAM variable = Y6_reduct depth = 2048
	#pragma HLS STREAM variable = Y6_scal depth = 2048
	#pragma HLS STREAM variable = Y6_accum depth = 2048

	#pragma HLS STREAM variable = Y7_in depth = 2048
	#pragma HLS STREAM variable = A7_nosym depth = 2048
	#pragma HLS STREAM variable = X7_nosym depth = 2048
	#pragma HLS STREAM variable = A7_sym depth = 2048
	#pragma HLS STREAM variable = X7_sym depth = 2048
	#pragma HLS STREAM variable = Y7_noscal depth = 2048
	#pragma HLS STREAM variable = Y7_noaccum depth = 2048
	#pragma HLS STREAM variable = Y7_reduct depth = 2048
	#pragma HLS STREAM variable = Y7_scal depth = 2048
	#pragma HLS STREAM variable = Y7_accum depth = 2048

	#pragma HLS STREAM variable = Y8_in depth = 2048
	#pragma HLS STREAM variable = A8_nosym depth = 2048
	#pragma HLS STREAM variable = X8_nosym depth = 2048
	#pragma HLS STREAM variable = A8_sym depth = 2048
	#pragma HLS STREAM variable = X8_sym depth = 2048
	#pragma HLS STREAM variable = Y8_noscal depth = 2048
	#pragma HLS STREAM variable = Y8_noaccum depth = 2048
	#pragma HLS STREAM variable = Y8_reduct depth = 2048
	#pragma HLS STREAM variable = Y8_scal depth = 2048
	#pragma HLS STREAM variable = Y8_accum depth = 2048

	#pragma HLS STREAM variable = Y9_in depth = 2048
	#pragma HLS STREAM variable = A9_nosym depth = 2048
	#pragma HLS STREAM variable = X9_nosym depth = 2048
	#pragma HLS STREAM variable = A9_sym depth = 2048
	#pragma HLS STREAM variable = X9_sym depth = 2048
	#pragma HLS STREAM variable = Y9_noscal depth = 2048
	#pragma HLS STREAM variable = Y9_noaccum depth = 2048
	#pragma HLS STREAM variable = Y9_reduct depth = 2048
	#pragma HLS STREAM variable = Y9_scal depth = 2048
	#pragma HLS STREAM variable = Y9_accum depth = 2048

	#pragma HLS STREAM variable = Y10_in depth = 2048
	#pragma HLS STREAM variable = A10_nosym depth = 2048
	#pragma HLS STREAM variable = X10_nosym depth = 2048
	#pragma HLS STREAM variable = A10_sym depth = 2048
	#pragma HLS STREAM variable = X10_sym depth = 2048
	#pragma HLS STREAM variable = Y10_noscal depth = 2048
	#pragma HLS STREAM variable = Y10_noaccum depth = 2048
	#pragma HLS STREAM variable = Y10_reduct depth = 2048
	#pragma HLS STREAM variable = Y10_scal depth = 2048
	#pragma HLS STREAM variable = Y10_accum depth = 2048

	#pragma HLS dataflow

	read_y(Y1_in, Y1_nosym, N/CUs);
	read_y(Y2_in, Y2_nosym, N/CUs);
	read_y(Y3_in, Y3_nosym, N/CUs);
	read_y(Y4_in, Y4_nosym, N/CUs);
	read_y(Y5_in, Y5_nosym, N/CUs);
	read_y(Y6_in, Y6_nosym, N/CUs);
	read_y(Y7_in, Y7_nosym, N/CUs);
	read_y(Y8_in, Y8_nosym, N/CUs);
	read_y(Y9_in, Y9_nosym, N/CUs);
	read_y(Y10_in, Y10_nosym, N/CUs);

	read_x(X1_nosym, X1_sym, X1, N/CUs, K);
	read_x(X2_nosym, X2_sym, X2, N/CUs, K);
	read_x(X3_nosym, X3_sym, X3, N/CUs, K);
	read_x(X4_nosym, X4_sym, X4, N/CUs, K);
	read_x(X5_nosym, X5_sym, X5, N/CUs, K);
	read_x(X6_nosym, X6_sym, X6, N/CUs, K);
	read_x(X7_nosym, X7_sym, X7, N/CUs, K);
	read_x(X8_nosym, X8_sym, X8, N/CUs, K);
	read_x(X9_nosym, X9_sym, X9, N/CUs, K);
	read_x(X10_nosym, X10_sym, X10, N/CUs, K);

	read_matrix(A1, A1_nosym, A1_sym, N/CUs, K);
	read_matrix(A2, A2_nosym, A2_sym, N/CUs, K);
	read_matrix(A3, A3_nosym, A3_sym, N/CUs, K);
	read_matrix(A4, A4_nosym, A4_sym, N/CUs, K);
	read_matrix(A5, A5_nosym, A5_sym, N/CUs, K);
	read_matrix(A6, A6_nosym, A6_sym, N/CUs, K);
	read_matrix(A7, A7_nosym, A7_sym, N/CUs, K);
	read_matrix(A8, A8_nosym, A8_sym, N/CUs, K);
	read_matrix(A9, A9_nosym, A9_sym, N/CUs, K);
	read_matrix(A10, A10_nosym, A10_sym, N/CUs, K);

	scal_nosym(A1_nosym, X1_nosym, Y1_noscal, alpha, N/CUs, K);
	scal_nosym(A2_nosym, X2_nosym, Y2_noscal, alpha, N/CUs, K);
	scal_nosym(A3_nosym, X3_nosym, Y3_noscal, alpha, N/CUs, K);
	scal_nosym(A4_nosym, X4_nosym, Y4_noscal, alpha, N/CUs, K);
	scal_nosym(A5_nosym, X5_nosym, Y5_noscal, alpha, N/CUs, K);
	scal_nosym(A6_nosym, X6_nosym, Y6_noscal, alpha, N/CUs, K);
	scal_nosym(A7_nosym, X7_nosym, Y7_noscal, alpha, N/CUs, K);
	scal_nosym(A8_nosym, X8_nosym, Y8_noscal, alpha, N/CUs, K);
	scal_nosym(A9_nosym, X9_nosym, Y9_noscal, alpha, N/CUs, K);
	scal_nosym(A10_nosym, X10_nosym, Y10_noscal, alpha, N/CUs, K);

	accum_nosym(Y1_noscal, Y1_noaccum, N/CUs, K);
	accum_nosym(Y2_noscal, Y2_noaccum, N/CUs, K);
	accum_nosym(Y3_noscal, Y3_noaccum, N/CUs, K);
	accum_nosym(Y4_noscal, Y4_noaccum, N/CUs, K);
	accum_nosym(Y5_noscal, Y5_noaccum, N/CUs, K);
	accum_nosym(Y6_noscal, Y6_noaccum, N/CUs, K);
	accum_nosym(Y7_noscal, Y7_noaccum, N/CUs, K);
	accum_nosym(Y8_noscal, Y8_noaccum, N/CUs, K);
	accum_nosym(Y9_noscal, Y9_noaccum, N/CUs, K);
	accum_nosym(Y10_noscal, Y10_noaccum, N/CUs, K);

	reduction(Y1_noaccum, Y1_reduct, N/CUs);
	reduction(Y2_noaccum, Y2_reduct, N/CUs);
	reduction(Y3_noaccum, Y3_reduct, N/CUs);
	reduction(Y4_noaccum, Y4_reduct, N/CUs);
	reduction(Y5_noaccum, Y5_reduct, N/CUs);
	reduction(Y6_noaccum, Y6_reduct, N/CUs);
	reduction(Y7_noaccum, Y7_reduct, N/CUs);
	reduction(Y8_noaccum, Y8_reduct, N/CUs);
	reduction(Y9_noaccum, Y9_reduct, N/CUs);
	reduction(Y10_noaccum, Y10_reduct, N/CUs);

	scal_sym(A1_sym, X1_sym, Y1_scal, alpha, N/CUs, K);
	scal_sym(A2_sym, X2_sym, Y2_scal, alpha, N/CUs, K);
	scal_sym(A3_sym, X3_sym, Y3_scal, alpha, N/CUs, K);
	scal_sym(A4_sym, X4_sym, Y4_scal, alpha, N/CUs, K);
	scal_sym(A5_sym, X5_sym, Y5_scal, alpha, N/CUs, K);
	scal_sym(A6_sym, X6_sym, Y6_scal, alpha, N/CUs, K);
	scal_sym(A7_sym, X7_sym, Y7_scal, alpha, N/CUs, K);
	scal_sym(A8_sym, X8_sym, Y8_scal, alpha, N/CUs, K);
	scal_sym(A9_sym, X9_sym, Y9_scal, alpha, N/CUs, K);
	scal_sym(A10_sym, X10_sym, Y10_scal, alpha, N/CUs, K);

	accum_sym(Y1_scal, Y1_accum, N/CUs, K, 0*N/CUs);
	accum_sym(Y2_scal, Y2_accum, N/CUs, K, 1*N/CUs);
	accum_sym(Y3_scal, Y3_accum, N/CUs, K, 2*N/CUs);
	accum_sym(Y4_scal, Y4_accum, N/CUs, K, 3*N/CUs);
	accum_sym(Y5_scal, Y5_accum, N/CUs, K, 4*N/CUs);
	accum_sym(Y6_scal, Y6_accum, N/CUs, K, 5*N/CUs);
	accum_sym(Y7_scal, Y7_accum, N/CUs, K, 6*N/CUs);
	accum_sym(Y8_scal, Y8_accum, N/CUs, K, 7*N/CUs);
	accum_sym(Y9_scal, Y9_accum, N/CUs, K, 8*N/CUs);
	accum_sym(Y10_scal, Y10_accum, N/CUs, K, 9*N/CUs);

	write_nosym(Y1_nosym, Y1_in, Y1_reduct, beta, N/CUs);
	write_nosym(Y2_nosym, Y2_in, Y2_reduct, beta, N/CUs);
	write_nosym(Y3_nosym, Y3_in, Y3_reduct, beta, N/CUs);
	write_nosym(Y4_nosym, Y4_in, Y4_reduct, beta, N/CUs);
	write_nosym(Y5_nosym, Y5_in, Y5_reduct, beta, N/CUs);
	write_nosym(Y6_nosym, Y6_in, Y6_reduct, beta, N/CUs);
	write_nosym(Y7_nosym, Y7_in, Y7_reduct, beta, N/CUs);
	write_nosym(Y8_nosym, Y8_in, Y8_reduct, beta, N/CUs);
	write_nosym(Y9_nosym, Y9_in, Y9_reduct, beta, N/CUs);
	write_nosym(Y10_nosym, Y10_in, Y10_reduct, beta, N/CUs);

	write_sym(Y1_sym, Y1_accum, N/CUs, K, 0*N/CUs);
	write_sym(Y2_sym, Y2_accum, N/CUs, K, 1*N/CUs);
	write_sym(Y3_sym, Y3_accum, N/CUs, K, 2*N/CUs);
	write_sym(Y4_sym, Y4_accum, N/CUs, K, 3*N/CUs);
	write_sym(Y5_sym, Y5_accum, N/CUs, K, 4*N/CUs);
	write_sym(Y6_sym, Y6_accum, N/CUs, K, 5*N/CUs);
	write_sym(Y7_sym, Y7_accum, N/CUs, K, 6*N/CUs);
	write_sym(Y8_sym, Y8_accum, N/CUs, K, 7*N/CUs);
	write_sym(Y9_sym, Y9_accum, N/CUs, K, 8*N/CUs);
	write_sym(Y10_sym, Y10_accum, N/CUs, K, 9*N/CUs);

}}
