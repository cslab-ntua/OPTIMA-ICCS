#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
#define CUs 14

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void read_a(hls::stream<v_dt>& Astream,v_dt *A,int M,int N){

	int i=0;

	read_a:
	while(i<M*(N/VDATA_SIZE)){
		#pragma HLS pipeline II=1
		v_dt temp;
		temp = A[i];
		Astream << temp;
		i++;
	}
}

void read_x(hls::stream<v_dt>& Xstream1,hls::stream<v_dt>& Xstream2,hls::stream<v_dt>& Xstream3,hls::stream<v_dt>& Xstream4,hls::stream<v_dt>& Xstream5,hls::stream<v_dt>& Xstream6,
		hls::stream<v_dt>& Xstream7,hls::stream<v_dt>& Xstream8,hls::stream<v_dt>& Xstream9,hls::stream<v_dt>& Xstream10,hls::stream<v_dt>& Xstream11,hls::stream<v_dt>& Xstream12, 
		hls::stream<v_dt>& Xstream13, hls::stream<v_dt>& Xstream14, v_dt *X,int M,int N){

	int i=0;

	read_x:
	while(i<M*(N/VDATA_SIZE)){
		#pragma HLS pipeline II=1
		v_dt temp;
		temp = X[i%(N/VDATA_SIZE)];
		Xstream1 << temp;
		Xstream2 << temp;
		Xstream3 << temp;
		Xstream4 << temp;
		Xstream5 << temp;
		Xstream6 << temp;
		Xstream7 << temp;
		Xstream8 << temp;
		Xstream9 << temp;
		Xstream10 << temp;
		Xstream11 << temp;
		Xstream12 << temp;
		Xstream13 << temp;
		Xstream14 << temp;


		i++;
	}
}

void read_y(hls::stream<float>& Ystream,float *Y,int M){

	read_y:
	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Ystream <<Y[i];
	}
}

void scal_ax(hls::stream<v_dt>& Astream,hls::stream<v_dt>& Xstream,hls::stream<v_dt>& AXscal,int M,int N,float alpha){

	int i=0;

	scal_ax:
	while(i<M*(N/VDATA_SIZE)){
		#pragma HLS pipeline II=1
		v_dt temp;
		v_dt X_temp = Xstream.read();
		v_dt A_temp = Astream.read();
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
		}
		AXscal << temp;
		i++;
	}
}

