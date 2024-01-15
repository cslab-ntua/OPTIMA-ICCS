#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
#define CUs 10

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void wide_read_X(hls::stream<v_dt>& Xout, v_dt *X,const int M,const int N,const int KU,const int KL){

	int i=0,j=0;

	int N_cols = ((KU+KL)/16 + 1);

	x_read:
	while(i<M){
		#pragma HLS pipeline II=1

		Xout << X[i*N_cols+j];

		int cond = (KU+KL)/VDATA_SIZE;
		j== (KU+KL)/VDATA_SIZE ? i++ : i+=0;
		j== (KU+KL)/VDATA_SIZE ? j=0 : j++;
	}
}

void wide_read_A(hls::stream<v_dt>& Aout,v_dt *A,const int N,const int M,const int KU,const int KL){

	int i=0,j=0;

	int N_cols = ((KU+KL)/16 + 1);

	A_read:
	while(i<M){
		#pragma HLS pipeline II=1

		Aout << A[i*N_cols+j];

		int cond = (KU+KL)/VDATA_SIZE;
		j== (KU+KL)/VDATA_SIZE ? i++ : i+=0;
		j== (KU+KL)/VDATA_SIZE ? j=0 : j++;
	}
}

void scal_ax(hls::stream<v_dt>&Ain, hls::stream<v_dt>&Xin,hls::stream<v_dt>&Yout, const float alpha,const int N,
		const int M,const int KU,const int KL, const int scal_factor){

	int i=0,j=0;

	scal:
	while(i<M){
		#pragma HLS pipeline II=1
		v_dt Xtemp=Xin.read();
		v_dt Atemp=Ain.read();
		v_dt out;


		for (int k=0;k<VDATA_SIZE;k++){
			#pragma HLS UNROLL
			int upper, lower;

			(M*scal_factor-i) <= KU ? upper=KU+1-(M*scal_factor-i) : upper=0 ;

			KL>=(CUs-M)*scal_factor ? lower= KL-(CUs-M)*scal_factor : lower=KL;

			if (k>=hls::fmax(0,KL-lower-i-j*VDATA_SIZE) and k<hls::fmin(VDATA_SIZE, KU+KL+1-j*VDATA_SIZE-upper)){
				out.data[k] = alpha*Xtemp.data[k]*Atemp.data[k];
			}
			else {
				out.data[k]=0;
			}
		}
		Yout <<out;

		int cond = (KU+KL)/VDATA_SIZE;
		j== (KU+KL)/VDATA_SIZE ? i++ : i+=0;
		j== (KU+KL)/VDATA_SIZE ? j=0 : j++;
	}
}

