#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
#define CUs 14

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void wide_read_X(hls::stream<v_dt>& Xout, v_dt *X,const int N,const int K){

	int i=0,j=0;

	int N_cols = K/16 + 1;

	x_read:
	while(i<N){
		#pragma HLS pipeline II=1

		Xout << X[i*N_cols+j];

		int cond = K/VDATA_SIZE;
		j== cond ? i++ : i+=0;
		j== cond ? j=0 : j++;

	}
}

void wide_read_A(hls::stream<v_dt>& Aout,v_dt *A,const int N,const int K){

	int i=0,j=0;

	int N_cols = K/16 + 1;

	A_read:
	while(i<N){
		#pragma HLS pipeline II=1

		Aout << A[i*N_cols+j];

		int cond = K/VDATA_SIZE;
		j== cond ? i++ : i+=0;
		j== cond ? j=0 : j++;
	}
}

void scal_ax(hls::stream<v_dt>&Ain, hls::stream<v_dt>&Xin,hls::stream<v_dt>&Yout, const int N,const int K){

	int i=0,j=0;

	scal:
	while(i<N){
		#pragma HLS pipeline II=1
		v_dt Xtemp=Xin.read();
		v_dt Atemp=Ain.read();
		v_dt out;


		for (int k=0;k<VDATA_SIZE;k++){
			#pragma HLS UNROLL
			
			if (k<hls::fmin(VDATA_SIZE,K+1-j*VDATA_SIZE)){
				out.data[k] = Xtemp.data[k]*Atemp.data[k];
			}
			else {
				out.data[k]=0;
			}

		}

		Yout <<out;

		int cond = K/VDATA_SIZE;
		j== cond ? i++ : i+=0;
		j== cond ? j=0 : j++;
	}
}

void accum(hls::stream<v_dt>& AXscal,hls::stream<v_dt>& AXaccum, const int N, const int K){

	non_pipelined:
	for(int i=0;i<N;i++){
		v_dt Y_temp;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			Y_temp.data[k]=0.0f;
		}


		pipelined:
		for(int j=0;j<K+1;j+=VDATA_SIZE){
			#pragma hls pipeline II=1
			v_dt temp = AXscal.read();

			for(int k=0;k<VDATA_SIZE;k++){
				#pragma HLS unroll
				Y_temp.data[k] +=temp.data[k];
			}

		}
		AXaccum << Y_temp;
	}

}

void reduction(hls::stream<v_dt>& AXaccum,hls::stream<float>& AXreduc,int N){

	reduction:
	for(int i=0;i<N;i++){
		#pragma hls pipeline II=1

		v_dt temp = AXaccum.read();
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
		AXreduc << tmp15;

	}

}

void write_x(hls::stream<float>& Xstream,float *X,int N){

	write_y:
	for(int i=0;i<N;i++){
		#pragma HLS pipeline II=1
		X[i] = Xstream.read();
	}

}

