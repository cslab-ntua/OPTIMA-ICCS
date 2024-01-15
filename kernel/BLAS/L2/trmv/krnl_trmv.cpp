#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void read_x(const char Uplo,hls::stream<v_dt>& Xstream,const v_dt *X,const int N, const int start_row, const int end_row){

	int i=0, j=start_row/VDATA_SIZE;

	read_x:
	while(i<end_row){
		#pragma HLS pipeline II=1

		v_dt Xtemp = X[j];
		Xstream << Xtemp;

		int cond = N/VDATA_SIZE - 1;
		int res = (i+start_row)/VDATA_SIZE;
		
		j==cond ? i++ : i+=0;
                j==cond ? j=res : j++;

		//j==N/VDATA_SIZE-1 ? i++ : i+=0;
		//j==N/VDATA_SIZE-1 ? j=(i+start_row)/VDATA_SIZE : j++;		
		/*if (Uplo == 'U' or Uplo == 'u'){
		if (j==N/VDATA_SIZE-1){
			i++;
			j=(i+start_row)/VDATA_SIZE;
		}
		else{
			j++;
		}
		}
		else if (Uplo == 'L' or Uplo == 'l'){
			if (j==i/VDATA_SIZE){
				i++;
				j=0;
			}
			else{
				j++;
			}
		}*/
	}
}


void write_x(hls::stream<float>& Ystream,float *Y,const int N, const int end_row){


	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();
	}
}

void read_matrix(const char Uplo,const v_dt *A, hls::stream<v_dt>& AStream,const int N, const int start_row, const int end_row){

	int i=0, j=start_row/VDATA_SIZE;

	read_a:
	while(i<end_row){
		#pragma HLS pipeline II=1

		v_dt Atemp = A[i*(N/VDATA_SIZE)+j];
		AStream << Atemp;

		int cond = N/VDATA_SIZE - 1;
                int res = (i+start_row)/VDATA_SIZE;

                j==cond ? i++ : i+=0;
                j==cond ? j=res : j++;
		//j==N/VDATA_SIZE-1 ? i++ : i+=0;
                //j==N/VDATA_SIZE-1 ? j=(i+start_row)/VDATA_SIZE : j++;
		/*if (Uplo == 'U' or Uplo == 'u'){
		if (j==N/VDATA_SIZE-1){
			i++;
			j=(start_row+i)/VDATA_SIZE;
		}
		else{
			j++;
		}
		}
		else if (Uplo == 'L' or Uplo == 'l'){
			if (j==i/VDATA_SIZE){
				i++;
				j=0;
			}
			else{
				j++;
			}
		}*/
	}
}

void scal_ax(const char Uplo,hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y,const int N, const int start_row, const int end_row){

	int i=0,j=start_row/VDATA_SIZE;

	scal_ax:
	while(i<end_row){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		int cond1 = (i+start_row)/VDATA_SIZE;
		int cond2 = (i+start_row)%VDATA_SIZE;

		int cond = N/VDATA_SIZE - 1;
                int res = (i+start_row)/VDATA_SIZE;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			//if(j==(i+start_row)/VDATA_SIZE and k<(i+start_row)%VDATA_SIZE){
			j==cond1 and k<cond2 ? temp.data[k]=0 : temp.data[k]=X_temp.data[k]*A_temp.data[k];
			/*if(j==cond1 and k<cond2){
				temp.data[k]=0;
			}
			else{
				temp.data[k]=X_temp.data[k]*A_temp.data[k];
			}*/
		}

		Y << temp;
		
		j==cond ? i++ : i+=0;
                j==cond ? j=res : j++;

		//j==N/VDATA_SIZE-1 ? i++ : i+=0;
                //j==N/VDATA_SIZE-1 ? j=(i+start_row)/VDATA_SIZE : j++;
		/*	if (j==N/VDATA_SIZE-1){
				i++;
				j=(start_row+i)/VDATA_SIZE;
			}
			else{
				j++;
			}*/
	}
}