void accum(hls::stream<v_dt>& AXscal,hls::stream<v_dt>& AXaccum, const int M, const int N, const int KU,const int KL){

	non_pipelined:
	for(int i=0;i<M;i++){
		v_dt Y_temp;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			Y_temp.data[k]=0.0f;
		}


		pipelined:
		for(int j=0;j<KU+KL+1;j+=VDATA_SIZE){
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

void reduction(hls::stream<v_dt>& AXaccum,hls::stream<float>& AXreduc,int M){

	reduction:
	for(int i=0;i<M;i++){
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

void read_y(hls::stream<float>& Ystream,float *Y,int M){

	read_y:
	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Ystream <<Y[i];
	}

}

void scal_y(hls::stream<float>& Y_in,hls::stream<float>& Y_out,int M,float beta){

	scal_y:
	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		float Y_temp = Y_in.read();
		Y_out << Y_temp*beta;

	}
}

void write_y(hls::stream<float>& Y_in1,hls::stream<float>& Y_in2,float *Y,int M){

	write_y:
	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Y[i] = Y_in1.read() + Y_in2.read();
	}

}

//layout is for specific FPGA to check available sizes
extern "C" {
void krnl_gbmv(const char TransA, const int M, const int N, const int KL, const int KU, const float alpha, v_dt *A1, v_dt *A2, v_dt *A3, v_dt *A4,
		v_dt *A5, v_dt *A6, v_dt *A7, v_dt *A8, v_dt *A9, v_dt *A10, const int lda, v_dt  *X1, v_dt  *X2, v_dt  *X3, v_dt  *X4, v_dt  *X5, 
		v_dt  *X6, v_dt  *X7, v_dt  *X8, v_dt  *X9, v_dt  *X10, const int incX, const float beta, float  *Y1, float  *Y2, float  *Y3, 
		float  *Y4, float  *Y5, float  *Y6, float  *Y7, float  *Y8, float  *Y9, float  *Y10, const int incY) {

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
	#pragma HLS INTERFACE m_axi port = Y1 offset = slave bundle = ddr10
	#pragma HLS INTERFACE m_axi port = Y2 offset = slave bundle = ddr11
	#pragma HLS INTERFACE m_axi port = Y3 offset = slave bundle = ddr12
	#pragma HLS INTERFACE m_axi port = Y4 offset = slave bundle = ddr13
	#pragma HLS INTERFACE m_axi port = Y5 offset = slave bundle = ddr14
	#pragma HLS INTERFACE m_axi port = Y6 offset = slave bundle = ddr15
	#pragma HLS INTERFACE m_axi port = Y7 offset = slave bundle = ddr16
	#pragma HLS INTERFACE m_axi port = Y8 offset = slave bundle = ddr17
	#pragma HLS INTERFACE m_axi port = Y9 offset = slave bundle = ddr18
	#pragma HLS INTERFACE m_axi port = Y10 offset = slave bundle = ddr19
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = ddr20
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = ddr21
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = ddr22
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = ddr23
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = ddr24
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = ddr25
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = ddr26
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = ddr27
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = ddr28
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = ddr29

	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = M
	#pragma HLS INTERFACE s_axilite port = KL
	#pragma HLS INTERFACE s_axilite port = KU
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = incX
	#pragma HLS INTERFACE s_axilite port = incY
	#pragma HLS INTERFACE s_axilite port = alpha
	#pragma HLS INTERFACE s_axilite port = beta

	static hls::stream<float> Y_in1("Yin1");
	static hls::stream<float> Y_in2("Yin2");
	static hls::stream<float> Y_in3("Yin3");
	static hls::stream<float> Y_in4("Yin4");
	static hls::stream<float> Y_in5("Yin5");
	static hls::stream<float> Y_in6("Yin6");
	static hls::stream<float> Y_in7("Yin7");
	static hls::stream<float> Y_in8("Yin8");
	static hls::stream<float> Y_in9("Yin9");
	static hls::stream<float> Y_in10("Yin10");

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

	static hls::stream<float> Y_scal1("Yscal1");
	static hls::stream<float> Y_scal2("Yscal2");
	static hls::stream<float> Y_scal3("Yscal3");
	static hls::stream<float> Y_scal4("Yscal4");
	static hls::stream<float> Y_scal5("Yscal5");
	static hls::stream<float> Y_scal6("Yscal6");
	static hls::stream<float> Y_scal7("Yscal7");
	static hls::stream<float> Y_scal8("Yscal8");
	static hls::stream<float> Y_scal9("Yscal9");
	static hls::stream<float> Y_scal10("Yscal10");
	
	static hls::stream<float> AXreduc1("AXreduc1");
	static hls::stream<float> AXreduc2("AXreduc2");
	static hls::stream<float> AXreduc3("AXreduc3");
	static hls::stream<float> AXreduc4("AXreduc4");
	static hls::stream<float> AXreduc5("AXreduc5");
	static hls::stream<float> AXreduc6("AXreduc6");
	static hls::stream<float> AXreduc7("AXreduc7");
	static hls::stream<float> AXreduc8("AXreduc8");
	static hls::stream<float> AXreduc9("AXreduc9");
	static hls::stream<float> AXreduc10("AXreduc10");

	#pragma HLS STREAM variable = Y_in1 depth = 2048
	#pragma HLS STREAM variable = X1 depth = 2048
	#pragma HLS STREAM variable = A1 depth = 2048
	#pragma HLS STREAM variable = AXscal1 depth = 2048
	#pragma HLS STREAM variable = AXaccum1 depth = 2048
	#pragma HLS STREAM variable = Y_scal1 depth = 2048
	#pragma HLS STREAM variable = AXreduc1 depth = 2048

	#pragma HLS STREAM variable = Y_in2 depth = 2048
	#pragma HLS STREAM variable = X_in2 depth = 2048
	#pragma HLS STREAM variable = A_in2 depth = 2048
	#pragma HLS STREAM variable = AXscal2 depth = 2048
	#pragma HLS STREAM variable = AXaccum2 depth = 2048
	#pragma HLS STREAM variable = Y_scal2 depth = 2048
	#pragma HLS STREAM variable = AXreduc2 depth = 2048

	#pragma HLS STREAM variable = Y_in3 depth = 2048
	#pragma HLS STREAM variable = X_in3 depth = 2048
	#pragma HLS STREAM variable = A_in3 depth = 2048
	#pragma HLS STREAM variable = AXscal3 depth = 2048
	#pragma HLS STREAM variable = AXaccum3 depth = 2048
	#pragma HLS STREAM variable = Y_scal3 depth = 2048
	#pragma HLS STREAM variable = AXreduc3 depth = 2048

	#pragma HLS STREAM variable = Y_in4 depth = 2048
	#pragma HLS STREAM variable = X_in4 depth = 2048
	#pragma HLS STREAM variable = A_in4 depth = 2048
	#pragma HLS STREAM variable = AXscal4 depth = 2048
	#pragma HLS STREAM variable = AXaccum4 depth = 2048
	#pragma HLS STREAM variable = Y_scal4 depth = 2048
	#pragma HLS STREAM variable = AXreduc4 depth = 2048

	#pragma HLS STREAM variable = Y_in5 depth = 2048
        #pragma HLS STREAM variable = X_in5 depth = 2048
        #pragma HLS STREAM variable = A_in5 depth = 2048
        #pragma HLS STREAM variable = AXscal5 depth = 2048
        #pragma HLS STREAM variable = AXaccum5 depth = 2048
        #pragma HLS STREAM variable = Y_scal5 depth = 2048
        #pragma HLS STREAM variable = AXreduc5 depth = 2048

	#pragma HLS STREAM variable = Y_in6 depth = 2048
        #pragma HLS STREAM variable = X_in6 depth = 2048
        #pragma HLS STREAM variable = A_in6 depth = 2048
        #pragma HLS STREAM variable = AXscal6 depth = 2048
        #pragma HLS STREAM variable = AXaccum6 depth = 2048
        #pragma HLS STREAM variable = Y_scal6 depth = 2048
        #pragma HLS STREAM variable = AXreduc6 depth = 2048

	#pragma HLS STREAM variable = Y_in7 depth = 2048
        #pragma HLS STREAM variable = X_in7 depth = 2048
        #pragma HLS STREAM variable = A_in7 depth = 2048
        #pragma HLS STREAM variable = AXscal7 depth = 2048
        #pragma HLS STREAM variable = AXaccum7 depth = 2048
        #pragma HLS STREAM variable = Y_scal7 depth = 2048
        #pragma HLS STREAM variable = AXreduc7 depth = 2048

	#pragma HLS STREAM variable = Y_in8 depth = 2048
        #pragma HLS STREAM variable = X_in8 depth = 2048
        #pragma HLS STREAM variable = A_in8 depth = 2048
        #pragma HLS STREAM variable = AXscal8 depth = 2048
        #pragma HLS STREAM variable = AXaccum8 depth = 2048
        #pragma HLS STREAM variable = Y_scal8 depth = 2048
        #pragma HLS STREAM variable = AXreduc8 depth = 2048

	#pragma HLS STREAM variable = Y_in9 depth = 2048
        #pragma HLS STREAM variable = X_in9 depth = 2048
        #pragma HLS STREAM variable = A_in9 depth = 2048
        #pragma HLS STREAM variable = AXscal9 depth = 2048
        #pragma HLS STREAM variable = AXaccum9 depth = 2048
        #pragma HLS STREAM variable = Y_scal9 depth = 2048
        #pragma HLS STREAM variable = AXreduc9 depth = 2048

	#pragma HLS STREAM variable = Y_in10 depth = 2048
        #pragma HLS STREAM variable = X_in10 depth = 2048
        #pragma HLS STREAM variable = A_in10 depth = 2048
        #pragma HLS STREAM variable = AXscal10 depth = 2048
        #pragma HLS STREAM variable = AXaccum10 depth = 2048
        #pragma HLS STREAM variable = Y_scal10 depth = 2048
        #pragma HLS STREAM variable = AXreduc10 depth = 2048

	#pragma HLS dataflow
	read_y(Y_in1,Y1,M/CUs);
	read_y(Y_in2,Y2,M/CUs);
	read_y(Y_in3,Y3,M/CUs);
	read_y(Y_in4,Y4,M/CUs);
	read_y(Y_in5,Y5,M/CUs);
	read_y(Y_in6,Y6,M/CUs);
	read_y(Y_in7,Y7,M/CUs);
	read_y(Y_in8,Y8,M/CUs);
	read_y(Y_in9,Y9,M/CUs);
	read_y(Y_in10,Y10,M/CUs);

	wide_read_X(X_in1,X1,M/CUs,N,KU,KL);
	wide_read_X(X_in2,X2,M/CUs,N,KU,KL);
	wide_read_X(X_in3,X3,M/CUs,N,KU,KL);
	wide_read_X(X_in4,X4,M/CUs,N,KU,KL);
	wide_read_X(X_in5,X5,M/CUs,N,KU,KL);
	wide_read_X(X_in6,X6,M/CUs,N,KU,KL);
	wide_read_X(X_in7,X7,M/CUs,N,KU,KL);
	wide_read_X(X_in8,X8,M/CUs,N,KU,KL);
	wide_read_X(X_in9,X9,M/CUs,N,KU,KL);
	wide_read_X(X_in10,X10,M/CUs,N,KU,KL);

	wide_read_A(A_in1,A1,N,M/CUs,KU,KL);
	wide_read_A(A_in2,A2,N,M/CUs,KU,KL);
	wide_read_A(A_in3,A3,N,M/CUs,KU,KL);
	wide_read_A(A_in4,A4,N,M/CUs,KU,KL);
	wide_read_A(A_in5,A5,N,M/CUs,KU,KL);
	wide_read_A(A_in6,A6,N,M/CUs,KU,KL);
	wide_read_A(A_in7,A7,N,M/CUs,KU,KL);
	wide_read_A(A_in8,A8,N,M/CUs,KU,KL);
	wide_read_A(A_in9,A9,N,M/CUs,KU,KL);
	wide_read_A(A_in10,A10,N,M/CUs,KU,KL);

	scal_ax(A_in1,X_in1,AXscal1,alpha,N,M/CUs,KU,KL,10);
	scal_ax(A_in2,X_in2,AXscal2,alpha,N,M/CUs,KU,KL,9);
	scal_ax(A_in3,X_in3,AXscal3,alpha,N,M/CUs,KU,KL,8);
	scal_ax(A_in4,X_in4,AXscal4,alpha,N,M/CUs,KU,KL,7);
	scal_ax(A_in5,X_in5,AXscal5,alpha,N,M/CUs,KU,KL,6);
	scal_ax(A_in6,X_in6,AXscal6,alpha,N,M/CUs,KU,KL,5);
	scal_ax(A_in7,X_in7,AXscal7,alpha,N,M/CUs,KU,KL,4);
	scal_ax(A_in8,X_in8,AXscal8,alpha,N,M/CUs,KU,KL,3);
	scal_ax(A_in9,X_in9,AXscal9,alpha,N,M/CUs,KU,KL,2);
	scal_ax(A_in10,X_in10,AXscal10,alpha,N,M/CUs,KU,KL,1);

	accum(AXscal1,AXaccum1,M/CUs,N,KU,KL);
	accum(AXscal2,AXaccum2,M/CUs,N,KU,KL);
	accum(AXscal3,AXaccum3,M/CUs,N,KU,KL);
	accum(AXscal4,AXaccum4,M/CUs,N,KU,KL);
	accum(AXscal5,AXaccum5,M/CUs,N,KU,KL);
	accum(AXscal6,AXaccum6,M/CUs,N,KU,KL);
	accum(AXscal7,AXaccum7,M/CUs,N,KU,KL);
	accum(AXscal8,AXaccum8,M/CUs,N,KU,KL);
	accum(AXscal9,AXaccum9,M/CUs,N,KU,KL);
	accum(AXscal10,AXaccum10,M/CUs,N,KU,KL);

	reduction(AXaccum1,AXreduc1,M/CUs);
	reduction(AXaccum2,AXreduc2,M/CUs);
	reduction(AXaccum3,AXreduc3,M/CUs);
	reduction(AXaccum4,AXreduc4,M/CUs);
	reduction(AXaccum5,AXreduc5,M/CUs);
	reduction(AXaccum6,AXreduc6,M/CUs);
	reduction(AXaccum7,AXreduc7,M/CUs);
	reduction(AXaccum8,AXreduc8,M/CUs);
	reduction(AXaccum9,AXreduc9,M/CUs);
	reduction(AXaccum10,AXreduc10,M/CUs);

	scal_y(Y_in1,Y_scal1,M/CUs,beta);
	scal_y(Y_in2,Y_scal2,M/CUs,beta);
	scal_y(Y_in3,Y_scal3,M/CUs,beta);
	scal_y(Y_in4,Y_scal4,M/CUs,beta);
	scal_y(Y_in5,Y_scal5,M/CUs,beta);
	scal_y(Y_in6,Y_scal6,M/CUs,beta);
	scal_y(Y_in7,Y_scal7,M/CUs,beta);
	scal_y(Y_in8,Y_scal8,M/CUs,beta);
	scal_y(Y_in9,Y_scal9,M/CUs,beta);
	scal_y(Y_in10,Y_scal10,M/CUs,beta);

	write_y(Y_scal1,AXreduc1,Y1,M/CUs);
	write_y(Y_scal2,AXreduc2,Y2,M/CUs);
	write_y(Y_scal3,AXreduc3,Y3,M/CUs);
	write_y(Y_scal4,AXreduc4,Y4,M/CUs);
	write_y(Y_scal5,AXreduc5,Y5,M/CUs);
	write_y(Y_scal6,AXreduc6,Y6,M/CUs);
	write_y(Y_scal7,AXreduc7,Y7,M/CUs);
	write_y(Y_scal8,AXreduc8,Y8,M/CUs);
	write_y(Y_scal9,AXreduc9,Y9,M/CUs);
	write_y(Y_scal10,AXreduc10,Y10,M/CUs);

}
}