//layout is for specific FPGA to check available sizes
extern "C" {
void krnl_tbmv(const char Uplo,const char TransA,const char Diag, const int N, const int K, v_dt *A1, v_dt *A2, v_dt *A3, v_dt *A4, v_dt *A5, 
		v_dt *A6, v_dt *A7, v_dt *A8, v_dt *A9, v_dt *A10, v_dt *A11, v_dt *A12, v_dt *A13, v_dt *A14, const int lda, v_dt  *X1_in, 
		v_dt  *X2_in, v_dt *X3_in, v_dt *X4_in, v_dt *X5_in, v_dt *X6_in, v_dt *X7_in, v_dt *X8_in, v_dt *X9_in, v_dt *X10_in, v_dt *X11_in, 
		v_dt *X12_in, v_dt *X13_in, v_dt *X14_in, const int incX, float *X1_out, float *X2_out, float *X3_out, float *X4_out, float *X5_out, 
		float *X6_out, float *X7_out, float *X8_out, float *X9_out, float *X10_out, float *X11_out, float *X12_out, float *X13_out, float *X14_out){

	#pragma HLS INTERFACE m_axi port = X1_in offset = slave bundle = ddr0
	#pragma HLS INTERFACE m_axi port = X2_in offset = slave bundle = ddr1
	#pragma HLS INTERFACE m_axi port = X3_in offset = slave bundle = ddr2
	#pragma HLS INTERFACE m_axi port = X4_in offset = slave bundle = ddr3
	#pragma HLS INTERFACE m_axi port = X5_in offset = slave bundle = ddr4
	#pragma HLS INTERFACE m_axi port = X6_in offset = slave bundle = ddr5
	#pragma HLS INTERFACE m_axi port = X7_in offset = slave bundle = ddr6
	#pragma HLS INTERFACE m_axi port = X8_in offset = slave bundle = ddr7
	#pragma HLS INTERFACE m_axi port = X9_in offset = slave bundle = ddr8
	#pragma HLS INTERFACE m_axi port = X10_in offset = slave bundle = ddr9
	#pragma HLS INTERFACE m_axi port = X11_in offset = slave bundle = ddr10
	#pragma HLS INTERFACE m_axi port = X12_in offset = slave bundle = ddr11
	#pragma HLS INTERFACE m_axi port = X13_in offset = slave bundle = ddr12
	#pragma HLS INTERFACE m_axi port = X14_in offset = slave bundle = ddr13	
	#pragma HLS INTERFACE m_axi port = X1_out offset = slave bundle = ddr0
	#pragma HLS INTERFACE m_axi port = X2_out offset = slave bundle = ddr1
	#pragma HLS INTERFACE m_axi port = X3_out offset = slave bundle = ddr2
	#pragma HLS INTERFACE m_axi port = X4_out offset = slave bundle = ddr3
	#pragma HLS INTERFACE m_axi port = X5_out offset = slave bundle = ddr4
	#pragma HLS INTERFACE m_axi port = X6_out offset = slave bundle = ddr5
	#pragma HLS INTERFACE m_axi port = X7_out offset = slave bundle = ddr6
	#pragma HLS INTERFACE m_axi port = X8_out offset = slave bundle = ddr7
	#pragma HLS INTERFACE m_axi port = X9_out offset = slave bundle = ddr8
	#pragma HLS INTERFACE m_axi port = X10_out offset = slave bundle = ddr9
	#pragma HLS INTERFACE m_axi port = X11_out offset = slave bundle = ddr10
	#pragma HLS INTERFACE m_axi port = X12_out offset = slave bundle = ddr11
	#pragma HLS INTERFACE m_axi port = X13_out offset = slave bundle = ddr12
	#pragma HLS INTERFACE m_axi port = X14_out offset = slave bundle = ddr13	
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = ddr14
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = ddr15
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = ddr16
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = ddr17
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = ddr18
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = ddr19
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = ddr20
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = ddr21
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = ddr22
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = ddr23
	#pragma HLS INTERFACE m_axi port = A11 offset = slave bundle = ddr24
	#pragma HLS INTERFACE m_axi port = A12 offset = slave bundle = ddr25
	#pragma HLS INTERFACE m_axi port = A13 offset = slave bundle = ddr26
	#pragma HLS INTERFACE m_axi port = A14 offset = slave bundle = ddr27

	#pragma HLS INTERFACE s_axilite port = Uplo
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = Diag
	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = K
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = incX

	static hls::stream<v_dt> X_in1("X_in1");
	static hls::stream<v_dt> X_in2("X_in2");
	static hls::stream<v_dt> X_in3("X_in3");
	static hls::stream<v_dt> X_in4("X_in4");
	static hls::stream<v_dt> X_in5("X_in5");
	static hls::stream<v_dt> X_in6("X_in6");
	static hls::stream<v_dt> X_in7("X_in7");
	static hls::stream<v_dt> X_in8("X_in8");
	static hls::stream<v_dt> X_in9("X_in9");
	static hls::stream<v_dt> X_in10("X_in10");
	static hls::stream<v_dt> X_in11("X_in11");
	static hls::stream<v_dt> X_in12("X_in12");
	static hls::stream<v_dt> X_in13("X_in13");
	static hls::stream<v_dt> X_in14("X_in14");

	static hls::stream<v_dt> A_in1("A_in1");
	static hls::stream<v_dt> A_in2("A_in2");
	static hls::stream<v_dt> A_in3("A_in3");
	static hls::stream<v_dt> A_in4("A_in4");
	static hls::stream<v_dt> A_in5("A_in5");
	static hls::stream<v_dt> A_in6("A_in6");
	static hls::stream<v_dt> A_in7("A_in7");
	static hls::stream<v_dt> A_in8("A_in8");
	static hls::stream<v_dt> A_in9("A_in9");
	static hls::stream<v_dt> A_in10("A_in10");
	static hls::stream<v_dt> A_in11("A_in11");
	static hls::stream<v_dt> A_in12("A_in12");
	static hls::stream<v_dt> A_in13("A_in13");
	static hls::stream<v_dt> A_in14("A_in14");

	static hls::stream<v_dt> AXscal1("AXscal1");
	static hls::stream<v_dt> AXscal2("AXscal2");
	static hls::stream<v_dt> AXscal3("AXscal3");
	static hls::stream<v_dt> AXscal4("AXscal4");
	static hls::stream<v_dt> AXscal5("AXscal5");
	static hls::stream<v_dt> AXscal6("AXscal6");
	static hls::stream<v_dt> AXscal7("AXscal7");
	static hls::stream<v_dt> AXscal8("AXscal8");
	static hls::stream<v_dt> AXscal9("AXscal9");
	static hls::stream<v_dt> AXscal10("AXscal10");
	static hls::stream<v_dt> AXscal11("AXscal11");
	static hls::stream<v_dt> AXscal12("AXscal12");
	static hls::stream<v_dt> AXscal13("AXscal13");
	static hls::stream<v_dt> AXscal14("AXscal14");

	static hls::stream<v_dt> AXaccum1("AXaccum1");
	static hls::stream<v_dt> AXaccum2("AXaccum2");
	static hls::stream<v_dt> AXaccum3("AXaccum3");
	static hls::stream<v_dt> AXaccum4("AXaccum4");
	static hls::stream<v_dt> AXaccum5("AXaccum5");
	static hls::stream<v_dt> AXaccum6("AXaccum6");
	static hls::stream<v_dt> AXaccum7("AXaccum7");
	static hls::stream<v_dt> AXaccum8("AXaccum8");
	static hls::stream<v_dt> AXaccum9("AXaccum9");
	static hls::stream<v_dt> AXaccum10("AXaccum10");
	static hls::stream<v_dt> AXaccum11("AXaccum11");
	static hls::stream<v_dt> AXaccum12("AXaccum12");
	static hls::stream<v_dt> AXaccum13("AXaccum13");
	static hls::stream<v_dt> AXaccum14("AXaccum14");

	static hls::stream<float> X_out1("X_out1");
	static hls::stream<float> X_out2("X_out2");
	static hls::stream<float> X_out3("X_out3");
	static hls::stream<float> X_out4("X_out4");
	static hls::stream<float> X_out5("X_out5");
	static hls::stream<float> X_out6("X_out6");
	static hls::stream<float> X_out7("X_out7");
	static hls::stream<float> X_out8("X_out8");
	static hls::stream<float> X_out9("X_out9");
	static hls::stream<float> X_out10("X_out10");
	static hls::stream<float> X_out11("X_out11");
	static hls::stream<float> X_out12("X_out12");
	static hls::stream<float> X_out13("X_out13");
	static hls::stream<float> X_out14("X_out14");

	#pragma HLS STREAM variable = X_in1 depth = 2048
	#pragma HLS STREAM variable = A_in1 depth = 2048
	#pragma HLS STREAM variable = AXscal1 depth = 2048
	#pragma HLS STREAM variable = AXaccum1 depth = 2048
	#pragma HLS STREAM variable = X_out1 depth = 2048

	#pragma HLS STREAM variable = X_in2 depth = 2048
	#pragma HLS STREAM variable = A_in2 depth = 2048
	#pragma HLS STREAM variable = AXscal2 depth = 2048
	#pragma HLS STREAM variable = AXaccum2 depth = 2048
	#pragma HLS STREAM variable = X_out2 depth = 2048

	#pragma HLS STREAM variable = X_in3 depth = 2048
	#pragma HLS STREAM variable = A_in3 depth = 2048
	#pragma HLS STREAM variable = AXscal3 depth = 2048
	#pragma HLS STREAM variable = AXaccum3 depth = 2048
	#pragma HLS STREAM variable = X_out3 depth = 2048

	#pragma HLS STREAM variable = X_in4 depth = 2048
	#pragma HLS STREAM variable = A_in4 depth = 2048
	#pragma HLS STREAM variable = AXscal4 depth = 2048
	#pragma HLS STREAM variable = AXaccum4 depth = 2048
	#pragma HLS STREAM variable = X_out4 depth = 2048

	#pragma HLS STREAM variable = X_in5 depth = 2048
    #pragma HLS STREAM variable = A_in5 depth = 2048
    #pragma HLS STREAM variable = AXscal5 depth = 2048
    #pragma HLS STREAM variable = AXaccum5 depth = 2048
    #pragma HLS STREAM variable = X_out5 depth = 2048

	#pragma HLS STREAM variable = X_in6 depth = 2048
    #pragma HLS STREAM variable = A_in6 depth = 2048
    #pragma HLS STREAM variable = AXscal6 depth = 2048
    #pragma HLS STREAM variable = AXaccum6 depth = 2048
    #pragma HLS STREAM variable = X_out6 depth = 2048

	#pragma HLS STREAM variable = X_in7 depth = 2048
    #pragma HLS STREAM variable = A_in7 depth = 2048
    #pragma HLS STREAM variable = AXscal7 depth = 2048
    #pragma HLS STREAM variable = AXaccum7 depth = 2048
    #pragma HLS STREAM variable = X_out7 depth = 2048

	#pragma HLS STREAM variable = X_in8 depth = 2048
    #pragma HLS STREAM variable = A_in8 depth = 2048
    #pragma HLS STREAM variable = AXscal8 depth = 2048
    #pragma HLS STREAM variable = AXaccum8 depth = 2048
    #pragma HLS STREAM variable = X_out8 depth = 2048

	#pragma HLS STREAM variable = X_in9 depth = 2048
    #pragma HLS STREAM variable = A_in9 depth = 2048
    #pragma HLS STREAM variable = AXscal9 depth = 2048
    #pragma HLS STREAM variable = AXaccum9 depth = 2048
    #pragma HLS STREAM variable = X_out9 depth = 2048

	#pragma HLS STREAM variable = X_in10 depth = 2048
    #pragma HLS STREAM variable = A_in10 depth = 2048
    #pragma HLS STREAM variable = AXscal10 depth = 2048
    #pragma HLS STREAM variable = AXaccum10 depth = 2048
    #pragma HLS STREAM variable = X_out10 depth = 2048

	#pragma HLS STREAM variable = X_in11 depth = 2048
    #pragma HLS STREAM variable = A_in11 depth = 2048
    #pragma HLS STREAM variable = AXscal11 depth = 2048
    #pragma HLS STREAM variable = AXaccum11 depth = 2048
    #pragma HLS STREAM variable = X_out11 depth = 2048

	#pragma HLS STREAM variable = X_in12 depth = 2048
    #pragma HLS STREAM variable = A_in12 depth = 2048
    #pragma HLS STREAM variable = AXscal12 depth = 2048
    #pragma HLS STREAM variable = AXaccum12 depth = 2048
    #pragma HLS STREAM variable = X_out12 depth = 2048

	#pragma HLS STREAM variable = X_in13 depth = 2048
    #pragma HLS STREAM variable = A_in13 depth = 2048
    #pragma HLS STREAM variable = AXscal13 depth = 2048
    #pragma HLS STREAM variable = AXaccum13 depth = 2048
    #pragma HLS STREAM variable = X_out13 depth = 2048

	#pragma HLS STREAM variable = X_in14 depth = 2048
    #pragma HLS STREAM variable = A_in14 depth = 2048
    #pragma HLS STREAM variable = AXscal14 depth = 2048
    #pragma HLS STREAM variable = AXaccum14 depth = 2048
    #pragma HLS STREAM variable = X_out14 depth = 2048

	#pragma HLS dataflow

	wide_read_X(X_in1,X1_in,N/CUs,K);
	wide_read_X(X_in2,X2_in,N/CUs,K);
	wide_read_X(X_in3,X3_in,N/CUs,K);
	wide_read_X(X_in4,X4_in,N/CUs,K);
	wide_read_X(X_in5,X5_in,N/CUs,K);
	wide_read_X(X_in6,X6_in,N/CUs,K);
	wide_read_X(X_in7,X7_in,N/CUs,K);
	wide_read_X(X_in8,X8_in,N/CUs,K);
	wide_read_X(X_in9,X9_in,N/CUs,K);
	wide_read_X(X_in10,X10_in,N/CUs,K);
	wide_read_X(X_in11,X11_in,N/CUs,K);
	wide_read_X(X_in12,X12_in,N/CUs,K);
	wide_read_X(X_in13,X13_in,N/CUs,K);
	wide_read_X(X_in14,X14_in,N/CUs,K);

	wide_read_A(A_in1,A1,N/CUs,K);
	wide_read_A(A_in2,A2,N/CUs,K);
	wide_read_A(A_in3,A3,N/CUs,K);
	wide_read_A(A_in4,A4,N/CUs,K);
	wide_read_A(A_in5,A5,N/CUs,K);
	wide_read_A(A_in6,A6,N/CUs,K);
	wide_read_A(A_in7,A7,N/CUs,K);
	wide_read_A(A_in8,A8,N/CUs,K);
	wide_read_A(A_in9,A9,N/CUs,K);
	wide_read_A(A_in10,A10,N/CUs,K);
	wide_read_A(A_in11,A11,N/CUs,K);
	wide_read_A(A_in12,A12,N/CUs,K);
	wide_read_A(A_in13,A13,N/CUs,K);
	wide_read_A(A_in14,A14,N/CUs,K);

	scal_ax(A_in1,X_in1,AXscal1,N/CUs,K);
	scal_ax(A_in2,X_in2,AXscal2,N/CUs,K);
	scal_ax(A_in3,X_in3,AXscal3,N/CUs,K);
	scal_ax(A_in4,X_in4,AXscal4,N/CUs,K);
	scal_ax(A_in5,X_in5,AXscal5,N/CUs,K);
	scal_ax(A_in6,X_in6,AXscal6,N/CUs,K);
	scal_ax(A_in7,X_in7,AXscal7,N/CUs,K);
	scal_ax(A_in8,X_in8,AXscal8,N/CUs,K);
	scal_ax(A_in9,X_in9,AXscal9,N/CUs,K);
	scal_ax(A_in10,X_in10,AXscal10,N/CUs,K);
	scal_ax(A_in11,X_in11,AXscal11,N/CUs,K);
	scal_ax(A_in12,X_in12,AXscal12,N/CUs,K);
	scal_ax(A_in13,X_in13,AXscal13,N/CUs,K);
	scal_ax(A_in14,X_in14,AXscal14,N/CUs,K);

	accum(AXscal1,AXaccum1,N/CUs,K);
	accum(AXscal2,AXaccum2,N/CUs,K);
	accum(AXscal3,AXaccum3,N/CUs,K);
	accum(AXscal4,AXaccum4,N/CUs,K);
	accum(AXscal5,AXaccum5,N/CUs,K);
	accum(AXscal6,AXaccum6,N/CUs,K);
	accum(AXscal7,AXaccum7,N/CUs,K);
	accum(AXscal8,AXaccum8,N/CUs,K);
	accum(AXscal9,AXaccum9,N/CUs,K);
	accum(AXscal10,AXaccum10,N/CUs,K);
	accum(AXscal11,AXaccum11,N/CUs,K);
	accum(AXscal12,AXaccum12,N/CUs,K);
	accum(AXscal13,AXaccum13,N/CUs,K);
	accum(AXscal14,AXaccum14,N/CUs,K);

	reduction(AXaccum1,X_out1,N/CUs);
	reduction(AXaccum2,X_out2,N/CUs);
	reduction(AXaccum3,X_out3,N/CUs);
	reduction(AXaccum4,X_out4,N/CUs);
	reduction(AXaccum5,X_out5,N/CUs);
	reduction(AXaccum6,X_out6,N/CUs);
	reduction(AXaccum7,X_out7,N/CUs);
	reduction(AXaccum8,X_out8,N/CUs);
	reduction(AXaccum9,X_out9,N/CUs);
	reduction(AXaccum10,X_out10,N/CUs);
	reduction(AXaccum11,X_out11,N/CUs);
	reduction(AXaccum12,X_out12,N/CUs);
	reduction(AXaccum13,X_out13,N/CUs);
	reduction(AXaccum14,X_out14,N/CUs);

	write_x(X_out1,X1_out,N/CUs);
	write_x(X_out2,X2_out,N/CUs);
	write_x(X_out3,X3_out,N/CUs);
	write_x(X_out4,X4_out,N/CUs);
	write_x(X_out5,X5_out,N/CUs);
	write_x(X_out6,X6_out,N/CUs);
	write_x(X_out7,X7_out,N/CUs);
	write_x(X_out8,X8_out,N/CUs);
	write_x(X_out9,X9_out,N/CUs);
	write_x(X_out10,X10_out,N/CUs);
	write_x(X_out11,X11_out,N/CUs);
	write_x(X_out12,X12_out,N/CUs);
	write_x(X_out13,X13_out,N/CUs);
	write_x(X_out14,X14_out,N/CUs);

}}

