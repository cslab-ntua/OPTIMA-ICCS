#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
#define BRAM_SIZE 1024

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void read_y(const char Uplo, hls::stream<float>& Yin, float *Y, const int end_row){

    read_y: for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Yin << Y[i];
	}
}

void write_nosym(float *Y, hls::stream<float>& Yin, hls::stream<float>& Y_nosym, const float beta, const int end_row){

	write_y: for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i]=beta*Yin.read()+Y_nosym.read();
	}
}

void write_sym(float *Y, hls::stream<float> &Y_sym, const int N, const int start_row){

	write_y: for(int i=0;i<N-start_row;i++){
		#pragma HLS pipeline II=1
		float y_temp = Y_sym.read();
		Y[i+start_row]=y_temp;
	}
}

void read_x(const char Uplo, hls::stream<v_dt>& X_nosym, hls::stream<float>& X_sym, const v_dt *X, const int N, const int start_row, const int end_row){

	int i=0,j=start_row/VDATA_SIZE;

	read_X: while(i<end_row){
		#pragma HLS pipeline II=1
		v_dt Xtemp = X[j];
		float Xtemp_sym[VDATA_SIZE];
		#pragma HLS array_partition variable=Xtemp_sym complete

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			Xtemp_sym[k] = Xtemp.data[k];
		}

		int cond = N/VDATA_SIZE - 1;
		int res = (i+start_row)/VDATA_SIZE;

		X_nosym << Xtemp;

		if (j==(i+start_row)/VDATA_SIZE)
			X_sym << Xtemp_sym[(i+start_row)%VDATA_SIZE];

		j==cond ? i++ : i+=0;
		j==cond ? j=res : j++;

	}
}

void read_matrix(const char Uplo,const v_dt *A, hls::stream<v_dt>& A_nosym, hls::stream<v_dt>& A_sym, const int N, const int start_row, const int end_row){

	int i=0, j=start_row/VDATA_SIZE;

	wide_read:
	while(i<end_row){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i*(N/VDATA_SIZE)+j];
		A_sym << Atemp;
		A_nosym << Atemp;

		int cond = N/VDATA_SIZE - 1;
		int res = (i+start_row)/VDATA_SIZE;

		j==cond ? i++ : i+=0;
		j==cond ? j=res : j++;

	}
}

void scal_nosym(const char Uplo, hls::stream<v_dt>& A, hls::stream<v_dt>& X, hls::stream<v_dt>& Y,const float alpha,const int N, const int start_row, const int end_row){

	int i=0,j=start_row/VDATA_SIZE;

	scal_nosym: while(i<end_row){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		int cond1=(i+start_row)/VDATA_SIZE;
		int cond2=(i+start_row)%VDATA_SIZE;

		int cond = N/VDATA_SIZE - 1;
		int res = (i+start_row)/VDATA_SIZE;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			j==cond1 and k<cond2 ? temp.data[k]=0 : temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
		}

		Y << temp;

		j==cond ? i++ : i+=0;
		j==cond ? j=res : j++;
	}
}

void scal_sym(const char Uplo,hls::stream<v_dt>& A, hls::stream<float>& X, hls::stream<v_dt>& Y,const float alpha,const int N, const int start_row, const int end_row){

	int i=0,j=start_row/VDATA_SIZE;

	float X_in=0;

	scal_sym: while(i<end_row){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt temp;

		int cond1=(i+start_row)/VDATA_SIZE;
		int cond2=(i+start_row)%VDATA_SIZE;

		int cond = N/VDATA_SIZE - 1;
		int res = (i+start_row)/VDATA_SIZE;

		if(j==cond1)
			X_in = X.read();

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			j==cond1 and k<=cond2 ? temp.data[k]=0 : temp.data[k]=alpha*X_in*A_temp.data[k];
		}

		Y << temp;

		j==cond ? i++ : i+=0;
		j==cond ? j=res : j++;
	}
}

void accum_nosym(const char Uplo,hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row){

	v_dt X_temp;

	int i=0,j=start_row/VDATA_SIZE;

	nested_pipelined: while(i<end_row){
		#pragma HLS pipeline II=1

		if(j==(start_row+i)/VDATA_SIZE){
			for(int k=0;k<VDATA_SIZE;k++){
	        		#pragma HLS unroll
	            		X_temp.data[k]=0.0f;
	        	}
		}

	    	v_dt temp = In.read();

        	int cond = N/VDATA_SIZE - 1;
        	int res = (i+start_row)/VDATA_SIZE;


	    	for(int k=0;k<VDATA_SIZE;k++){
	    		#pragma HLS unroll
	        	X_temp.data[k] +=temp.data[k];
	    	}

	    	if (j==cond) Out << X_temp;

	    	j==cond ? i++ : i+=0;
	    	j==cond ? j=res : j++;
	}

}