void accum(hls::stream<v_dt>& AXscal,hls::stream<v_dt>& AXaccum,int M,int N){

        non_pipelined:
        for(int i=0;i<M;i++){
                v_dt Y_temp;

                for(int k=0;k<VDATA_SIZE;k++){
                        #pragma HLS unroll
                        Y_temp.data[k]=0.0f;
                }


                pipelined:
                for(int j=0;j<N;j+=VDATA_SIZE){
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

void sum_y(hls::stream<float>& Y_in1,hls::stream<float>& Y_in2,hls::stream<float>& Y_out,int M){

	sum_y:
	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Y_out << Y_in1.read()+Y_in2.read();
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

void write_y(hls::stream<float>& Ystream,float *Y,int M){

	write:
	for(int i=0;i<M;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();
	}
}

extern "C" {
void krnl_gemv(const char TransA, const int M, const int N, const float alpha, v_dt  *A1, v_dt  *A2, v_dt  *A3, v_dt  *A4, v_dt  *A5, v_dt  *A6, v_dt  *A7, v_dt  *A8, v_dt  *A9, v_dt  *A10, 
		v_dt  *A11, v_dt  *A12, v_dt  *A13, v_dt  *A14, const int lda, v_dt  *X, const int incX, const float beta, float  *Y1, float  *Y2, float  *Y3, float  *Y4, float  *Y5, 
		float  *Y6, float  *Y7, float  *Y8, float  *Y9, float  *Y10, float  *Y11, float  *Y12, float  *Y13, float  *Y14, const int incY) {

	#pragma HLS INTERFACE m_axi port = X offset = slave bundle = mem0 //HBM 0
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = mem1 //HBM 1
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = mem2 //HBM 1
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = mem3 //HBM 1
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = mem4 //HBM 1
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = mem5 //HBM 1
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = mem6 //HBM 1
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = mem7 //HBM 1
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = mem8 //HBM 1
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = mem9 //HBM 1
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = mem10 //HBM 1
	#pragma HLS INTERFACE m_axi port = A11 offset = slave bundle = mem11 //HBM 1
	#pragma HLS INTERFACE m_axi port = A12 offset = slave bundle = mem12 //HBM 1
	#pragma HLS INTERFACE m_axi port = A13 offset = slave bundle = mem13 //HBM 1
	#pragma HLS INTERFACE m_axi port = A14 offset = slave bundle = mem14 //HBM 1

	#pragma HLS INTERFACE m_axi port = Y1 offset = slave bundle = mem15 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y2 offset = slave bundle = mem16 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y3 offset = slave bundle = mem17 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y4 offset = slave bundle = mem18 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y5 offset = slave bundle = mem19 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y6 offset = slave bundle = mem20 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y7 offset = slave bundle = mem21 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y8 offset = slave bundle = mem22 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y9 offset = slave bundle = mem23 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y10 offset = slave bundle = mem24 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y11 offset = slave bundle = mem25 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y12 offset = slave bundle = mem26 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y13 offset = slave bundle = mem27 //HBM 0
	#pragma HLS INTERFACE m_axi port = Y14 offset = slave bundle = mem28 //HBM 0


	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = M
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = incX
	#pragma HLS INTERFACE s_axilite port = incY
	#pragma HLS INTERFACE s_axilite port = alpha
	#pragma HLS INTERFACE s_axilite port = beta

	static hls::stream<v_dt> A_temp1("Atemp1");
	static hls::stream<v_dt> A_temp2("Atemp2");
	static hls::stream<v_dt> A_temp3("Atemp3");
	static hls::stream<v_dt> A_temp4("Atemp4");
	static hls::stream<v_dt> A_temp5("Atemp5");
	static hls::stream<v_dt> A_temp6("Atemp6");
	static hls::stream<v_dt> A_temp7("Atemp7");
	static hls::stream<v_dt> A_temp8("Atemp8");
	static hls::stream<v_dt> A_temp9("Atemp9");
	static hls::stream<v_dt> A_temp10("Atemp10");
	static hls::stream<v_dt> A_temp11("Atemp11");
	static hls::stream<v_dt> A_temp12("Atemp12");
	static hls::stream<v_dt> A_temp13("Atemp13");
	static hls::stream<v_dt> A_temp14("Atemp14");

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
	static hls::stream<float> Y_in11("Yin11");
	static hls::stream<float> Y_in12("Yin12");
	static hls::stream<float> Y_in13("Yin13");
	static hls::stream<float> Y_in14("Yin14");

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
	static hls::stream<float> Y_scal11("Yscal11");
	static hls::stream<float> Y_scal12("Yscal12");
	static hls::stream<float> Y_scal13("Yscal13");
	static hls::stream<float> Y_scal14("Yscal14");

	static hls::stream<v_dt> AX_scal1("AXscal1");
	static hls::stream<v_dt> AX_scal2("AXscal2");
	static hls::stream<v_dt> AX_scal3("AXscal3");
	static hls::stream<v_dt> AX_scal4("AXscal4");
	static hls::stream<v_dt> AX_scal5("AXscal5");
	static hls::stream<v_dt> AX_scal6("AXscal6");
	static hls::stream<v_dt> AX_scal7("AXscal7");
	static hls::stream<v_dt> AX_scal8("AXscal8");
	static hls::stream<v_dt> AX_scal9("AXscal9");
	static hls::stream<v_dt> AX_scal10("AXscal10");
	static hls::stream<v_dt> AX_scal11("AXscal11");
	static hls::stream<v_dt> AX_scal12("AXscal12");
	static hls::stream<v_dt> AX_scal13("AXscal13");
	static hls::stream<v_dt> AX_scal14("AXscal14");

	static hls::stream<v_dt> AX_accum1("AXaccum1");
	static hls::stream<v_dt> AX_accum2("AXaccum2");
	static hls::stream<v_dt> AX_accum3("AXaccum3");
	static hls::stream<v_dt> AX_accum4("AXaccum4");
	static hls::stream<v_dt> AX_accum5("AXaccum5");
	static hls::stream<v_dt> AX_accum6("AXaccum6");
	static hls::stream<v_dt> AX_accum7("AXaccum7");
	static hls::stream<v_dt> AX_accum8("AXaccum8");
	static hls::stream<v_dt> AX_accum9("AXaccum9");
	static hls::stream<v_dt> AX_accum10("AXaccum10");
	static hls::stream<v_dt> AX_accum11("AXaccum11");
	static hls::stream<v_dt> AX_accum12("AXaccum12");
	static hls::stream<v_dt> AX_accum13("AXaccum13");
	static hls::stream<v_dt> AX_accum14("AXaccum14");

	static hls::stream<float> AX_reduc1("AXreduc1");
	static hls::stream<float> AX_reduc2("AXreduc2");
	static hls::stream<float> AX_reduc3("AXreduc3");
	static hls::stream<float> AX_reduc4("AXreduc4");
	static hls::stream<float> AX_reduc5("AXreduc5");
	static hls::stream<float> AX_reduc6("AXreduc6");
	static hls::stream<float> AX_reduc7("AXreduc7");
	static hls::stream<float> AX_reduc8("AXreduc8");
	static hls::stream<float> AX_reduc9("AXreduc9");
	static hls::stream<float> AX_reduc10("AXreduc10");
	static hls::stream<float> AX_reduc11("AXreduc11");
	static hls::stream<float> AX_reduc12("AXreduc12");
	static hls::stream<float> AX_reduc13("AXreduc13");
	static hls::stream<float> AX_reduc14("AXreduc14");

	static hls::stream<float> Y_out1("Yout1");
	static hls::stream<float> Y_out2("Yout2");
	static hls::stream<float> Y_out3("Yout3");
	static hls::stream<float> Y_out4("Yout4");
	static hls::stream<float> Y_out5("Yout5");
	static hls::stream<float> Y_out6("Yout6");
	static hls::stream<float> Y_out7("Yout7");
	static hls::stream<float> Y_out8("Yout8");
	static hls::stream<float> Y_out9("Yout9");
	static hls::stream<float> Y_out10("Yout10");
	static hls::stream<float> Y_out11("Yout11");
	static hls::stream<float> Y_out12("Yout12");
	static hls::stream<float> Y_out13("Yout13");
	static hls::stream<float> Y_out14("Yout14");

	static hls::stream<v_dt> X_temp1("Xtemp1");
	static hls::stream<v_dt> X_temp2("Xtemp2");
	static hls::stream<v_dt> X_temp3("Xtemp3");
	static hls::stream<v_dt> X_temp4("Xtemp4");
	static hls::stream<v_dt> X_temp5("Xtemp5");
	static hls::stream<v_dt> X_temp6("Xtemp6");
	static hls::stream<v_dt> X_temp7("Xtemp7");
	static hls::stream<v_dt> X_temp8("Xtemp8");
	static hls::stream<v_dt> X_temp9("Xtemp9");
	static hls::stream<v_dt> X_temp10("Xtemp10");
	static hls::stream<v_dt> X_temp11("Xtemp11");
	static hls::stream<v_dt> X_temp12("Xtemp12");
	static hls::stream<v_dt> X_temp13("Xtemp13");
	static hls::stream<v_dt> X_temp14("Xtemp14");


	#pragma HLS STREAM variable = A_temp1 depth = 2048
	#pragma HLS STREAM variable = Y_in1 depth = 2048
	#pragma HLS STREAM variable = Y_scal1 depth = 2048
	#pragma HLS STREAM variable = AX_scal1 depth = 2048
	#pragma HLS STREAM variable = AX_accum1 depth = 2048
	#pragma HLS STREAM variable = Y_out1 depth = 2048
	#pragma HLS STREAM variable = X_temp1 depth = 2048

	#pragma HLS STREAM variable = A_temp2 depth = 2048
	#pragma HLS STREAM variable = Y_in2 depth = 2048
	#pragma HLS STREAM variable = Y_scal2 depth = 2048
	#pragma HLS STREAM variable = AX_scal2 depth = 2048
	#pragma HLS STREAM variable = AX_accum2 depth = 2048
	#pragma HLS STREAM variable = Y_out2 depth = 2048
	#pragma HLS STREAM variable = X_temp2 depth = 2048

	#pragma HLS STREAM variable = A_temp3 depth = 2048
	#pragma HLS STREAM variable = Y_in3 depth = 2048
	#pragma HLS STREAM variable = Y_scal3 depth = 2048
	#pragma HLS STREAM variable = AX_scal3 depth = 2048
	#pragma HLS STREAM variable = AX_accum3 depth = 2048
	#pragma HLS STREAM variable = Y_out3 depth = 2048
	#pragma HLS STREAM variable = X_temp3 depth = 2048

	#pragma HLS STREAM variable = A_temp4 depth = 2048
	#pragma HLS STREAM variable = Y_in4 depth = 2048
	#pragma HLS STREAM variable = Y_scal4 depth = 2048
	#pragma HLS STREAM variable = AX_scal4 depth = 2048
	#pragma HLS STREAM variable = AX_accum4 depth = 2048
	#pragma HLS STREAM variable = Y_out4 depth = 2048
	#pragma HLS STREAM variable = X_temp4 depth = 2048

        #pragma HLS STREAM variable = A_temp5 depth = 2048
        #pragma HLS STREAM variable = Y_in5 depth = 2048
        #pragma HLS STREAM variable = Y_scal5 depth = 2048
        #pragma HLS STREAM variable = AX_scal5 depth = 2048
        #pragma HLS STREAM variable = AX_accum5 depth = 2048
        #pragma HLS STREAM variable = Y_out5 depth = 2048
        #pragma HLS STREAM variable = X_temp5 depth = 2048

	#pragma HLS STREAM variable = A_temp6 depth = 2048
        #pragma HLS STREAM variable = Y_in6 depth = 2048
        #pragma HLS STREAM variable = Y_scal6 depth = 2048
        #pragma HLS STREAM variable = AX_scal6 depth = 2048
        #pragma HLS STREAM variable = AX_accum6 depth = 2048
        #pragma HLS STREAM variable = Y_out6 depth = 2048
        #pragma HLS STREAM variable = X_temp6 depth = 2048

	#pragma HLS STREAM variable = A_temp7 depth = 2048
        #pragma HLS STREAM variable = Y_in7 depth = 2048
        #pragma HLS STREAM variable = Y_scal7 depth = 2048
        #pragma HLS STREAM variable = AX_scal7 depth = 2048
        #pragma HLS STREAM variable = AX_accum7 depth = 2048
        #pragma HLS STREAM variable = Y_out7 depth = 2048
        #pragma HLS STREAM variable = X_temp7 depth = 2048

        #pragma HLS STREAM variable = A_temp8 depth = 2048
        #pragma HLS STREAM variable = Y_in8 depth = 2048
        #pragma HLS STREAM variable = Y_scal8 depth = 2048
        #pragma HLS STREAM variable = AX_scal8 depth = 2048
        #pragma HLS STREAM variable = AX_accum8 depth = 2048
        #pragma HLS STREAM variable = Y_out8 depth = 2048
        #pragma HLS STREAM variable = X_temp8 depth = 2048
        
	#pragma HLS STREAM variable = A_temp9 depth = 2048
        #pragma HLS STREAM variable = Y_in9 depth = 2048
        #pragma HLS STREAM variable = Y_scal9 depth = 2048
        #pragma HLS STREAM variable = AX_scal9 depth = 2048
        #pragma HLS STREAM variable = AX_accum9 depth = 2048
        #pragma HLS STREAM variable = Y_out9 depth = 2048
        #pragma HLS STREAM variable = X_temp9 depth = 2048
	       
	#pragma HLS STREAM variable = A_temp10 depth = 2048
        #pragma HLS STREAM variable = Y_in10 depth = 2048
        #pragma HLS STREAM variable = Y_scal10 depth = 2048
        #pragma HLS STREAM variable = AX_scal10 depth = 2048
        #pragma HLS STREAM variable = AX_accum10 depth = 2048
        #pragma HLS STREAM variable = Y_out10 depth = 2048
        #pragma HLS STREAM variable = X_temp10 depth = 2048
       
       	#pragma HLS STREAM variable = A_temp11 depth = 2048
        #pragma HLS STREAM variable = Y_in11 depth = 2048
        #pragma HLS STREAM variable = Y_scal11 depth = 2048
        #pragma HLS STREAM variable = AX_scal11 depth = 2048
        #pragma HLS STREAM variable = AX_accum11 depth = 2048
        #pragma HLS STREAM variable = Y_out11 depth = 2048
        #pragma HLS STREAM variable = X_temp11 depth = 2048

	#pragma HLS STREAM variable = A_temp12 depth = 2048
        #pragma HLS STREAM variable = Y_in12 depth = 2048
        #pragma HLS STREAM variable = Y_scal12 depth = 2048
        #pragma HLS STREAM variable = AX_scal12 depth = 2048
        #pragma HLS STREAM variable = AX_accum12 depth = 2048
        #pragma HLS STREAM variable = Y_out12 depth = 2048
        #pragma HLS STREAM variable = X_temp12 depth = 2048

        #pragma HLS STREAM variable = A_temp13 depth = 2048
        #pragma HLS STREAM variable = Y_in13 depth = 2048
        #pragma HLS STREAM variable = Y_scal13 depth = 2048
        #pragma HLS STREAM variable = AX_scal13 depth = 2048
        #pragma HLS STREAM variable = AX_accum13 depth = 2048
        #pragma HLS STREAM variable = Y_out13 depth = 2048
        #pragma HLS STREAM variable = X_temp13 depth = 2048

        #pragma HLS STREAM variable = A_temp14 depth = 2048
        #pragma HLS STREAM variable = Y_in14 depth = 2048
        #pragma HLS STREAM variable = Y_scal14 depth = 2048
        #pragma HLS STREAM variable = AX_scal14 depth = 2048
        #pragma HLS STREAM variable = AX_accum14 depth = 2048
        #pragma HLS STREAM variable = Y_out14 depth = 2048
        #pragma HLS STREAM variable = X_temp14 depth = 2048

	#pragma HLS dataflow
	read_a(A_temp1,A1,M/CUs,N);
	read_a(A_temp2,A2,M/CUs,N);
	read_a(A_temp3,A3,M/CUs,N);
	read_a(A_temp4,A4,M/CUs,N);
	read_a(A_temp5,A5,M/CUs,N);
	read_a(A_temp6,A6,M/CUs,N);
	read_a(A_temp7,A7,M/CUs,N);
	read_a(A_temp8,A8,M/CUs,N);
	read_a(A_temp9,A9,M/CUs,N);
	read_a(A_temp10,A10,M/CUs,N);
	read_a(A_temp11,A11,M/CUs,N);
	read_a(A_temp12,A12,M/CUs,N);
	read_a(A_temp13,A13,M/CUs,N);
	read_a(A_temp14,A14,M/CUs,N);


	read_x(X_temp1,X_temp2,X_temp3,X_temp4,X_temp5,X_temp6,X_temp7,X_temp8,X_temp9,
			X_temp10,X_temp11,X_temp12,X_temp13,X_temp14,X,M/CUs,N);

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
	read_y(Y_in11,Y11,M/CUs);
	read_y(Y_in12,Y12,M/CUs);
	read_y(Y_in13,Y13,M/CUs);
	read_y(Y_in14,Y14,M/CUs);

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
   	scal_y(Y_in11,Y_scal11,M/CUs,beta);
   	scal_y(Y_in12,Y_scal12,M/CUs,beta);
   	scal_y(Y_in13,Y_scal13,M/CUs,beta);
   	scal_y(Y_in14,Y_scal14,M/CUs,beta);

   	scal_ax(A_temp1,X_temp1,AX_scal1,M/CUs,N,alpha);
   	scal_ax(A_temp2,X_temp2,AX_scal2,M/CUs,N,alpha);
   	scal_ax(A_temp3,X_temp3,AX_scal3,M/CUs,N,alpha);
   	scal_ax(A_temp4,X_temp4,AX_scal4,M/CUs,N,alpha);
   	scal_ax(A_temp5,X_temp5,AX_scal5,M/CUs,N,alpha);
   	scal_ax(A_temp6,X_temp6,AX_scal6,M/CUs,N,alpha);
   	scal_ax(A_temp7,X_temp7,AX_scal7,M/CUs,N,alpha);
   	scal_ax(A_temp8,X_temp8,AX_scal8,M/CUs,N,alpha);
   	scal_ax(A_temp9,X_temp9,AX_scal9,M/CUs,N,alpha);
   	scal_ax(A_temp10,X_temp10,AX_scal10,M/CUs,N,alpha);
   	scal_ax(A_temp11,X_temp11,AX_scal11,M/CUs,N,alpha);
   	scal_ax(A_temp12,X_temp12,AX_scal12,M/CUs,N,alpha);
   	scal_ax(A_temp13,X_temp13,AX_scal13,M/CUs,N,alpha);
   	scal_ax(A_temp14,X_temp14,AX_scal14,M/CUs,N,alpha);

	accum(AX_scal1,AX_accum1,M/CUs,N);
	accum(AX_scal2,AX_accum2,M/CUs,N);
	accum(AX_scal3,AX_accum3,M/CUs,N);
	accum(AX_scal4,AX_accum4,M/CUs,N);
	accum(AX_scal5,AX_accum5,M/CUs,N);
	accum(AX_scal6,AX_accum6,M/CUs,N);
	accum(AX_scal7,AX_accum7,M/CUs,N);
	accum(AX_scal8,AX_accum8,M/CUs,N);
	accum(AX_scal9,AX_accum9,M/CUs,N);
	accum(AX_scal10,AX_accum10,M/CUs,N);
	accum(AX_scal11,AX_accum11,M/CUs,N);
	accum(AX_scal12,AX_accum12,M/CUs,N);
	accum(AX_scal13,AX_accum13,M/CUs,N);
	accum(AX_scal14,AX_accum14,M/CUs,N);

	reduction(AX_accum1,AX_reduc1,M/CUs);
	reduction(AX_accum2,AX_reduc2,M/CUs);
	reduction(AX_accum3,AX_reduc3,M/CUs);
	reduction(AX_accum4,AX_reduc4,M/CUs);
	reduction(AX_accum5,AX_reduc5,M/CUs);
	reduction(AX_accum6,AX_reduc6,M/CUs);
	reduction(AX_accum7,AX_reduc7,M/CUs);
	reduction(AX_accum8,AX_reduc8,M/CUs);
	reduction(AX_accum9,AX_reduc9,M/CUs);
	reduction(AX_accum10,AX_reduc10,M/CUs);
	reduction(AX_accum11,AX_reduc11,M/CUs);
	reduction(AX_accum12,AX_reduc12,M/CUs);
	reduction(AX_accum13,AX_reduc13,M/CUs);
	reduction(AX_accum14,AX_reduc14,M/CUs);

	sum_y(AX_reduc1,Y_scal1,Y_out1,M/CUs);
	sum_y(AX_reduc2,Y_scal2,Y_out2,M/CUs);
	sum_y(AX_reduc3,Y_scal3,Y_out3,M/CUs);
	sum_y(AX_reduc4,Y_scal4,Y_out4,M/CUs);
	sum_y(AX_reduc5,Y_scal5,Y_out5,M/CUs);
	sum_y(AX_reduc6,Y_scal6,Y_out6,M/CUs);
	sum_y(AX_reduc7,Y_scal7,Y_out7,M/CUs);
	sum_y(AX_reduc8,Y_scal8,Y_out8,M/CUs);
	sum_y(AX_reduc9,Y_scal9,Y_out9,M/CUs);
	sum_y(AX_reduc10,Y_scal10,Y_out10,M/CUs);
	sum_y(AX_reduc11,Y_scal11,Y_out11,M/CUs);
	sum_y(AX_reduc12,Y_scal12,Y_out12,M/CUs);
	sum_y(AX_reduc13,Y_scal13,Y_out13,M/CUs);
	sum_y(AX_reduc14,Y_scal14,Y_out14,M/CUs);

	write_y(Y_out1,Y1,M/CUs);
	write_y(Y_out2,Y2,M/CUs);
	write_y(Y_out3,Y3,M/CUs);
	write_y(Y_out4,Y4,M/CUs);
	write_y(Y_out5,Y5,M/CUs);
	write_y(Y_out6,Y6,M/CUs);
	write_y(Y_out7,Y7,M/CUs);
	write_y(Y_out8,Y8,M/CUs);
	write_y(Y_out9,Y9,M/CUs);
	write_y(Y_out10,Y10,M/CUs);
	write_y(Y_out11,Y11,M/CUs);
	write_y(Y_out12,Y12,M/CUs);
	write_y(Y_out13,Y13,M/CUs);
	write_y(Y_out14,Y14,M/CUs);

}}