void reduction(hls::stream<v_dt>& In,hls::stream<float>& Out,const int N, const int end_row){

	reduction:
	for(int i=0;i<end_row;i++){
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

void accum(const char Uplo,hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row){

	v_dt X_temp;
	int i=0,j=start_row/VDATA_SIZE;

	nested_pipelined:
	while(i<end_row){
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

		//int cond = N/VDATA_SIZE - 1;
                //int res = (i+start_row)/VDATA_SIZE;

		if (j==cond) Out << X_temp;

		j==cond ? i++ : i+=0;
                j==cond ? j=res : j++;

		/*if (j==N/VDATA_SIZE-1) Out << X_temp;
		if (j==N/VDATA_SIZE-1){
			i++;
			j=(start_row+i)/VDATA_SIZE;
		}
		else{
			j++;
		}*/

	}
}

extern "C" {
void krnl_trmv(const char Uplo, const char TransA, const char Diag, const int N, const int break_row1, const int break_row2, const int break_row3,
		const int break_row4, const int break_row5, const int break_row6, const int break_row7, const int break_row8, const int break_row9,
		const int break_row10, const int break_row11, const int break_row12, const int break_row13, const v_dt  *A1, const v_dt  *A2, const v_dt  *A3, 
		const v_dt  *A4, const v_dt  *A5, const v_dt  *A6, const v_dt  *A7, const v_dt  *A8, const v_dt  *A9, const v_dt  *A10, const v_dt  *A11,
	    const v_dt  *A12, const v_dt  *A13, const v_dt  *A14, const int lda, v_dt  *X_in1, v_dt  *X_in2, v_dt  *X_in3, v_dt  *X_in4, v_dt  *X_in5,
		v_dt  *X_in6, v_dt  *X_in7, v_dt  *X_in8, v_dt  *X_in9, v_dt  *X_in10, v_dt  *X_in11, v_dt  *X_in12, v_dt  *X_in13, v_dt  *X_in14,
		const int incX,	float  *X_out1, float  *X_out2, float  *X_out3, float  *X_out4, float  *X_out5, float  *X_out6, float  *X_out7, 
		float  *X_out8, float  *X_out9, float  *X_out10, float  *X_out11, float  *X_out12, float  *X_out13, float  *X_out14){

	#pragma HLS INTERFACE m_axi port = X_in1 offset = slave bundle = mem0
	#pragma HLS INTERFACE m_axi port = X_in2 offset = slave bundle = mem1
	#pragma HLS INTERFACE m_axi port = X_in3 offset = slave bundle = mem2
	#pragma HLS INTERFACE m_axi port = X_in4 offset = slave bundle = mem3
	#pragma HLS INTERFACE m_axi port = X_in5 offset = slave bundle = mem4
	#pragma HLS INTERFACE m_axi port = X_in6 offset = slave bundle = mem5
	#pragma HLS INTERFACE m_axi port = X_in7 offset = slave bundle = mem6
	#pragma HLS INTERFACE m_axi port = X_in8 offset = slave bundle = mem7
	#pragma HLS INTERFACE m_axi port = X_in9 offset = slave bundle = mem8
	#pragma HLS INTERFACE m_axi port = X_in10 offset = slave bundle = mem9
	#pragma HLS INTERFACE m_axi port = X_in11 offset = slave bundle = mem10
	#pragma HLS INTERFACE m_axi port = X_in12 offset = slave bundle = mem11
	#pragma HLS INTERFACE m_axi port = X_in13 offset = slave bundle = mem12
	#pragma HLS INTERFACE m_axi port = X_in14 offset = slave bundle = mem13
	#pragma HLS INTERFACE m_axi port = X_out1 offset = slave bundle = mem0
    #pragma HLS INTERFACE m_axi port = X_out2 offset = slave bundle = mem1
    #pragma HLS INTERFACE m_axi port = X_out3 offset = slave bundle = mem2
    #pragma HLS INTERFACE m_axi port = X_out4 offset = slave bundle = mem3	
    #pragma HLS INTERFACE m_axi port = X_out5 offset = slave bundle = mem4	
    #pragma HLS INTERFACE m_axi port = X_out6 offset = slave bundle = mem5	
    #pragma HLS INTERFACE m_axi port = X_out7 offset = slave bundle = mem6	
    #pragma HLS INTERFACE m_axi port = X_out8 offset = slave bundle = mem7	
    #pragma HLS INTERFACE m_axi port = X_out9 offset = slave bundle = mem8	
    #pragma HLS INTERFACE m_axi port = X_out10 offset = slave bundle = mem9	
    #pragma HLS INTERFACE m_axi port = X_out11 offset = slave bundle = mem10	
    #pragma HLS INTERFACE m_axi port = X_out12 offset = slave bundle = mem11
	#pragma HLS INTERFACE m_axi port = X_out13 offset = slave bundle = mem12
    #pragma HLS INTERFACE m_axi port = X_out14 offset = slave bundle = mem13
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = mem14
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = mem15
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = mem16
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = mem17
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = mem18
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = mem19
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = mem20
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = mem21
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = mem22
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = mem23
	#pragma HLS INTERFACE m_axi port = A11 offset = slave bundle = mem24
	#pragma HLS INTERFACE m_axi port = A12 offset = slave bundle = mem25
	#pragma HLS INTERFACE m_axi port = A13 offset = slave bundle = mem26
	#pragma HLS INTERFACE m_axi port = A14 offset = slave bundle = mem27

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
	#pragma HLS INTERFACE s_axilite port = break_row10
	#pragma HLS INTERFACE s_axilite port = break_row11
	#pragma HLS INTERFACE s_axilite port = break_row12
	#pragma HLS INTERFACE s_axilite port = break_row13	
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = incX
	#pragma HLS INTERFACE s_axilite port = Diag
	#pragma HLS INTERFACE s_axilite port = Uplo


	static hls::stream<v_dt> A1_in("A1_in");
	static hls::stream<v_dt> X1_in("X1_in");
	static hls::stream<v_dt> X1_scal("X1_scal");
	static hls::stream<v_dt> X1_accum("X1_accum");
	static hls::stream<float> X1_out("X1_out");

	static hls::stream<v_dt> A2_in("A2_in");
	static hls::stream<v_dt> X2_in("X2_in");
	static hls::stream<v_dt> X2_scal("X2_scal");
	static hls::stream<v_dt> X2_accum("X2_accum");
	static hls::stream<float> X2_out("X2_out");

	static hls::stream<v_dt> A3_in("A3_in");
	static hls::stream<v_dt> X3_in("X3_in");
	static hls::stream<v_dt> X3_scal("X3_scal");
	static hls::stream<v_dt> X3_accum("X3_accum");
	static hls::stream<float> X3_out("X3_out");

	static hls::stream<v_dt> A4_in("A4_in");
	static hls::stream<v_dt> X4_in("X4_in");
	static hls::stream<v_dt> X4_scal("X4_scal");
	static hls::stream<v_dt> X4_accum("X4_accum");
	static hls::stream<float> X4_out("X4_out");

	static hls::stream<v_dt> A5_in("A5_in");
    static hls::stream<v_dt> X5_in("X5_in");
    static hls::stream<v_dt> X5_scal("X5_scal");
    static hls::stream<v_dt> X5_accum("X5_accum");
    static hls::stream<float> X5_out("X5_out");
	
	static hls::stream<v_dt> A6_in("A6_in");
    static hls::stream<v_dt> X6_in("X6_in");
    static hls::stream<v_dt> X6_scal("X6_scal");
    static hls::stream<v_dt> X6_accum("X6_accum");
    static hls::stream<float> X6_out("X6_out");
	
	static hls::stream<v_dt> A7_in("A7_in");
    static hls::stream<v_dt> X7_in("X7_in");
    static hls::stream<v_dt> X7_scal("X7_scal");
    static hls::stream<v_dt> X7_accum("X7_accum");
    static hls::stream<float> X7_out("X7_out");
	
	static hls::stream<v_dt> A8_in("A8_in");
    static hls::stream<v_dt> X8_in("X8_in");
    static hls::stream<v_dt> X8_scal("X8_scal");
    static hls::stream<v_dt> X8_accum("X8_accum");
    static hls::stream<float> X8_out("X8_out");
	
	static hls::stream<v_dt> A9_in("A9_in");
    static hls::stream<v_dt> X9_in("X9_in");
    static hls::stream<v_dt> X9_scal("X9_scal");
    static hls::stream<v_dt> X9_accum("X9_accum");
    static hls::stream<float> X9_out("X9_out");
	
	static hls::stream<v_dt> A10_in("A10_in");
    static hls::stream<v_dt> X10_in("X10_in");
    static hls::stream<v_dt> X10_scal("X10_scal");
    static hls::stream<v_dt> X10_accum("X10_accum");
    static hls::stream<float> X10_out("X10_out");
	
	static hls::stream<v_dt> A11_in("A11_in");
    static hls::stream<v_dt> X11_in("X11_in");
    static hls::stream<v_dt> X11_scal("X11_scal");
    static hls::stream<v_dt> X11_accum("X11_accum");
    static hls::stream<float> X11_out("X11_out");
	
	static hls::stream<v_dt> A12_in("A12_in");
    static hls::stream<v_dt> X12_in("X12_in");
    static hls::stream<v_dt> X12_scal("X12_scal");
    static hls::stream<v_dt> X12_accum("X12_accum");
    static hls::stream<float> X12_out("X12_out");

	static hls::stream<v_dt> A13_in("A13_in");
    static hls::stream<v_dt> X13_in("X13_in");
    static hls::stream<v_dt> X13_scal("X13_scal");
    static hls::stream<v_dt> X13_accum("X13_accum");
    static hls::stream<float> X13_out("X13_out");

	static hls::stream<v_dt> A14_in("A14_in");
    static hls::stream<v_dt> X14_in("X14_in");
    static hls::stream<v_dt> X14_scal("X14_scal");
    static hls::stream<v_dt> X14_accum("X14_accum");
    static hls::stream<float> X14_out("X14_out");        
	
	const int break_line1 = break_row2-break_row1;
	const int break_line2 = break_row3-break_row2;
	const int break_line3 = break_row4-break_row3;
	const int break_line4 = break_row5-break_row4;
	const int break_line5 = break_row6-break_row5;
	const int break_line6 = break_row7-break_row6;
	const int break_line7 = break_row8-break_row7;
	const int break_line8 = break_row9-break_row8;
	const int break_line9 = break_row10-break_row9;
	const int break_line10 = break_row11-break_row10;
	const int break_line11 = break_row12-break_row11;
	const int break_line12 = break_row13-break_row12;
	const int break_line13 = N-break_row13;



	#pragma HLS STREAM variable = A1_in depth = 2048
	#pragma HLS STREAM variable = X1_in depth = 2048
	#pragma HLS STREAM variable = X1_scal depth = 2048
	#pragma HLS STREAM variable = X1_accum depth = 2048
	#pragma HLS STREAM variable = X1_out depth = 2048

	#pragma HLS STREAM variable = A2_in depth = 2048
	#pragma HLS STREAM variable = X2_in depth = 2048
	#pragma HLS STREAM variable = X2_scal depth = 2048
	#pragma HLS STREAM variable = X2_accum depth = 2048
	#pragma HLS STREAM variable = X2_out depth = 2048

	#pragma HLS STREAM variable = A3_in depth = 2048
	#pragma HLS STREAM variable = X3_in depth = 2048
	#pragma HLS STREAM variable = X3_scal depth = 2048
	#pragma HLS STREAM variable = X3_accum depth = 2048
	#pragma HLS STREAM variable = X3_out depth = 2048

	#pragma HLS STREAM variable = A4_in depth = 2048
	#pragma HLS STREAM variable = X4_in depth = 2048
	#pragma HLS STREAM variable = X4_scal depth = 2048
	#pragma HLS STREAM variable = X4_accum depth = 2048
	#pragma HLS STREAM variable = X4_out depth = 2048

	#pragma HLS STREAM variable = A5_in depth = 2048
    #pragma HLS STREAM variable = X5_in depth = 2048
    #pragma HLS STREAM variable = X5_scal depth = 2048
    #pragma HLS STREAM variable = X5_accum depth = 2048
    #pragma HLS STREAM variable = X5_out depth = 2048

	#pragma HLS STREAM variable = A6_in depth = 2048
    #pragma HLS STREAM variable = X6_in depth = 2048
    #pragma HLS STREAM variable = X6_scal depth = 2048
    #pragma HLS STREAM variable = X6_accum depth = 2048
    #pragma HLS STREAM variable = X6_out depth = 2048

	#pragma HLS STREAM variable = A7_in depth = 2048
    #pragma HLS STREAM variable = X7_in depth = 2048
    #pragma HLS STREAM variable = X7_scal depth = 2048
    #pragma HLS STREAM variable = X7_accum depth = 2048
    #pragma HLS STREAM variable = X7_out depth = 2048

	#pragma HLS STREAM variable = A8_in depth = 2048
    #pragma HLS STREAM variable = X8_in depth = 2048
    #pragma HLS STREAM variable = X8_scal depth = 2048
    #pragma HLS STREAM variable = X8_accum depth = 2048
    #pragma HLS STREAM variable = X8_out depth = 2048

	#pragma HLS STREAM variable = A9_in depth = 2048
    #pragma HLS STREAM variable = X9_in depth = 2048
    #pragma HLS STREAM variable = X9_scal depth = 2048
    #pragma HLS STREAM variable = X9_accum depth = 2048
    #pragma HLS STREAM variable = X9_out depth = 2048

	#pragma HLS STREAM variable = A10_in depth = 2048
    #pragma HLS STREAM variable = X10_in depth = 2048
    #pragma HLS STREAM variable = X10_scal depth = 2048
    #pragma HLS STREAM variable = X10_accum depth = 2048
    #pragma HLS STREAM variable = X10_out depth = 2048

	#pragma HLS STREAM variable = A11_in depth = 2048
    #pragma HLS STREAM variable = X11_in depth = 2048
    #pragma HLS STREAM variable = X11_scal depth = 2048
    #pragma HLS STREAM variable = X11_accum depth = 2048
    #pragma HLS STREAM variable = X11_out depth = 2048

	#pragma HLS STREAM variable = A12_in depth = 2048
    #pragma HLS STREAM variable = X12_in depth = 2048
    #pragma HLS STREAM variable = X12_scal depth = 2048
    #pragma HLS STREAM variable = X12_accum depth = 2048
    #pragma HLS STREAM variable = X12_out depth = 2048

 	#pragma HLS STREAM variable = A13_in depth = 2048
    #pragma HLS STREAM variable = X13_in depth = 2048
    #pragma HLS STREAM variable = X13_scal depth = 2048
    #pragma HLS STREAM variable = X13_accum depth = 2048
    #pragma HLS STREAM variable = X13_out depth = 2048
    
	#pragma HLS STREAM variable = A14_in depth = 2048
    #pragma HLS STREAM variable = X14_in depth = 2048
    #pragma HLS STREAM variable = X14_scal depth = 2048
    #pragma HLS STREAM variable = X14_accum depth = 2048
    #pragma HLS STREAM variable = X14_out depth = 2048       

	#pragma HLS dataflow

	read_x(Uplo,X1_in,X_in1,N,0,break_row1);
	read_x(Uplo,X2_in,X_in2,N,break_row1,break_line1);
	read_x(Uplo,X3_in,X_in3,N,break_row2,break_line2);
	read_x(Uplo,X4_in,X_in4,N,break_row3,break_line3);
	read_x(Uplo,X5_in,X_in5,N,break_row4,break_line4);
	read_x(Uplo,X6_in,X_in6,N,break_row5,break_line5);
	read_x(Uplo,X7_in,X_in7,N,break_row6,break_line6);
	read_x(Uplo,X8_in,X_in8,N,break_row7,break_line7);
	read_x(Uplo,X9_in,X_in9,N,break_row8,break_line8);
	read_x(Uplo,X10_in,X_in10,N,break_row9,break_line9);
	read_x(Uplo,X11_in,X_in11,N,break_row10,break_line10);
	read_x(Uplo,X12_in,X_in12,N,break_row11,break_line11);
	read_x(Uplo,X13_in,X_in13,N,break_row12,break_line12);
	read_x(Uplo,X14_in,X_in14,N,break_row13,break_line13);


	read_matrix(Uplo,A1,A1_in,N,0,break_row1);
	read_matrix(Uplo,A2,A2_in,N,break_row1,break_line1);
	read_matrix(Uplo,A3,A3_in,N,break_row2,break_line2);
	read_matrix(Uplo,A4,A4_in,N,break_row3,break_line3);
	read_matrix(Uplo,A5,A5_in,N,break_row4,break_line4);
	read_matrix(Uplo,A6,A6_in,N,break_row5,break_line5);
	read_matrix(Uplo,A7,A7_in,N,break_row6,break_line6);
	read_matrix(Uplo,A8,A8_in,N,break_row7,break_line7);
	read_matrix(Uplo,A9,A9_in,N,break_row8,break_line8);
	read_matrix(Uplo,A10,A10_in,N,break_row9,break_line9);
	read_matrix(Uplo,A11,A11_in,N,break_row10,break_line10);
	read_matrix(Uplo,A12,A12_in,N,break_row11,break_line11);
	read_matrix(Uplo,A13,A13_in,N,break_row12,break_line12);
	read_matrix(Uplo,A14,A14_in,N,break_row13,break_line13);


	scal_ax(Uplo,X1_in,A1_in,X1_scal,N,0,break_row1);
	scal_ax(Uplo,X2_in,A2_in,X2_scal,N,break_row1,break_line1);
	scal_ax(Uplo,X3_in,A3_in,X3_scal,N,break_row2,break_line2);
	scal_ax(Uplo,X4_in,A4_in,X4_scal,N,break_row3,break_line3);
	scal_ax(Uplo,X5_in,A5_in,X5_scal,N,break_row4,break_line4);
	scal_ax(Uplo,X6_in,A6_in,X6_scal,N,break_row5,break_line5);
	scal_ax(Uplo,X7_in,A7_in,X7_scal,N,break_row6,break_line6);
	scal_ax(Uplo,X8_in,A8_in,X8_scal,N,break_row7,break_line7);
	scal_ax(Uplo,X9_in,A9_in,X9_scal,N,break_row8,break_line8);
	scal_ax(Uplo,X10_in,A10_in,X10_scal,N,break_row9,break_line9);
	scal_ax(Uplo,X11_in,A11_in,X11_scal,N,break_row10,break_line10);
	scal_ax(Uplo,X12_in,A12_in,X12_scal,N,break_row11,break_line11);
	scal_ax(Uplo,X13_in,A13_in,X13_scal,N,break_row12,break_line12);
	scal_ax(Uplo,X14_in,A14_in,X14_scal,N,break_row13,break_line13);


	accum(Uplo,X1_scal,X1_accum,N,0,break_row1);
	accum(Uplo,X2_scal,X2_accum,N,break_row1,break_line1);
	accum(Uplo,X3_scal,X3_accum,N,break_row2,break_line2);
	accum(Uplo,X4_scal,X4_accum,N,break_row3,break_line3);
	accum(Uplo,X5_scal,X5_accum,N,break_row4,break_line4);
	accum(Uplo,X6_scal,X6_accum,N,break_row5,break_line5);
	accum(Uplo,X7_scal,X7_accum,N,break_row6,break_line6);
	accum(Uplo,X8_scal,X8_accum,N,break_row7,break_line7);
	accum(Uplo,X9_scal,X9_accum,N,break_row8,break_line8);
	accum(Uplo,X10_scal,X10_accum,N,break_row9,break_line9);
	accum(Uplo,X11_scal,X11_accum,N,break_row10,break_line10);
	accum(Uplo,X12_scal,X12_accum,N,break_row11,break_line11);
	accum(Uplo,X13_scal,X13_accum,N,break_row12,break_line12);
	accum(Uplo,X14_scal,X14_accum,N,break_row13,break_line13);


	reduction(X1_accum,X1_out,N,break_row1);
	reduction(X2_accum,X2_out,N,break_line1);
	reduction(X3_accum,X3_out,N,break_line2);
	reduction(X4_accum,X4_out,N,break_line3);
	reduction(X5_accum,X5_out,N,break_line4);
	reduction(X6_accum,X6_out,N,break_line5);
	reduction(X7_accum,X7_out,N,break_line6);
	reduction(X8_accum,X8_out,N,break_line7);
	reduction(X9_accum,X9_out,N,break_line8);
	reduction(X10_accum,X10_out,N,break_line9);
	reduction(X11_accum,X11_out,N,break_line10);
	reduction(X12_accum,X12_out,N,break_line11);
	reduction(X13_accum,X13_out,N,break_line12);
	reduction(X14_accum,X14_out,N,break_line13);


	write_x(X1_out,X_out1,N,break_row1);
	write_x(X2_out,X_out2,N,break_line1);
	write_x(X3_out,X_out3,N,break_line2);
	write_x(X4_out,X_out4,N,break_line3);
	write_x(X5_out,X_out5,N,break_line4);
	write_x(X6_out,X_out6,N,break_line5);
	write_x(X7_out,X_out7,N,break_line6);
	write_x(X8_out,X_out8,N,break_line7);
	write_x(X9_out,X_out9,N,break_line8);
	write_x(X10_out,X_out10,N,break_line9);
	write_x(X11_out,X_out11,N,break_line10);
	write_x(X12_out,X_out12,N,break_line11);
	write_x(X13_out,X_out13,N,break_line12);
	write_x(X14_out,X_out14,N,break_line13);


}}