void reduction(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

	reduction: for(int i=0;i<end_row;i++){
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

void accum_sym(const char Uplo,hls::stream<v_dt>& In,hls::stream<float>& Out,const int N, const int start_row, const int end_row){


	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	int length=start_row;

	non_pipelined_nosym: for(int i=0;i<end_row;i++){

		pipelined_nosym: for(int j=0; j<N-length; j+=VDATA_SIZE){
			#pragma HLS pipeline II=1
			v_dt temp = In.read();

			if (i%2==0){
				if(i==0){
					for (int k=0;k<VDATA_SIZE;k++){
						#pragma HLS UNROLL
						bram_pong[k][(j+length)/VDATA_SIZE] = temp.data[k];
					}
				}
				else{
					for (int k=0;k<VDATA_SIZE;k++){
						#pragma HLS UNROLL
						bram_pong[k][(j+length)/VDATA_SIZE] = bram_ping[k][(j+length)/VDATA_SIZE] + temp.data[k];
					}
				}

			}
			else{
				for (int k=0;k<VDATA_SIZE;k++){
					#pragma HLS UNROLL
					bram_ping[k][(j+length)/VDATA_SIZE] = bram_pong[k][(j+length)/VDATA_SIZE] + temp.data[k];
				}
			}

		}

		length = start_row + i;
		if (hls::fmod(i,2)==0)
			Out << bram_pong[(i+start_row)%VDATA_SIZE][(i+start_row)/VDATA_SIZE];
		else
			Out << bram_ping[(i+start_row)%VDATA_SIZE][(i+start_row)/VDATA_SIZE];

	}


	rest_sym:for(int i=end_row+start_row; i<N; i++){
		#pragma HLS pipeline II=1
		if (hls::fmod(end_row-1,2)==0)
			Out << bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		else
			Out << bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
	}

}

//layout is for specific FPGA to check available sizes
extern "C" {
void krnl_symv (const char Uplo, const int N, const float alpha, const int break_row1, const int break_row2, const int break_row3, const int break_row4, 
		const int break_row5, const int break_row6, const int break_row7, const int break_row8, const int break_row9, const v_dt *A1, const v_dt *A2,
		const v_dt *A3, const v_dt *A4, const v_dt *A5, const v_dt *A6, const v_dt *A7, const v_dt *A8, const v_dt *A9, const v_dt *A10, 
		const int lda, const v_dt *X1, const v_dt *X2, const v_dt *X3, const v_dt *X4, const v_dt *X5, const v_dt *X6, const v_dt *X7, 
		const v_dt *X8, const v_dt *X9, const v_dt *X10, const int incX, const float beta, float *Y1_nosym, float *Y2_nosym, float *Y3_nosym, 
		float *Y4_nosym, float *Y5_nosym, float *Y6_nosym, float *Y7_nosym, float *Y8_nosym, float *Y9_nosym, float *Y10_nosym, const int incY, 
		float *Y1_sym, float *Y2_sym, float *Y3_sym, float *Y4_sym, float *Y5_sym, float *Y6_sym, float *Y7_sym, float *Y8_sym, float *Y9_sym, 
		float *Y10_sym){

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
	#pragma HLS INTERFACE s_axilite port = break_row1
	#pragma HLS INTERFACE s_axilite port = break_row2
	#pragma HLS INTERFACE s_axilite port = break_row3
	#pragma HLS INTERFACE s_axilite port = break_row4
	#pragma HLS INTERFACE s_axilite port = break_row5
	#pragma HLS INTERFACE s_axilite port = break_row6
	#pragma HLS INTERFACE s_axilite port = break_row7
	#pragma HLS INTERFACE s_axilite port = break_row8
	#pragma HLS INTERFACE s_axilite port = break_row9
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

	const int break_line1 = break_row2-break_row1;
	const int break_line2 = break_row3-break_row2;
	const int break_line3 = break_row4-break_row3;
	const int break_line4 = break_row5-break_row4;
	const int break_line5 = break_row6-break_row5;
	const int break_line6 = break_row7-break_row6;
	const int break_line7 = break_row8-break_row7;
	const int break_line8 = break_row9-break_row8;
	const int break_line9 = N-break_row9;

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

	read_y(Uplo,Y1_in,Y1_nosym,break_row1);
	read_y(Uplo,Y2_in,Y2_nosym,break_line1);
	read_y(Uplo,Y3_in,Y3_nosym,break_line2);
	read_y(Uplo,Y4_in,Y4_nosym,break_line3);
	read_y(Uplo,Y5_in,Y5_nosym,break_line4);
	read_y(Uplo,Y6_in,Y6_nosym,break_line5);
	read_y(Uplo,Y7_in,Y7_nosym,break_line6);
	read_y(Uplo,Y8_in,Y8_nosym,break_line7);
	read_y(Uplo,Y9_in,Y9_nosym,break_line8);
	read_y(Uplo,Y10_in,Y10_nosym,break_line9);


	read_x(Uplo,X1_nosym,X1_sym, X1, N, 0, break_row1);
	read_x(Uplo,X2_nosym,X2_sym, X2, N, break_row1, break_line1);
	read_x(Uplo,X3_nosym,X3_sym, X3, N, break_row2, break_line2);
	read_x(Uplo,X4_nosym,X4_sym, X4, N, break_row3, break_line3);
	read_x(Uplo,X5_nosym,X5_sym, X5, N, break_row4, break_line4);
	read_x(Uplo,X6_nosym,X6_sym, X6, N, break_row5, break_line5);
	read_x(Uplo,X7_nosym,X7_sym, X7, N, break_row6, break_line6);
	read_x(Uplo,X8_nosym,X8_sym, X8, N, break_row7, break_line7);
	read_x(Uplo,X9_nosym,X9_sym, X9, N, break_row8, break_line8);
	read_x(Uplo,X10_nosym,X10_sym, X10, N, break_row9, break_line9);

	read_matrix(Uplo,A1,A1_nosym,A1_sym,N,0,break_row1);
	read_matrix(Uplo,A2,A2_nosym,A2_sym,N,break_row1,break_line1);
	read_matrix(Uplo,A3,A3_nosym,A3_sym,N,break_row2,break_line2);
	read_matrix(Uplo,A4,A4_nosym,A4_sym,N,break_row3,break_line3);
	read_matrix(Uplo,A5,A5_nosym,A5_sym,N,break_row4,break_line4);
	read_matrix(Uplo,A6,A6_nosym,A6_sym,N,break_row5,break_line5);
	read_matrix(Uplo,A7,A7_nosym,A7_sym,N,break_row6,break_line6);
	read_matrix(Uplo,A8,A8_nosym,A8_sym,N,break_row7,break_line7);
	read_matrix(Uplo,A9,A9_nosym,A9_sym,N,break_row8,break_line8);
	read_matrix(Uplo,A10,A10_nosym,A10_sym,N,break_row9,break_line9);

	scal_nosym(Uplo,A1_nosym, X1_nosym,Y1_noscal,alpha,N,0,break_row1);
	scal_nosym(Uplo,A2_nosym, X2_nosym,Y2_noscal,alpha,N,break_row1,break_line1);
	scal_nosym(Uplo,A3_nosym, X3_nosym,Y3_noscal,alpha,N,break_row2,break_line2);
	scal_nosym(Uplo,A4_nosym, X4_nosym,Y4_noscal,alpha,N,break_row3,break_line3);
	scal_nosym(Uplo,A5_nosym, X5_nosym,Y5_noscal,alpha,N,break_row4,break_line4);
	scal_nosym(Uplo,A6_nosym, X6_nosym,Y6_noscal,alpha,N,break_row5,break_line5);
	scal_nosym(Uplo,A7_nosym, X7_nosym,Y7_noscal,alpha,N,break_row6,break_line6);
	scal_nosym(Uplo,A8_nosym, X8_nosym,Y8_noscal,alpha,N,break_row7,break_line7);
	scal_nosym(Uplo,A9_nosym, X9_nosym,Y9_noscal,alpha,N,break_row8,break_line8);
	scal_nosym(Uplo,A10_nosym, X10_nosym,Y10_noscal,alpha,N,break_row9,break_line9);

	accum_nosym(Uplo,Y1_noscal,Y1_noaccum,N,0,break_row1);
	accum_nosym(Uplo,Y2_noscal,Y2_noaccum,N,break_row1,break_line1);
	accum_nosym(Uplo,Y3_noscal,Y3_noaccum,N,break_row2,break_line2);
	accum_nosym(Uplo,Y4_noscal,Y4_noaccum,N,break_row3,break_line3);
	accum_nosym(Uplo,Y5_noscal,Y5_noaccum,N,break_row4,break_line4);
	accum_nosym(Uplo,Y6_noscal,Y6_noaccum,N,break_row5,break_line5);
	accum_nosym(Uplo,Y7_noscal,Y7_noaccum,N,break_row6,break_line6);
	accum_nosym(Uplo,Y8_noscal,Y8_noaccum,N,break_row7,break_line7);
	accum_nosym(Uplo,Y9_noscal,Y9_noaccum,N,break_row8,break_line8);
	accum_nosym(Uplo,Y10_noscal,Y10_noaccum,N,break_row9,break_line9);

	reduction(Y1_noaccum,Y1_reduct,break_row1);
	reduction(Y2_noaccum,Y2_reduct,break_line1);
	reduction(Y3_noaccum,Y3_reduct,break_line2);
	reduction(Y4_noaccum,Y4_reduct,break_line3);
	reduction(Y5_noaccum,Y5_reduct,break_line4);
	reduction(Y6_noaccum,Y6_reduct,break_line5);
	reduction(Y7_noaccum,Y7_reduct,break_line6);
	reduction(Y8_noaccum,Y8_reduct,break_line7);
	reduction(Y9_noaccum,Y9_reduct,break_line8);
	reduction(Y10_noaccum,Y10_reduct,break_line9);

	scal_sym(Uplo,A1_sym, X1_sym, Y1_scal, alpha, N, 0, break_row1);
	scal_sym(Uplo,A2_sym, X2_sym, Y2_scal, alpha, N, break_row1, break_line1);
	scal_sym(Uplo,A3_sym, X3_sym, Y3_scal, alpha, N, break_row2, break_line2);
	scal_sym(Uplo,A4_sym, X4_sym, Y4_scal, alpha, N, break_row3, break_line3);
	scal_sym(Uplo,A5_sym, X5_sym, Y5_scal, alpha, N, break_row4, break_line4);
	scal_sym(Uplo,A6_sym, X6_sym, Y6_scal, alpha, N, break_row5, break_line5);
	scal_sym(Uplo,A7_sym, X7_sym, Y7_scal, alpha, N, break_row6, break_line6);
	scal_sym(Uplo,A8_sym, X8_sym, Y8_scal, alpha, N, break_row7, break_line7);
	scal_sym(Uplo,A9_sym, X9_sym, Y9_scal, alpha, N, break_row8, break_line8);
	scal_sym(Uplo,A10_sym, X10_sym, Y10_scal, alpha, N, break_row9, break_line9);

	accum_sym(Uplo, Y1_scal, Y1_accum, N, 0, break_row1);
	accum_sym(Uplo, Y2_scal, Y2_accum, N, break_row1, break_line1);
	accum_sym(Uplo, Y3_scal, Y3_accum, N, break_row2, break_line2);
	accum_sym(Uplo, Y4_scal, Y4_accum, N, break_row3, break_line3);
	accum_sym(Uplo, Y5_scal, Y5_accum, N, break_row4, break_line4);
	accum_sym(Uplo, Y6_scal, Y6_accum, N, break_row5, break_line5);
	accum_sym(Uplo, Y7_scal, Y7_accum, N, break_row6, break_line6);
	accum_sym(Uplo, Y8_scal, Y8_accum, N, break_row7, break_line7);
	accum_sym(Uplo, Y9_scal, Y9_accum, N, break_row8, break_line8);
	accum_sym(Uplo, Y10_scal, Y10_accum, N, break_row9, break_line9);

	write_nosym(Y1_nosym,Y1_in,Y1_reduct,beta,break_row1);
	write_nosym(Y2_nosym,Y2_in,Y2_reduct,beta,break_line1);
	write_nosym(Y3_nosym,Y3_in,Y3_reduct,beta,break_line2);
	write_nosym(Y4_nosym,Y4_in,Y4_reduct,beta,break_line3);
	write_nosym(Y5_nosym,Y5_in,Y5_reduct,beta,break_line4);
	write_nosym(Y6_nosym,Y6_in,Y6_reduct,beta,break_line5);
	write_nosym(Y7_nosym,Y7_in,Y7_reduct,beta,break_line6);
	write_nosym(Y8_nosym,Y8_in,Y8_reduct,beta,break_line7);
	write_nosym(Y9_nosym,Y9_in,Y9_reduct,beta,break_line8);
	write_nosym(Y10_nosym,Y10_in,Y10_reduct,beta,break_line9);

	write_sym(Y1_sym, Y1_accum, N, 0);
	write_sym(Y2_sym, Y2_accum, N, break_row1);
	write_sym(Y3_sym, Y3_accum, N, break_row2);
	write_sym(Y4_sym, Y4_accum, N, break_row3);
	write_sym(Y5_sym, Y5_accum, N, break_row4);
	write_sym(Y6_sym, Y6_accum, N, break_row5);
	write_sym(Y7_sym, Y7_accum, N, break_row6);
	write_sym(Y8_sym, Y8_accum, N, break_row7);
	write_sym(Y9_sym, Y9_accum, N, break_row8);
	write_sym(Y10_sym, Y10_accum, N, break_row9);

}}

