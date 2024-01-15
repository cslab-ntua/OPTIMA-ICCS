#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define BRAM_SIZE 1024
#define VDATA_SIZE 16
#define CUs 16

typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

//4 COMPUTE UNITS
void read_b(hls::stream<float>& Bstream1, hls::stream<float>& Bstream2, hls::stream<float>& Bstream3, hls::stream<float>& Bstream4,
		hls::stream<float>& Bstream5, hls::stream<float>& Bstream6, hls::stream<float>& Bstream7, hls::stream<float>& Bstream8, hls::stream<float>& Bstream9,
		hls::stream<float>& Bstream10, hls::stream<float>& Bstream11, hls::stream<float>& Bstream12, hls::stream<float>& Bstream13, hls::stream<float>& Bstream14,
		hls::stream<float>& Bstream15, hls::stream<float>& Bstream16, const float *B, const int N){

	read_b: for(int i=0;i<N;i++){
		#pragma HLS pipeline II=1
		if(i<1*N/CUs)
			Bstream1 << B[i];
		else if(i<2*N/CUs and i>= 1*N/CUs)
			Bstream2 << B[i];
		else if(i<3*N/CUs and i>= 2*N/CUs)
			Bstream3 << B[i];
		else if(i<4*N/CUs and i>= 3*N/CUs)
			Bstream4 << B[i];
		else if(i<5*N/CUs and i>= 4*N/CUs)
			Bstream5 << B[i];
		else if(i<6*N/CUs and i>= 5*N/CUs)
			Bstream6 << B[i];
		else if(i<7*N/CUs and i>= 6*N/CUs)
			Bstream7 << B[i];
		else if(i<8*N/CUs and i>= 7*N/CUs)
			Bstream8 << B[i];
		else if(i<9*N/CUs and i>= 8*N/CUs)
			Bstream9 << B[i];
		else if(i<10*N/CUs and i>= 9*N/CUs)
			Bstream10 << B[i];
		else if(i<11*N/CUs and i>= 10*N/CUs)
			Bstream11 << B[i];
		else if(i<12*N/CUs and i>= 11*N/CUs)
			Bstream12 << B[i];
		else if(i<13*N/CUs and i>= 12*N/CUs)
			Bstream13 << B[i];
		else if(i<14*N/CUs and i>= 13*N/CUs)
			Bstream14 << B[i];
		else if(i<15*N/CUs and i>= 14*N/CUs)
			Bstream15 << B[i];
		else if(i<16*N/CUs and i>= 15*N/CUs)
			Bstream16 << B[i];
	}
}

void write_x(hls::stream<float>& Xstream1, hls::stream<float>& Xstream2, hls::stream<float>& Xstream3, hls::stream<float>& Xstream4,
		hls::stream<float>& Xstream5, hls::stream<float>& Xstream6, hls::stream<float>& Xstream7, hls::stream<float>& Xstream8, hls::stream<float>& Xstream9,
		hls::stream<float>& Xstream10, hls::stream<float>& Xstream11, hls::stream<float>& Xstream12, hls::stream<float>& Xstream13, hls::stream<float>& Xstream14,
		hls::stream<float>& Xstream15, hls::stream<float>& Xstream16, float *X, const int N){

	write_x: for(int i=0;i<N;i++){
		#pragma HLS pipeline II=1
		if(i<1*N/CUs)
			X[i] = Xstream1.read();
		else if(i<2*N/CUs and i>= 1*N/CUs)
			X[i] = Xstream2.read();
		else if(i<3*N/CUs and i>= 2*N/CUs)
			X[i] = Xstream3.read();
		else if(i<4*N/CUs and i>= 3*N/CUs)
			X[i] = Xstream4.read();
		else if(i<5*N/CUs and i>= 4*N/CUs)
			X[i] = Xstream5.read();
		else if(i<6*N/CUs and i>= 5*N/CUs)
			X[i] = Xstream6.read();
		else if(i<7*N/CUs and i>= 6*N/CUs)
			X[i] = Xstream7.read();
		else if(i<8*N/CUs and i>= 7*N/CUs)
			X[i] = Xstream8.read();
		else if(i<9*N/CUs and i>= 8*N/CUs)
			X[i] = Xstream9.read();
		else if(i<10*N/CUs and i>= 9*N/CUs)
			X[i] = Xstream10.read();
		else if(i<11*N/CUs and i>= 10*N/CUs)
			X[i] = Xstream11.read();
		else if(i<12*N/CUs and i>= 11*N/CUs)
			X[i] = Xstream12.read();
		else if(i<13*N/CUs and i>= 12*N/CUs)
			X[i] = Xstream13.read();
		else if(i<14*N/CUs and i>= 13*N/CUs)
			X[i] = Xstream14.read();
		else if(i<15*N/CUs and i>= 14*N/CUs)
			X[i] = Xstream15.read();
		else if(i<16*N/CUs and i>= 15*N/CUs)
			X[i] = Xstream16.read();
	}
}

void read_matrix(const v_dt *A, hls::stream<v_dt>& AStream, const int N, const int K, const int start, const int end){

	int offset = end, j=0;
	int N_cols = K/16 + 1;

	int i = (K<=N/CUs) ? start : hls::fmax(0,start);

	wide_read1: while(i<offset){
		#pragma HLS pipeline II=1

		AStream << A[i*N_cols+j];

		if(j==K/VDATA_SIZE){
			i++;
			j=0;
		}
		else{
			j++;
		}
	}
}

void tbsv_sync1(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_2,
		hls::stream<float>& Bcast_1_to_3, hls::stream<float>& Bcast_1_to_4, hls::stream<float>& Bcast_1_to_5, hls::stream<float>& Bcast_1_to_6,
		hls::stream<float>& Bcast_1_to_7, hls::stream<float>& Bcast_1_to_8, hls::stream<float>& Bcast_1_to_9, hls::stream<float>& Bcast_1_to_10,
		hls::stream<float>& Bcast_1_to_11, hls::stream<float>& Bcast_1_to_12, hls::stream<float>& Bcast_1_to_13, hls::stream<float>& Bcast_1_to_14,
		hls::stream<float>& Bcast_1_to_15, hls::stream<float>& Bcast_1_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;
	int offset1_2 = hls::fmax(1*N/CUs-K,0);
	int offset1_3 = hls::fmax(2*N/CUs-K,0);
	int offset1_4 = hls::fmax(3*N/CUs-K,0);
	int offset1_5 = hls::fmax(4*N/CUs-K,0);
	int offset1_6 = hls::fmax(5*N/CUs-K,0);
	int offset1_7 = hls::fmax(6*N/CUs-K,0);
	int offset1_8 = hls::fmax(7*N/CUs-K,0);
	int offset1_9 = hls::fmax(8*N/CUs-K,0);
	int offset1_10 = hls::fmax(9*N/CUs-K,0);
	int offset1_11 = hls::fmax(10*N/CUs-K,0);
	int offset1_12 = hls::fmax(11*N/CUs-K,0);
	int offset1_13 = hls::fmax(12*N/CUs-K,0);
	int offset1_14 = hls::fmax(13*N/CUs-K,0);
	int offset1_15 = hls::fmax(14*N/CUs-K,0);
	int offset1_16 = hls::fmax(15*N/CUs-K,0);

	trsv_sync1_loop_i__: for(int i=0;i<N/CUs;i++){
		float Xtemp=0;
		if(i%2==0)
			Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
		else
			Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];

		int j=0;
		trsv_sync1_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			j+=VDATA_SIZE;
		}

		XStream << Xtemp;
		if(i>=offset1_2)
			Bcast_1_to_2 << Xtemp;
		if(i>=offset1_3)
			Bcast_1_to_3 << Xtemp;
		if(i>=offset1_4)
			Bcast_1_to_4 << Xtemp;
		if(i>=offset1_5)
			Bcast_1_to_5 << Xtemp;
		if(i>=offset1_6)
			Bcast_1_to_6 << Xtemp;
		if(i>=offset1_7)
			Bcast_1_to_7 << Xtemp;
		if(i>=offset1_8)
			Bcast_1_to_8 << Xtemp;
		if(i>=offset1_9)
			Bcast_1_to_9 << Xtemp;
		if(i>=offset1_10)
			Bcast_1_to_10 << Xtemp;
		if(i>=offset1_11)
			Bcast_1_to_11 << Xtemp;
		if(i>=offset1_12)
			Bcast_1_to_12 << Xtemp;
		if(i>=offset1_13)
			Bcast_1_to_13 << Xtemp;
		if(i>=offset1_14)
			Bcast_1_to_14 << Xtemp;
		if(i>=offset1_15)
			Bcast_1_to_15 << Xtemp;
		if(i>=offset1_16)
			Bcast_1_to_16 << Xtemp;
	}
}

void tbsv_sync2(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_2,
		hls::stream<float>& Bcast_2_to_3, hls::stream<float>& Bcast_2_to_4, hls::stream<float>& Bcast_2_to_5, hls::stream<float>& Bcast_2_to_6,
		hls::stream<float>& Bcast_2_to_7, hls::stream<float>& Bcast_2_to_8, hls::stream<float>& Bcast_2_to_9, hls::stream<float>& Bcast_2_to_10,
		hls::stream<float>& Bcast_2_to_11, hls::stream<float>& Bcast_2_to_12, hls::stream<float>& Bcast_2_to_13, hls::stream<float>& Bcast_2_to_14,
		hls::stream<float>& Bcast_2_to_15, hls::stream<float>& Bcast_2_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,2);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_2 = (K<=N/CUs) ? K : N/CUs;

	int offset2_3 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset2_4 = (K<=N/CUs) ? end : end-(K-N/CUs);
	int offset2_5 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset2_6 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset2_7 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset2_8 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset2_9 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset2_10 = (K<=N/CUs) ? end : end-(K-7*N/CUs);
	int offset2_11 = (K<=N/CUs) ? end : end-(K-8*N/CUs);
	int offset2_12 = (K<=N/CUs) ? end : end-(K-9*N/CUs);
	int offset2_13 = (K<=N/CUs) ? end : end-(K-10*N/CUs);
	int offset2_14 = (K<=N/CUs) ? end : end-(K-11*N/CUs);
	int offset2_15 = (K<=N/CUs) ? end : end-(K-12*N/CUs);
	int offset2_16 = (K<=N/CUs) ? end : end-(K-13*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_2)
			Xtemp = Bcast_1_to_2.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset2_3)
			Bcast_2_to_3 << Xtemp;
		if(i>=offset2_4)
			Bcast_2_to_4 << Xtemp;
		if(i>=offset2_5)
			Bcast_2_to_5 << Xtemp;
		if(i>=offset2_6)
			Bcast_2_to_6 << Xtemp;
		if(i>=offset2_7)
			Bcast_2_to_7 << Xtemp;
		if(i>=offset2_8)
			Bcast_2_to_8 << Xtemp;
		if(i>=offset2_9)
			Bcast_2_to_9 << Xtemp;
		if(i>=offset2_10)
			Bcast_2_to_10 << Xtemp;
		if(i>=offset2_11)
			Bcast_2_to_11 << Xtemp;
		if(i>=offset2_12)
			Bcast_2_to_12 << Xtemp;
		if(i>=offset2_13)
			Bcast_2_to_13 << Xtemp;
		if(i>=offset2_14)
			Bcast_2_to_14 << Xtemp;
		if(i>=offset2_15)
			Bcast_2_to_15 << Xtemp;
		if(i>=offset2_16)
			Bcast_2_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync3(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_3,
		hls::stream<float>& Bcast_2_to_3, hls::stream<float>& Bcast_3_to_4, hls::stream<float>& Bcast_3_to_5, hls::stream<float>& Bcast_3_to_6,
		hls::stream<float>& Bcast_3_to_7, hls::stream<float>& Bcast_3_to_8, hls::stream<float>& Bcast_3_to_9, hls::stream<float>& Bcast_3_to_10,
		hls::stream<float>& Bcast_3_to_11, hls::stream<float>& Bcast_3_to_12, hls::stream<float>& Bcast_3_to_13, hls::stream<float>& Bcast_3_to_14,
		hls::stream<float>& Bcast_3_to_15, hls::stream<float>& Bcast_3_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,3);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_3 = (K<=N/CUs) ? 0 : K-N/CUs;
	int offset2_3 = (K<=N/CUs) ? K : N/CUs+offset1_3;

	int offset3_4 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset3_5 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset3_6 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset3_7 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset3_8 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset3_9 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset3_10 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset3_11 = (K<=N/CUs) ? end : end-(K-7*N/CUs);
	int offset3_12 = (K<=N/CUs) ? end : end-(K-8*N/CUs);
	int offset3_13 = (K<=N/CUs) ? end : end-(K-9*N/CUs);
	int offset3_14 = (K<=N/CUs) ? end : end-(K-10*N/CUs);
	int offset3_15 = (K<=N/CUs) ? end : end-(K-11*N/CUs);
	int offset3_16 = (K<=N/CUs) ? end : end-(K-12*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){
		float Xtemp=0;
		if(i<offset1_3)
			Xtemp = Bcast_1_to_3.read();
		else if(i<offset2_3)
			Xtemp = Bcast_2_to_3.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset3_4)
			Bcast_3_to_4 << Xtemp;
		if(i>=offset3_5)
			Bcast_3_to_5 << Xtemp;
		if(i>=offset3_6)
			Bcast_3_to_6 << Xtemp;
		if(i>=offset3_7)
			Bcast_3_to_7 << Xtemp;
		if(i>=offset3_8)
			Bcast_3_to_8 << Xtemp;
		if(i>=offset3_9)
			Bcast_3_to_9 << Xtemp;
		if(i>=offset3_10)
			Bcast_3_to_10 << Xtemp;
		if(i>=offset3_11)
			Bcast_3_to_11 << Xtemp;
		if(i>=offset3_12)
			Bcast_3_to_12 << Xtemp;
		if(i>=offset3_13)
			Bcast_3_to_13 << Xtemp;
		if(i>=offset3_14)
			Bcast_3_to_14 << Xtemp;
		if(i>=offset3_15)
			Bcast_3_to_15 << Xtemp;
		if(i>=offset3_16)
			Bcast_3_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync4(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_4,
		hls::stream<float>& Bcast_2_to_4, hls::stream<float>& Bcast_3_to_4, hls::stream<float>& Bcast_4_to_5, hls::stream<float>& Bcast_4_to_6,
		hls::stream<float>& Bcast_4_to_7, hls::stream<float>& Bcast_4_to_8, hls::stream<float>& Bcast_4_to_9, hls::stream<float>& Bcast_4_to_10,
		hls::stream<float>& Bcast_4_to_11, hls::stream<float>& Bcast_4_to_12, hls::stream<float>& Bcast_4_to_13, hls::stream<float>& Bcast_4_to_14,
		hls::stream<float>& Bcast_4_to_15, hls::stream<float>& Bcast_4_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,4);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_4 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs,0);
	int offset2_4 = (K<=N/CUs) ? 0 : hls::fmax(K-N/CUs+offset1_4,0);
	int offset3_4 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset2_4,0);

	int offset4_5 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset4_6 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset4_7 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset4_8 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset4_9 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset4_10 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset4_11 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset4_12 = (K<=N/CUs) ? end : end-(K-7*N/CUs);
	int offset4_13 = (K<=N/CUs) ? end : end-(K-8*N/CUs);
	int offset4_14 = (K<=N/CUs) ? end : end-(K-9*N/CUs);
	int offset4_15 = (K<=N/CUs) ? end : end-(K-10*N/CUs);
	int offset4_16 = (K<=N/CUs) ? end : end-(K-11*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_4)
			Xtemp = Bcast_1_to_4.read();
		else if(i<offset2_4)
			Xtemp = Bcast_2_to_4.read();
		else if(i<offset3_4)
			Xtemp = Bcast_3_to_4.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset4_5)
			Bcast_4_to_5 << Xtemp;
		if(i>=offset4_6)
			Bcast_4_to_6 << Xtemp;
		if(i>=offset4_7)
			Bcast_4_to_7 << Xtemp;
		if(i>=offset4_8)
			Bcast_4_to_8 << Xtemp;
		if(i>=offset4_9)
			Bcast_4_to_9 << Xtemp;
		if(i>=offset4_10)
			Bcast_4_to_10 << Xtemp;
		if(i>=offset4_11)
			Bcast_4_to_11 << Xtemp;
		if(i>=offset4_12)
			Bcast_4_to_12 << Xtemp;
		if(i>=offset4_13)
			Bcast_4_to_13 << Xtemp;
		if(i>=offset4_14)
			Bcast_4_to_14 << Xtemp;
		if(i>=offset4_15)
			Bcast_4_to_15 << Xtemp;
		if(i>=offset4_16)
			Bcast_4_to_16 << Xtemp;


		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync5(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_5,
		hls::stream<float>& Bcast_2_to_5, hls::stream<float>& Bcast_3_to_5, hls::stream<float>& Bcast_4_to_5, hls::stream<float>& Bcast_5_to_6,
		hls::stream<float>& Bcast_5_to_7, hls::stream<float>& Bcast_5_to_8, hls::stream<float>& Bcast_5_to_9, hls::stream<float>& Bcast_5_to_10,
		hls::stream<float>& Bcast_5_to_11, hls::stream<float>& Bcast_5_to_12, hls::stream<float>& Bcast_5_to_13, hls::stream<float>& Bcast_5_to_14,
		hls::stream<float>& Bcast_5_to_15, hls::stream<float>& Bcast_5_to_16,const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,5);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_5 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs,0);
	int offset2_5 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset1_5,0);
	int offset3_5 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset2_5,0);
	int offset4_5 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset3_5,0);

	int offset5_6 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset5_7 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset5_8 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset5_9 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset5_10 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset5_11 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset5_12 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset5_13 = (K<=N/CUs) ? end : end-(K-7*N/CUs);
	int offset5_14 = (K<=N/CUs) ? end : end-(K-8*N/CUs);
	int offset5_15 = (K<=N/CUs) ? end : end-(K-9*N/CUs);
	int offset5_16 = (K<=N/CUs) ? end : end-(K-10*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_5)
			Xtemp = Bcast_1_to_5.read();
		else if(i<offset2_5)
			Xtemp = Bcast_2_to_5.read();
		else if(i<offset3_5)
			Xtemp = Bcast_3_to_5.read();
		else if(i<offset4_5)
			Xtemp = Bcast_4_to_5.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset5_6)
			Bcast_5_to_6 << Xtemp;
		if(i>=offset5_7)
			Bcast_5_to_7 << Xtemp;
		if(i>=offset5_8)
			Bcast_5_to_8 << Xtemp;
		if(i>=offset5_9)
			Bcast_5_to_9 << Xtemp;
		if(i>=offset5_10)
			Bcast_5_to_10 << Xtemp;
		if(i>=offset5_11)
			Bcast_5_to_11 << Xtemp;
		if(i>=offset5_12)
			Bcast_5_to_12 << Xtemp;
		if(i>=offset5_13)
			Bcast_5_to_13 << Xtemp;
		if(i>=offset5_14)
			Bcast_5_to_14 << Xtemp;
		if(i>=offset5_15)
			Bcast_5_to_15 << Xtemp;
		if(i>=offset5_16)
			Bcast_5_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync6(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_6,
		hls::stream<float>& Bcast_2_to_6, hls::stream<float>& Bcast_3_to_6, hls::stream<float>& Bcast_4_to_6, hls::stream<float>& Bcast_5_to_6,
		hls::stream<float>& Bcast_6_to_7, hls::stream<float>& Bcast_6_to_8, hls::stream<float>& Bcast_6_to_9, hls::stream<float>& Bcast_6_to_10,
		hls::stream<float>& Bcast_6_to_11, hls::stream<float>& Bcast_6_to_12, hls::stream<float>& Bcast_6_to_13, hls::stream<float>& Bcast_6_to_14,
		hls::stream<float>& Bcast_6_to_15, hls::stream<float>& Bcast_6_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,6);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_6 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs,0);
	int offset2_6 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset1_6,0);
	int offset3_6 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset2_6,0);
	int offset4_6 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset3_6,0);
	int offset5_6 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset4_6,0);

	int offset6_7 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset6_8 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset6_9 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset6_10 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset6_11 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset6_12 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset6_13 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset6_14 = (K<=N/CUs) ? end : end-(K-7*N/CUs);
	int offset6_15 = (K<=N/CUs) ? end : end-(K-8*N/CUs);
	int offset6_16 = (K<=N/CUs) ? end : end-(K-9*N/CUs);


	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_6)
			Xtemp = Bcast_1_to_6.read();
		else if(i<offset2_6)
			Xtemp = Bcast_2_to_6.read();
		else if(i<offset3_6)
			Xtemp = Bcast_3_to_6.read();
		else if(i<offset4_6)
			Xtemp = Bcast_4_to_6.read();
		else if(i<offset5_6)
			Xtemp = Bcast_5_to_6.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset6_7)
			Bcast_6_to_7 << Xtemp;
		if(i>=offset6_8)
			Bcast_6_to_8 << Xtemp;
		if(i>=offset6_9)
			Bcast_6_to_9 << Xtemp;
		if(i>=offset6_10)
			Bcast_6_to_10 << Xtemp;
		if(i>=offset6_11)
			Bcast_6_to_11 << Xtemp;
		if(i>=offset6_12)
			Bcast_6_to_12 << Xtemp;
		if(i>=offset6_13)
			Bcast_6_to_13 << Xtemp;
		if(i>=offset6_14)
			Bcast_6_to_14 << Xtemp;
		if(i>=offset6_15)
			Bcast_6_to_15 << Xtemp;
		if(i>=offset6_16)
			Bcast_6_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync7(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_7,
		hls::stream<float>& Bcast_2_to_7, hls::stream<float>& Bcast_3_to_7, hls::stream<float>& Bcast_4_to_7, hls::stream<float>& Bcast_5_to_7,
		hls::stream<float>& Bcast_6_to_7, hls::stream<float>& Bcast_7_to_8, hls::stream<float>& Bcast_7_to_9, hls::stream<float>& Bcast_7_to_10,
		hls::stream<float>& Bcast_7_to_11, hls::stream<float>& Bcast_7_to_12, hls::stream<float>& Bcast_7_to_13, hls::stream<float>& Bcast_7_to_14,
		hls::stream<float>& Bcast_7_to_15, hls::stream<float>& Bcast_7_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,7);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_7 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs,0);
	int offset2_7 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset1_7,0);
	int offset3_7 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset2_7,0);
	int offset4_7 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset3_7,0);
	int offset5_7 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset4_7,0);
	int offset6_7 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset5_7,0);

	int offset7_8 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset7_9 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset7_10 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset7_11 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset7_12 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset7_13 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset7_14 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset7_15 = (K<=N/CUs) ? end : end-(K-7*N/CUs);
	int offset7_16 = (K<=N/CUs) ? end : end-(K-8*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_7)
			Xtemp = Bcast_1_to_7.read();
		else if(i<offset2_7)
			Xtemp = Bcast_2_to_7.read();
		else if(i<offset3_7)
			Xtemp = Bcast_3_to_7.read();
		else if(i<offset4_7)
			Xtemp = Bcast_4_to_7.read();
		else if(i<offset5_7)
			Xtemp = Bcast_5_to_7.read();
		else if(i<offset6_7)
			Xtemp = Bcast_6_to_7.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset7_8)
			Bcast_7_to_8 << Xtemp;
		if(i>=offset7_9)
			Bcast_7_to_9 << Xtemp;
		if(i>=offset7_10)
			Bcast_7_to_10 << Xtemp;
		if(i>=offset7_11)
			Bcast_7_to_11 << Xtemp;
		if(i>=offset7_12)
			Bcast_7_to_12 << Xtemp;
		if(i>=offset7_13)
			Bcast_7_to_13 << Xtemp;
		if(i>=offset7_14)
			Bcast_7_to_14 << Xtemp;
		if(i>=offset7_15)
			Bcast_7_to_15 << Xtemp;
		if(i>=offset7_16)
			Bcast_7_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync8(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_8,
		hls::stream<float>& Bcast_2_to_8, hls::stream<float>& Bcast_3_to_8, hls::stream<float>& Bcast_4_to_8, hls::stream<float>& Bcast_5_to_8,
		hls::stream<float>& Bcast_6_to_8, hls::stream<float>& Bcast_7_to_8, hls::stream<float>& Bcast_8_to_9, hls::stream<float>& Bcast_8_to_10,
		hls::stream<float>& Bcast_8_to_11, hls::stream<float>& Bcast_8_to_12, hls::stream<float>& Bcast_8_to_13, hls::stream<float>& Bcast_8_to_14,
		hls::stream<float>& Bcast_8_to_15, hls::stream<float>& Bcast_8_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,8);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_8 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs,0);
	int offset2_8 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset1_8,0);
	int offset3_8 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset2_8,0);
	int offset4_8 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset3_8,0);
	int offset5_8 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset4_8,0);
	int offset6_8 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset5_8,0);
	int offset7_8 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset6_8,0);

	int offset8_9 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset8_10 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset8_11 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset8_12 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset8_13 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset8_14 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset8_15 = (K<=N/CUs) ? end : end-(K-6*N/CUs);
	int offset8_16 = (K<=N/CUs) ? end : end-(K-7*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_8)
			Xtemp = Bcast_1_to_8.read();
		else if(i<offset2_8)
			Xtemp = Bcast_2_to_8.read();
		else if(i<offset3_8)
			Xtemp = Bcast_3_to_8.read();
		else if(i<offset4_8)
			Xtemp = Bcast_4_to_8.read();
		else if(i<offset5_8)
			Xtemp = Bcast_5_to_8.read();
		else if(i<offset6_8)
			Xtemp = Bcast_6_to_8.read();
		else if(i<offset7_8)
			Xtemp = Bcast_7_to_8.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset8_9)
			Bcast_8_to_9 << Xtemp;
		if(i>=offset8_10)
			Bcast_8_to_10 << Xtemp;
		if(i>=offset8_11)
			Bcast_8_to_11 << Xtemp;
		if(i>=offset8_12)
			Bcast_8_to_12 << Xtemp;
		if(i>=offset8_13)
			Bcast_8_to_13 << Xtemp;
		if(i>=offset8_14)
			Bcast_8_to_14 << Xtemp;
		if(i>=offset8_15)
			Bcast_8_to_15 << Xtemp;
		if(i>=offset8_16)
			Bcast_8_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync9(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_9,
		hls::stream<float>& Bcast_2_to_9, hls::stream<float>& Bcast_3_to_9, hls::stream<float>& Bcast_4_to_9, hls::stream<float>& Bcast_5_to_9,
		hls::stream<float>& Bcast_6_to_9, hls::stream<float>& Bcast_7_to_9, hls::stream<float>& Bcast_8_to_9, hls::stream<float>& Bcast_9_to_10,
		hls::stream<float>& Bcast_9_to_11, hls::stream<float>& Bcast_9_to_12, hls::stream<float>& Bcast_9_to_13, hls::stream<float>& Bcast_9_to_14,
		hls::stream<float>& Bcast_9_to_15, hls::stream<float>& Bcast_9_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,9);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_9 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs,0);
	int offset2_9 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset1_9,0);
	int offset3_9 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset2_9,0);
	int offset4_9 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset3_9,0);
	int offset5_9 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset4_9,0);
	int offset6_9 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset5_9,0);
	int offset7_9 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset6_9,0);
	int offset8_9 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset7_9,0);

	int offset9_10 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset9_11 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset9_12 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset9_13 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset9_14 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset9_15 = (K<=N/CUs) ? end : end-(K-5*N/CUs);
	int offset9_16 = (K<=N/CUs) ? end : end-(K-6*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_9)
			Xtemp = Bcast_1_to_9.read();
		else if(i<offset2_9)
			Xtemp = Bcast_2_to_9.read();
		else if(i<offset3_9)
			Xtemp = Bcast_3_to_9.read();
		else if(i<offset4_9)
			Xtemp = Bcast_4_to_9.read();
		else if(i<offset5_9)
			Xtemp = Bcast_5_to_9.read();
		else if(i<offset6_9)
			Xtemp = Bcast_6_to_9.read();
		else if(i<offset7_9)
			Xtemp = Bcast_7_to_9.read();
		else if(i<offset8_9)
			Xtemp = Bcast_8_to_9.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset9_10)
			Bcast_9_to_10 << Xtemp;
		if(i>=offset9_11)
			Bcast_9_to_11 << Xtemp;
		if(i>=offset9_12)
			Bcast_9_to_12 << Xtemp;
		if(i>=offset9_13)
			Bcast_9_to_13 << Xtemp;
		if(i>=offset9_14)
			Bcast_9_to_14 << Xtemp;
		if(i>=offset9_15)
			Bcast_9_to_15 << Xtemp;
		if(i>=offset9_16)
			Bcast_9_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync10(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_10,
		hls::stream<float>& Bcast_2_to_10, hls::stream<float>& Bcast_3_to_10, hls::stream<float>& Bcast_4_to_10, hls::stream<float>& Bcast_5_to_10,
		hls::stream<float>& Bcast_6_to_10, hls::stream<float>& Bcast_7_to_10, hls::stream<float>& Bcast_8_to_10, hls::stream<float>& Bcast_9_to_10,
		hls::stream<float>& Bcast_10_to_11, hls::stream<float>& Bcast_10_to_12, hls::stream<float>& Bcast_10_to_13, hls::stream<float>& Bcast_10_to_14,
		hls::stream<float>& Bcast_10_to_15, hls::stream<float>& Bcast_10_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,10);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_10 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs,0);
	int offset2_10 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset1_10,0);
	int offset3_10 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset2_10,0);
	int offset4_10 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset3_10,0);
	int offset5_10 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset4_10,0);
	int offset6_10 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset5_10,0);
	int offset7_10 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset6_10,0);
	int offset8_10 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset7_10,0);
	int offset9_10 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset8_10,0);

	int offset10_11 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset10_12 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset10_13 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset10_14 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset10_15 = (K<=N/CUs) ? end : end-(K-4*N/CUs);
	int offset10_16 = (K<=N/CUs) ? end : end-(K-5*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_10)
			Xtemp = Bcast_1_to_10.read();
		else if(i<offset2_10)
			Xtemp = Bcast_2_to_10.read();
		else if(i<offset3_10)
			Xtemp = Bcast_3_to_10.read();
		else if(i<offset4_10)
			Xtemp = Bcast_4_to_10.read();
		else if(i<offset5_10)
			Xtemp = Bcast_5_to_10.read();
		else if(i<offset6_10)
			Xtemp = Bcast_6_to_10.read();
		else if(i<offset7_10)
			Xtemp = Bcast_7_to_10.read();
		else if(i<offset8_10)
			Xtemp = Bcast_8_to_10.read();
		else if(i<offset9_10)
			Xtemp = Bcast_9_to_10.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset10_11)
			Bcast_10_to_11 << Xtemp;
		if(i>=offset10_12)
			Bcast_10_to_12 << Xtemp;
		if(i>=offset10_13)
			Bcast_10_to_13 << Xtemp;
		if(i>=offset10_14)
			Bcast_10_to_14 << Xtemp;
		if(i>=offset10_15)
			Bcast_10_to_15 << Xtemp;
		if(i>=offset10_16)
			Bcast_10_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync11(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_11,
		hls::stream<float>& Bcast_2_to_11, hls::stream<float>& Bcast_3_to_11, hls::stream<float>& Bcast_4_to_11, hls::stream<float>& Bcast_5_to_11,
		hls::stream<float>& Bcast_6_to_11, hls::stream<float>& Bcast_7_to_11, hls::stream<float>& Bcast_8_to_11, hls::stream<float>& Bcast_9_to_11,
		hls::stream<float>& Bcast_10_to_11, hls::stream<float>& Bcast_11_to_12, hls::stream<float>& Bcast_11_to_13, hls::stream<float>& Bcast_11_to_14,
		hls::stream<float>& Bcast_11_to_15, hls::stream<float>& Bcast_11_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,11);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_11 = (K<=N/CUs) ? 0 : hls::fmax(K-9*N/CUs,0);
	int offset2_11 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs+offset1_11,0);
	int offset3_11 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset2_11,0);
	int offset4_11 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset3_11,0);
	int offset5_11 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset4_11,0);
	int offset6_11 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset5_11,0);
	int offset7_11 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset6_11,0);
	int offset8_11 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset7_11,0);
	int offset9_11 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset8_11,0);
	int offset10_11 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset9_11,0);

	int offset11_12 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset11_13 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset11_14 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset11_15 = (K<=N/CUs) ? end : end-(K-3*N/CUs);
	int offset11_16 = (K<=N/CUs) ? end : end-(K-4*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_11)
			Xtemp = Bcast_1_to_11.read();
		else if(i<offset2_11)
			Xtemp = Bcast_2_to_11.read();
		else if(i<offset3_11)
			Xtemp = Bcast_3_to_11.read();
		else if(i<offset4_11)
			Xtemp = Bcast_4_to_11.read();
		else if(i<offset5_11)
			Xtemp = Bcast_5_to_11.read();
		else if(i<offset6_11)
			Xtemp = Bcast_6_to_11.read();
		else if(i<offset7_11)
			Xtemp = Bcast_7_to_11.read();
		else if(i<offset8_11)
			Xtemp = Bcast_8_to_11.read();
		else if(i<offset9_11)
			Xtemp = Bcast_9_to_11.read();
		else if(i<offset10_11)
			Xtemp = Bcast_10_to_11.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset11_12)
			Bcast_11_to_12 << Xtemp;
		if(i>=offset11_13)
			Bcast_11_to_13 << Xtemp;
		if(i>=offset11_14)
			Bcast_11_to_14 << Xtemp;
		if(i>=offset11_15)
			Bcast_11_to_15 << Xtemp;
		if(i>=offset11_16)
			Bcast_11_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync12(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_12,
		hls::stream<float>& Bcast_2_to_12, hls::stream<float>& Bcast_3_to_12, hls::stream<float>& Bcast_4_to_12, hls::stream<float>& Bcast_5_to_12,
		hls::stream<float>& Bcast_6_to_12, hls::stream<float>& Bcast_7_to_12, hls::stream<float>& Bcast_8_to_12, hls::stream<float>& Bcast_9_to_12,
		hls::stream<float>& Bcast_10_to_12, hls::stream<float>& Bcast_11_to_12, hls::stream<float>& Bcast_12_to_13, hls::stream<float>& Bcast_12_to_14,
		hls::stream<float>& Bcast_12_to_15, hls::stream<float>& Bcast_12_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,12);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_12 = (K<=N/CUs) ? 0 : hls::fmax(K-10*N/CUs,0);
	int offset2_12 = (K<=N/CUs) ? 0 : hls::fmax(K-9*N/CUs+offset1_12,0);
	int offset3_12 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs+offset2_12,0);
	int offset4_12 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset3_12,0);
	int offset5_12 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset4_12,0);
	int offset6_12 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset5_12,0);
	int offset7_12 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset6_12,0);
	int offset8_12 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset7_12,0);
	int offset9_12 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset8_12,0);
	int offset10_12 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset9_12,0);
	int offset11_12 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset10_12,0);

	int offset12_13 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset12_14 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset12_15 = (K<=N/CUs) ? end : end-(K-2*N/CUs);
	int offset12_16 = (K<=N/CUs) ? end : end-(K-3*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_12)
			Xtemp = Bcast_1_to_12.read();
		else if(i<offset2_12)
			Xtemp = Bcast_2_to_12.read();
		else if(i<offset3_12)
			Xtemp = Bcast_3_to_12.read();
		else if(i<offset4_12)
			Xtemp = Bcast_4_to_12.read();
		else if(i<offset5_12)
			Xtemp = Bcast_5_to_12.read();
		else if(i<offset6_12)
			Xtemp = Bcast_6_to_12.read();
		else if(i<offset7_12)
			Xtemp = Bcast_7_to_12.read();
		else if(i<offset8_12)
			Xtemp = Bcast_8_to_12.read();
		else if(i<offset9_12)
			Xtemp = Bcast_9_to_12.read();
		else if(i<offset10_12)
			Xtemp = Bcast_10_to_12.read();
		else if(i<offset11_12)
			Xtemp = Bcast_11_to_12.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset12_13)
			Bcast_12_to_13 << Xtemp;
		if(i>=offset12_14)
			Bcast_12_to_14 << Xtemp;
		if(i>=offset12_15)
			Bcast_12_to_15 << Xtemp;
		if(i>=offset12_16)
			Bcast_12_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync13(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_13,
		hls::stream<float>& Bcast_2_to_13, hls::stream<float>& Bcast_3_to_13, hls::stream<float>& Bcast_4_to_13, hls::stream<float>& Bcast_5_to_13,
		hls::stream<float>& Bcast_6_to_13, hls::stream<float>& Bcast_7_to_13, hls::stream<float>& Bcast_8_to_13, hls::stream<float>& Bcast_9_to_13,
		hls::stream<float>& Bcast_10_to_13, hls::stream<float>& Bcast_11_to_13, hls::stream<float>& Bcast_12_to_13, hls::stream<float>& Bcast_13_to_14,
		hls::stream<float>& Bcast_13_to_15, hls::stream<float>& Bcast_13_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,13);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_13 = (K<=N/CUs) ? 0 : hls::fmax(K-11*N/CUs,0);
	int offset2_13 = (K<=N/CUs) ? 0 : hls::fmax(K-10*N/CUs+offset1_13,0);
	int offset3_13 = (K<=N/CUs) ? 0 : hls::fmax(K-9*N/CUs+offset2_13,0);
	int offset4_13 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs+offset3_13,0);
	int offset5_13 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset4_13,0);
	int offset6_13 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset5_13,0);
	int offset7_13 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset6_13,0);
	int offset8_13 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset7_13,0);
	int offset9_13 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset8_13,0);
	int offset10_13 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset9_13,0);
	int offset11_13 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset10_13,0);
	int offset12_13 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset11_13,0);

	int offset13_14 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset13_15 = (K<=N/CUs) ? end : end-(K-1*N/CUs);
	int offset13_16 = (K<=N/CUs) ? end : end-(K-2*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_13)
			Xtemp = Bcast_1_to_13.read();
		else if(i<offset2_13)
			Xtemp = Bcast_2_to_13.read();
		else if(i<offset3_13)
			Xtemp = Bcast_3_to_13.read();
		else if(i<offset4_13)
			Xtemp = Bcast_4_to_13.read();
		else if(i<offset5_13)
			Xtemp = Bcast_5_to_13.read();
		else if(i<offset6_13)
			Xtemp = Bcast_6_to_13.read();
		else if(i<offset7_13)
			Xtemp = Bcast_7_to_13.read();
		else if(i<offset8_13)
			Xtemp = Bcast_8_to_13.read();
		else if(i<offset9_13)
			Xtemp = Bcast_9_to_13.read();
		else if(i<offset10_13)
			Xtemp = Bcast_10_to_13.read();
		else if(i<offset11_13)
			Xtemp = Bcast_11_to_13.read();
		else if(i<offset12_13)
			Xtemp = Bcast_12_to_13.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset13_14)
			Bcast_13_to_14 << Xtemp;
		if(i>=offset13_15)
			Bcast_13_to_15 << Xtemp;
		if(i>=offset13_16)
			Bcast_13_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync14(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_14,
		hls::stream<float>& Bcast_2_to_14, hls::stream<float>& Bcast_3_to_14, hls::stream<float>& Bcast_4_to_14, hls::stream<float>& Bcast_5_to_14,
		hls::stream<float>& Bcast_6_to_14, hls::stream<float>& Bcast_7_to_14, hls::stream<float>& Bcast_8_to_14, hls::stream<float>& Bcast_9_to_14,
		hls::stream<float>& Bcast_10_to_14, hls::stream<float>& Bcast_11_to_14, hls::stream<float>& Bcast_12_to_14, hls::stream<float>& Bcast_13_to_14,
		hls::stream<float>& Bcast_14_to_15, hls::stream<float>& Bcast_14_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,14);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_14 = (K<=N/CUs) ? 0 : hls::fmax(K-12*N/CUs,0);
	int offset2_14 = (K<=N/CUs) ? 0 : hls::fmax(K-11*N/CUs+offset1_14,0);
	int offset3_14 = (K<=N/CUs) ? 0 : hls::fmax(K-10*N/CUs+offset2_14,0);
	int offset4_14 = (K<=N/CUs) ? 0 : hls::fmax(K-9*N/CUs+offset3_14,0);
	int offset5_14 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs+offset4_14,0);
	int offset6_14 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset5_14,0);
	int offset7_14 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset6_14,0);
	int offset8_14 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset7_14,0);
	int offset9_14 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset8_14,0);
	int offset10_14 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset9_14,0);
	int offset11_14 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset10_14,0);
	int offset12_14 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset11_14,0);
	int offset13_14 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset12_14,0);

	int offset14_15 = (K<=N/CUs) ? end-K : end-N/CUs;
	int offset14_16 = (K<=N/CUs) ? end : end-(K-1*N/CUs);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_14)
			Xtemp = Bcast_1_to_14.read();
		else if(i<offset2_14)
			Xtemp = Bcast_2_to_14.read();
		else if(i<offset3_14)
			Xtemp = Bcast_3_to_14.read();
		else if(i<offset4_14)
			Xtemp = Bcast_4_to_14.read();
		else if(i<offset5_14)
			Xtemp = Bcast_5_to_14.read();
		else if(i<offset6_14)
			Xtemp = Bcast_6_to_14.read();
		else if(i<offset7_14)
			Xtemp = Bcast_7_to_14.read();
		else if(i<offset8_14)
			Xtemp = Bcast_8_to_14.read();
		else if(i<offset9_14)
			Xtemp = Bcast_9_to_14.read();
		else if(i<offset10_14)
			Xtemp = Bcast_10_to_14.read();
		else if(i<offset11_14)
			Xtemp = Bcast_11_to_14.read();
		else if(i<offset12_14)
			Xtemp = Bcast_12_to_14.read();
		else if(i<offset13_14)
			Xtemp = Bcast_13_to_14.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset14_15)
			Bcast_14_to_15 << Xtemp;
		if(i>=offset14_16)
			Bcast_14_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync15(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_15,
		hls::stream<float>& Bcast_2_to_15, hls::stream<float>& Bcast_3_to_15, hls::stream<float>& Bcast_4_to_15, hls::stream<float>& Bcast_5_to_15,
		hls::stream<float>& Bcast_6_to_15, hls::stream<float>& Bcast_7_to_15, hls::stream<float>& Bcast_8_to_15, hls::stream<float>& Bcast_9_to_15,
		hls::stream<float>& Bcast_10_to_15, hls::stream<float>& Bcast_11_to_15, hls::stream<float>& Bcast_12_to_15, hls::stream<float>& Bcast_13_to_15,
		hls::stream<float>& Bcast_14_to_15, hls::stream<float>& Bcast_15_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,15);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_15 = (K<=N/CUs) ? 0 : hls::fmax(K-13*N/CUs,0);
	int offset2_15 = (K<=N/CUs) ? 0 : hls::fmax(K-12*N/CUs+offset1_15,0);
	int offset3_15 = (K<=N/CUs) ? 0 : hls::fmax(K-11*N/CUs+offset2_15,0);
	int offset4_15 = (K<=N/CUs) ? 0 : hls::fmax(K-10*N/CUs+offset3_15,0);
	int offset5_15 = (K<=N/CUs) ? 0 : hls::fmax(K-9*N/CUs+offset4_15,0);
	int offset6_15 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs+offset5_15,0);
	int offset7_15 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset6_15,0);
	int offset8_15 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset7_15,0);
	int offset9_15 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset8_15,0);
	int offset10_15 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset9_15,0);
	int offset11_15 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset10_15,0);
	int offset12_15 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset11_15,0);
	int offset13_15 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset12_15,0);
	int offset14_15 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset13_15,0);

	int offset15_16 = (K<=N/CUs) ? end-K : end-N/CUs;

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_15)
			Xtemp = Bcast_1_to_15.read();
		else if(i<offset2_15)
			Xtemp = Bcast_2_to_15.read();
		else if(i<offset3_15)
			Xtemp = Bcast_3_to_15.read();
		else if(i<offset4_15)
			Xtemp = Bcast_4_to_15.read();
		else if(i<offset5_15)
			Xtemp = Bcast_5_to_15.read();
		else if(i<offset6_15)
			Xtemp = Bcast_6_to_15.read();
		else if(i<offset7_15)
			Xtemp = Bcast_7_to_15.read();
		else if(i<offset8_15)
			Xtemp = Bcast_8_to_15.read();
		else if(i<offset9_15)
			Xtemp = Bcast_9_to_15.read();
		else if(i<offset10_15)
			Xtemp = Bcast_10_to_15.read();
		else if(i<offset11_15)
			Xtemp = Bcast_11_to_15.read();
		else if(i<offset12_15)
			Xtemp = Bcast_12_to_15.read();
		else if(i<offset13_15)
			Xtemp = Bcast_13_to_15.read();
		else if(i<offset14_15)
			Xtemp = Bcast_14_to_15.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset15_16)
			Bcast_15_to_16 << Xtemp;

		if(i>=offset)
			XStream << Xtemp;
	}
}

void tbsv_sync16(hls::stream<float>& BStream, hls::stream<v_dt>& AStream, hls::stream<float>& XStream, hls::stream<float>& Bcast_1_to_16,
		hls::stream<float>& Bcast_2_to_16, hls::stream<float>& Bcast_3_to_16, hls::stream<float>& Bcast_4_to_16, hls::stream<float>& Bcast_5_to_16,
		hls::stream<float>& Bcast_6_to_16, hls::stream<float>& Bcast_7_to_16, hls::stream<float>& Bcast_8_to_16, hls::stream<float>& Bcast_9_to_16,
		hls::stream<float>& Bcast_10_to_16, hls::stream<float>& Bcast_11_to_16, hls::stream<float>& Bcast_12_to_16, hls::stream<float>& Bcast_13_to_16,
		hls::stream<float>& Bcast_14_to_16, hls::stream<float>& Bcast_15_to_16, const int N, const int K){

	float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	/*for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}*/

	int N_cols = (K/16 + 1)*VDATA_SIZE;

	int helper = hls::max(K/(N/CUs)+1,16);
	int end = (K<=N/CUs) ? N/CUs+K : hls::fmin(helper*N/CUs, K+N/CUs);

	int offset = (K<=N/CUs) ? K : end-N/CUs;
	int offset1_16 = (K<=N/CUs) ? 0 : hls::fmax(K-14*N/CUs,0);
	int offset2_16 = (K<=N/CUs) ? 0 : hls::fmax(K-13*N/CUs+offset1_16,0);
	int offset3_16 = (K<=N/CUs) ? 0 : hls::fmax(K-12*N/CUs+offset2_16,0);
	int offset4_16 = (K<=N/CUs) ? 0 : hls::fmax(K-11*N/CUs+offset3_16,0);
	int offset5_16 = (K<=N/CUs) ? 0 : hls::fmax(K-10*N/CUs+offset4_16,0);
	int offset6_16 = (K<=N/CUs) ? 0 : hls::fmax(K-9*N/CUs+offset5_16,0);
	int offset7_16 = (K<=N/CUs) ? 0 : hls::fmax(K-8*N/CUs+offset6_16,0);
	int offset8_16 = (K<=N/CUs) ? 0 : hls::fmax(K-7*N/CUs+offset7_16,0);
	int offset9_16 = (K<=N/CUs) ? 0 : hls::fmax(K-6*N/CUs+offset8_16,0);
	int offset10_16 = (K<=N/CUs) ? 0 : hls::fmax(K-5*N/CUs+offset9_16,0);
	int offset11_16 = (K<=N/CUs) ? 0 : hls::fmax(K-4*N/CUs+offset10_16,0);
	int offset12_16 = (K<=N/CUs) ? 0 : hls::fmax(K-3*N/CUs+offset11_16,0);
	int offset13_16 = (K<=N/CUs) ? 0 : hls::fmax(K-2*N/CUs+offset12_16,0);
	int offset14_16 = (K<=N/CUs) ? 0 : hls::fmax(K-1*N/CUs+offset13_16,0);
	int offset15_16 = (K<=N/CUs) ? K : hls::fmax(N/CUs+offset14_16,0);

	trsv_sync2_loop_i__: for(int i=0;i<end;i++){

		float Xtemp=0;
		if(i<offset1_16)
			Xtemp = Bcast_1_to_16.read();
		else if(i<offset2_16)
			Xtemp = Bcast_2_to_16.read();
		else if(i<offset3_16)
			Xtemp = Bcast_3_to_16.read();
		else if(i<offset4_16)
			Xtemp = Bcast_4_to_16.read();
		else if(i<offset5_16)
			Xtemp = Bcast_5_to_16.read();
		else if(i<offset6_16)
			Xtemp = Bcast_6_to_16.read();
		else if(i<offset7_16)
			Xtemp = Bcast_7_to_16.read();
		else if(i<offset8_16)
			Xtemp = Bcast_8_to_16.read();
		else if(i<offset9_16)
			Xtemp = Bcast_9_to_16.read();
		else if(i<offset10_16)
			Xtemp = Bcast_10_to_16.read();
		else if(i<offset11_16)
			Xtemp = Bcast_11_to_16.read();
		else if(i<offset12_16)
			Xtemp = Bcast_12_to_16.read();
		else if(i<offset13_16)
			Xtemp = Bcast_13_to_16.read();
		else if(i<offset14_16)
			Xtemp = Bcast_14_to_16.read();
		else if(i<offset15_16)
			Xtemp = Bcast_15_to_16.read();
		else{
			if(i%2==0)
				Xtemp = BStream.read() - bram_ping[i%VDATA_SIZE][i/VDATA_SIZE];
			else
				Xtemp = BStream.read() - bram_pong[i%VDATA_SIZE][i/VDATA_SIZE];
		}

		int j=0;
		trsv_sync2_loop_j__: while(j<N_cols){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();
			float Abram[VDATA_SIZE];
			#pragma HLS array_partition variable=Abram complete dim=0


			for(int k=0;k<VDATA_SIZE;k++){
				#pragma hls unroll
				Abram[k]=Atemp.data[k];
			}

			if(i%2==0){
				bram_pong[0][(i+j+15)/VDATA_SIZE] = bram_ping[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_pong[1][(i+j+14)/VDATA_SIZE] = bram_ping[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_pong[2][(i+j+13)/VDATA_SIZE] = bram_ping[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_pong[3][(i+j+12)/VDATA_SIZE] = bram_ping[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_pong[4][(i+j+11)/VDATA_SIZE] = bram_ping[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_pong[5][(i+j+10)/VDATA_SIZE] = bram_ping[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_pong[6][(i+j+9)/VDATA_SIZE] = bram_ping[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_pong[7][(i+j+8)/VDATA_SIZE] = bram_ping[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_pong[8][(i+j+7)/VDATA_SIZE] = bram_ping[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_pong[9][(i+j+6)/VDATA_SIZE] = bram_ping[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_pong[10][(i+j+5)/VDATA_SIZE] = bram_ping[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_pong[11][(i+j+4)/VDATA_SIZE] = bram_ping[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_pong[12][(i+j+3)/VDATA_SIZE] = bram_ping[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_pong[13][(i+j+2)/VDATA_SIZE] = bram_ping[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_pong[14][(i+j+1)/VDATA_SIZE] = bram_ping[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_pong[15][(i+j+0)/VDATA_SIZE] = bram_ping[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}
			else{
				bram_ping[0][(i+j+15)/VDATA_SIZE] = bram_pong[0][(i+j+15)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+0)%VDATA_SIZE];
				bram_ping[1][(i+j+14)/VDATA_SIZE] = bram_pong[1][(i+j+14)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+1)%VDATA_SIZE];
				bram_ping[2][(i+j+13)/VDATA_SIZE] = bram_pong[2][(i+j+13)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+2)%VDATA_SIZE];
				bram_ping[3][(i+j+12)/VDATA_SIZE] = bram_pong[3][(i+j+12)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+3)%VDATA_SIZE];
				bram_ping[4][(i+j+11)/VDATA_SIZE] = bram_pong[4][(i+j+11)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+4)%VDATA_SIZE];
				bram_ping[5][(i+j+10)/VDATA_SIZE] = bram_pong[5][(i+j+10)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+5)%VDATA_SIZE];
				bram_ping[6][(i+j+9)/VDATA_SIZE] = bram_pong[6][(i+j+9)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+6)%VDATA_SIZE];
				bram_ping[7][(i+j+8)/VDATA_SIZE] = bram_pong[7][(i+j+8)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+7)%VDATA_SIZE];
				bram_ping[8][(i+j+7)/VDATA_SIZE] = bram_pong[8][(i+j+7)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+8)%VDATA_SIZE];
				bram_ping[9][(i+j+6)/VDATA_SIZE] = bram_pong[9][(i+j+6)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+9)%VDATA_SIZE];
				bram_ping[10][(i+j+5)/VDATA_SIZE] = bram_pong[10][(i+j+5)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+10)%VDATA_SIZE];
				bram_ping[11][(i+j+4)/VDATA_SIZE] = bram_pong[11][(i+j+4)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+11)%VDATA_SIZE];
				bram_ping[12][(i+j+3)/VDATA_SIZE] = bram_pong[12][(i+j+3)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+12)%VDATA_SIZE];
				bram_ping[13][(i+j+2)/VDATA_SIZE] = bram_pong[13][(i+j+2)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+13)%VDATA_SIZE];
				bram_ping[14][(i+j+1)/VDATA_SIZE] = bram_pong[14][(i+j+1)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+14)%VDATA_SIZE];
				bram_ping[15][(i+j+0)/VDATA_SIZE] = bram_pong[15][(i+j+0)/VDATA_SIZE] + Xtemp*Abram[(VDATA_SIZE-i+15)%VDATA_SIZE];
			}

			j+=VDATA_SIZE;
		}

		if(i>=offset)
			XStream << Xtemp;
	}
}

extern "C" {
void krnl_tbsv(const char Uplo, const char TransA, const char Diag, const int N, const int K, const v_dt  *A1, const v_dt  *A2, const v_dt  *A3,
		const v_dt  *A4, const v_dt  *A5, const v_dt  *A6, const v_dt  *A7, const v_dt  *A8, const v_dt  *A9, const v_dt  *A10, const v_dt  *A11,
		const v_dt  *A12, const v_dt  *A13, const v_dt  *A14, const v_dt  *A15, const v_dt  *A16, const int lda, float  *B, const int incX, float  *X){

	#pragma HLS INTERFACE m_axi port = B offset = slave bundle = ddr0
	#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = ddr1
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = ddr2
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = ddr3
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = ddr4
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = ddr5
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = ddr6
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = ddr7
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = ddr8
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = ddr9
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = ddr10
	#pragma HLS INTERFACE m_axi port = A11 offset = slave bundle = ddr11
	#pragma HLS INTERFACE m_axi port = A12 offset = slave bundle = ddr12
	#pragma HLS INTERFACE m_axi port = A13 offset = slave bundle = ddr13
	#pragma HLS INTERFACE m_axi port = A14 offset = slave bundle = ddr14
	#pragma HLS INTERFACE m_axi port = A15 offset = slave bundle = ddr15
	#pragma HLS INTERFACE m_axi port = A16 offset = slave bundle = ddr16

	#pragma HLS INTERFACE s_axilite port = Uplo
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = Diag
	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = K
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = incX


	#pragma HLS dataflow

	static hls::stream<v_dt> A1_in("A1_in");
	static hls::stream<v_dt> A2_in("A2_in");
	static hls::stream<v_dt> A3_in("A3_in");
	static hls::stream<v_dt> A4_in("A4_in");
	static hls::stream<v_dt> A5_in("A5_in");
	static hls::stream<v_dt> A6_in("A6_in");
	static hls::stream<v_dt> A7_in("A7_in");
	static hls::stream<v_dt> A8_in("A8_in");
	static hls::stream<v_dt> A9_in("A9_in");
	static hls::stream<v_dt> A10_in("A10_in");
	static hls::stream<v_dt> A11_in("A11_in");
	static hls::stream<v_dt> A12_in("A12_in");
	static hls::stream<v_dt> A13_in("A13_in");
	static hls::stream<v_dt> A14_in("A14_in");
	static hls::stream<v_dt> A15_in("A15_in");
	static hls::stream<v_dt> A16_in("A16_in");

	static hls::stream<float> B1_in("B1_in");
	static hls::stream<float> B2_in("B2_in");
	static hls::stream<float> B3_in("B3_in");
	static hls::stream<float> B4_in("B4_in");
	static hls::stream<float> B5_in("B5_in");
	static hls::stream<float> B6_in("B6_in");
	static hls::stream<float> B7_in("B7_in");
	static hls::stream<float> B8_in("B8_in");
	static hls::stream<float> B9_in("B9_in");
	static hls::stream<float> B10_in("B10_in");
	static hls::stream<float> B11_in("B11_in");
	static hls::stream<float> B12_in("B12_in");
	static hls::stream<float> B13_in("B13_in");
	static hls::stream<float> B14_in("B14_in");
	static hls::stream<float> B15_in("B15_in");
	static hls::stream<float> B16_in("B16_in");

	static hls::stream<float> X1_out("X1_out");
	static hls::stream<float> X2_out("X2_out");
	static hls::stream<float> X3_out("X3_out");
	static hls::stream<float> X4_out("X4_out");
	static hls::stream<float> X5_out("X5_out");
	static hls::stream<float> X6_out("X6_out");
	static hls::stream<float> X7_out("X7_out");
	static hls::stream<float> X8_out("X8_out");
	static hls::stream<float> X9_out("X9_out");
	static hls::stream<float> X10_out("X10_out");
	static hls::stream<float> X11_out("X11_out");
	static hls::stream<float> X12_out("X12_out");
	static hls::stream<float> X13_out("X13_out");
	static hls::stream<float> X14_out("X14_out");
	static hls::stream<float> X15_out("X15_out");
	static hls::stream<float> X16_out("X16_out");

	static hls::stream<float> Bcast_1_to_2("Bcast_1_to_2");
	static hls::stream<float> Bcast_1_to_3("Bcast_1_to_3");
	static hls::stream<float> Bcast_1_to_4("Bcast_1_to_4");
	static hls::stream<float> Bcast_1_to_5("Bcast_1_to_5");
	static hls::stream<float> Bcast_1_to_6("Bcast_1_to_6");
	static hls::stream<float> Bcast_1_to_7("Bcast_1_to_7");
	static hls::stream<float> Bcast_1_to_8("Bcast_1_to_8");
	static hls::stream<float> Bcast_1_to_9("Bcast_1_to_9");
	static hls::stream<float> Bcast_1_to_10("Bcast_1_to_10");
	static hls::stream<float> Bcast_1_to_11("Bcast_1_to_11");
	static hls::stream<float> Bcast_1_to_12("Bcast_1_to_12");
	static hls::stream<float> Bcast_1_to_13("Bcast_1_to_13");
	static hls::stream<float> Bcast_1_to_14("Bcast_1_to_14");
	static hls::stream<float> Bcast_1_to_15("Bcast_1_to_15");
	static hls::stream<float> Bcast_1_to_16("Bcast_1_to_16");

	static hls::stream<float> Bcast_2_to_3("Bcast_2_to_3");
	static hls::stream<float> Bcast_2_to_4("Bcast_2_to_4");
	static hls::stream<float> Bcast_2_to_5("Bcast_2_to_5");
	static hls::stream<float> Bcast_2_to_6("Bcast_2_to_6");
	static hls::stream<float> Bcast_2_to_7("Bcast_2_to_7");
	static hls::stream<float> Bcast_2_to_8("Bcast_2_to_8");
	static hls::stream<float> Bcast_2_to_9("Bcast_2_to_9");
	static hls::stream<float> Bcast_2_to_10("Bcast_2_to_10");
	static hls::stream<float> Bcast_2_to_11("Bcast_2_to_11");
	static hls::stream<float> Bcast_2_to_12("Bcast_2_to_12");
	static hls::stream<float> Bcast_2_to_13("Bcast_2_to_13");
	static hls::stream<float> Bcast_2_to_14("Bcast_2_to_14");
	static hls::stream<float> Bcast_2_to_15("Bcast_2_to_15");
	static hls::stream<float> Bcast_2_to_16("Bcast_2_to_16");

	static hls::stream<float> Bcast_3_to_4("Bcast_3_to_4");
	static hls::stream<float> Bcast_3_to_5("Bcast_3_to_5");
	static hls::stream<float> Bcast_3_to_6("Bcast_3_to_6");
	static hls::stream<float> Bcast_3_to_7("Bcast_3_to_7");
	static hls::stream<float> Bcast_3_to_8("Bcast_3_to_8");
	static hls::stream<float> Bcast_3_to_9("Bcast_3_to_9");
	static hls::stream<float> Bcast_3_to_10("Bcast_3_to_10");
	static hls::stream<float> Bcast_3_to_11("Bcast_3_to_11");
	static hls::stream<float> Bcast_3_to_12("Bcast_3_to_12");
	static hls::stream<float> Bcast_3_to_13("Bcast_3_to_13");
	static hls::stream<float> Bcast_3_to_14("Bcast_3_to_14");
	static hls::stream<float> Bcast_3_to_15("Bcast_3_to_15");
	static hls::stream<float> Bcast_3_to_16("Bcast_3_to_16");

	static hls::stream<float> Bcast_4_to_5("Bcast_4_to_5");
	static hls::stream<float> Bcast_4_to_6("Bcast_4_to_6");
	static hls::stream<float> Bcast_4_to_7("Bcast_4_to_7");
	static hls::stream<float> Bcast_4_to_8("Bcast_4_to_8");
	static hls::stream<float> Bcast_4_to_9("Bcast_4_to_9");
	static hls::stream<float> Bcast_4_to_10("Bcast_4_to_10");
	static hls::stream<float> Bcast_4_to_11("Bcast_4_to_11");
	static hls::stream<float> Bcast_4_to_12("Bcast_4_to_12");
	static hls::stream<float> Bcast_4_to_13("Bcast_4_to_13");
	static hls::stream<float> Bcast_4_to_14("Bcast_4_to_14");
	static hls::stream<float> Bcast_4_to_15("Bcast_4_to_15");
	static hls::stream<float> Bcast_4_to_16("Bcast_4_to_16");

	static hls::stream<float> Bcast_5_to_6("Bcast_5_to_6");
	static hls::stream<float> Bcast_5_to_7("Bcast_5_to_7");
	static hls::stream<float> Bcast_5_to_8("Bcast_5_to_8");
	static hls::stream<float> Bcast_5_to_9("Bcast_5_to_9");
	static hls::stream<float> Bcast_5_to_10("Bcast_5_to_10");
	static hls::stream<float> Bcast_5_to_11("Bcast_5_to_11");
	static hls::stream<float> Bcast_5_to_12("Bcast_5_to_12");
	static hls::stream<float> Bcast_5_to_13("Bcast_5_to_13");
	static hls::stream<float> Bcast_5_to_14("Bcast_5_to_14");
	static hls::stream<float> Bcast_5_to_15("Bcast_5_to_15");
	static hls::stream<float> Bcast_5_to_16("Bcast_5_to_16");

	static hls::stream<float> Bcast_6_to_7("Bcast_6_to_7");
	static hls::stream<float> Bcast_6_to_8("Bcast_6_to_8");
	static hls::stream<float> Bcast_6_to_9("Bcast_6_to_9");
	static hls::stream<float> Bcast_6_to_10("Bcast_6_to_10");
	static hls::stream<float> Bcast_6_to_11("Bcast_6_to_11");
	static hls::stream<float> Bcast_6_to_12("Bcast_6_to_12");
	static hls::stream<float> Bcast_6_to_13("Bcast_6_to_13");
	static hls::stream<float> Bcast_6_to_14("Bcast_6_to_14");
	static hls::stream<float> Bcast_6_to_15("Bcast_6_to_15");
	static hls::stream<float> Bcast_6_to_16("Bcast_6_to_16");

	static hls::stream<float> Bcast_7_to_8("Bcast_7_to_8");
	static hls::stream<float> Bcast_7_to_9("Bcast_7_to_9");
	static hls::stream<float> Bcast_7_to_10("Bcast_7_to_10");
	static hls::stream<float> Bcast_7_to_11("Bcast_7_to_11");
	static hls::stream<float> Bcast_7_to_12("Bcast_7_to_12");
	static hls::stream<float> Bcast_7_to_13("Bcast_7_to_13");
	static hls::stream<float> Bcast_7_to_14("Bcast_7_to_14");
	static hls::stream<float> Bcast_7_to_15("Bcast_7_to_15");
	static hls::stream<float> Bcast_7_to_16("Bcast_7_to_16");

	static hls::stream<float> Bcast_8_to_9("Bcast_8_to_9");
	static hls::stream<float> Bcast_8_to_10("Bcast_8_to_10");
	static hls::stream<float> Bcast_8_to_11("Bcast_8_to_11");
	static hls::stream<float> Bcast_8_to_12("Bcast_8_to_12");
	static hls::stream<float> Bcast_8_to_13("Bcast_8_to_13");
	static hls::stream<float> Bcast_8_to_14("Bcast_8_to_14");
	static hls::stream<float> Bcast_8_to_15("Bcast_8_to_15");
	static hls::stream<float> Bcast_8_to_16("Bcast_8_to_16");

	static hls::stream<float> Bcast_9_to_10("Bcast_9_to_10");
	static hls::stream<float> Bcast_9_to_11("Bcast_9_to_11");
	static hls::stream<float> Bcast_9_to_12("Bcast_9_to_12");
	static hls::stream<float> Bcast_9_to_13("Bcast_9_to_13");
	static hls::stream<float> Bcast_9_to_14("Bcast_9_to_14");
	static hls::stream<float> Bcast_9_to_15("Bcast_9_to_15");
	static hls::stream<float> Bcast_9_to_16("Bcast_9_to_16");

	static hls::stream<float> Bcast_10_to_11("Bcast_10_to_11");
	static hls::stream<float> Bcast_10_to_12("Bcast_10_to_12");
	static hls::stream<float> Bcast_10_to_13("Bcast_10_to_13");
	static hls::stream<float> Bcast_10_to_14("Bcast_10_to_14");
	static hls::stream<float> Bcast_10_to_15("Bcast_10_to_15");
	static hls::stream<float> Bcast_10_to_16("Bcast_10_to_16");

	static hls::stream<float> Bcast_11_to_12("Bcast_11_to_12");
	static hls::stream<float> Bcast_11_to_13("Bcast_11_to_13");
	static hls::stream<float> Bcast_11_to_14("Bcast_11_to_14");
	static hls::stream<float> Bcast_11_to_15("Bcast_11_to_15");
	static hls::stream<float> Bcast_11_to_16("Bcast_11_to_16");

	static hls::stream<float> Bcast_12_to_13("Bcast_12_to_13");
	static hls::stream<float> Bcast_12_to_14("Bcast_12_to_14");
	static hls::stream<float> Bcast_12_to_15("Bcast_12_to_15");
	static hls::stream<float> Bcast_12_to_16("Bcast_12_to_16");

	static hls::stream<float> Bcast_13_to_14("Bcast_13_to_14");
	static hls::stream<float> Bcast_13_to_15("Bcast_13_to_15");
	static hls::stream<float> Bcast_13_to_16("Bcast_13_to_16");

	static hls::stream<float> Bcast_14_to_15("Bcast_14_to_15");
	static hls::stream<float> Bcast_14_to_16("Bcast_14_to_16");

	static hls::stream<float> Bcast_15_to_16("Bcast_15_to_16");

	#pragma HLS STREAM variable = A1_in depth = 2048
	#pragma HLS STREAM variable = A2_in depth = 2048
	#pragma HLS STREAM variable = A3_in depth = 2048
	#pragma HLS STREAM variable = A4_in depth = 2048
	#pragma HLS STREAM variable = A5_in depth = 2048
	#pragma HLS STREAM variable = A6_in depth = 2048
	#pragma HLS STREAM variable = A7_in depth = 2048
	#pragma HLS STREAM variable = A8_in depth = 2048

	#pragma HLS STREAM variable = B1_in depth = 2048
	#pragma HLS STREAM variable = B2_in depth = 2048
	#pragma HLS STREAM variable = B3_in depth = 2048
	#pragma HLS STREAM variable = B4_in depth = 2048
	#pragma HLS STREAM variable = B5_in depth = 2048
	#pragma HLS STREAM variable = B6_in depth = 2048
	#pragma HLS STREAM variable = B7_in depth = 2048
	#pragma HLS STREAM variable = B8_in depth = 2048

	#pragma HLS STREAM variable = X1_out depth = 2048
	#pragma HLS STREAM variable = X2_out depth = 2048
	#pragma HLS STREAM variable = X3_out depth = 2048
	#pragma HLS STREAM variable = X4_out depth = 2048
	#pragma HLS STREAM variable = X5_out depth = 2048
	#pragma HLS STREAM variable = X6_out depth = 2048
	#pragma HLS STREAM variable = X7_out depth = 2048
	#pragma HLS STREAM variable = X8_out depth = 2048

	#pragma HLS STREAM variable = Bcast_1_to_2 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_3 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_4 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_5 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_6 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_7 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_2_to_3 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_4 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_5 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_6 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_7 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_2_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_3_to_4 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_5 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_6 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_7 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_3_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_4_to_5 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_6 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_7 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_4_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_5_to_6 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_7 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_5_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_6_to_7 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_6_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_7_to_8 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_7_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_8_to_9 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_8_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_9_to_10 depth = 2048
	#pragma HLS STREAM variable = Bcast_9_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_9_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_9_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_9_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_9_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_9_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_10_to_11 depth = 2048
	#pragma HLS STREAM variable = Bcast_10_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_10_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_10_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_10_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_10_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_11_to_12 depth = 2048
	#pragma HLS STREAM variable = Bcast_11_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_11_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_11_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_11_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_12_to_13 depth = 2048
	#pragma HLS STREAM variable = Bcast_12_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_12_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_12_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_13_to_14 depth = 2048
	#pragma HLS STREAM variable = Bcast_13_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_13_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_14_to_15 depth = 2048
	#pragma HLS STREAM variable = Bcast_14_to_16 depth = 2048

	#pragma HLS STREAM variable = Bcast_15_to_16 depth = 2048

	read_b(B1_in, B2_in, B3_in, B4_in, B5_in, B6_in, B7_in, B8_in,
			B9_in, B10_in, B11_in, B12_in, B13_in, B14_in, B15_in, B16_in, B, N);

	read_matrix(A1, A1_in, N, K, 0*N/CUs, 1*N/CUs);
	read_matrix(A2, A2_in, N, K, 1*N/CUs-K, 2*N/CUs);
	read_matrix(A3, A3_in, N, K, 2*N/CUs-K, 3*N/CUs);
	read_matrix(A4, A4_in, N, K, 3*N/CUs-K, 4*N/CUs);
	read_matrix(A5, A5_in, N, K, 4*N/CUs-K, 5*N/CUs);
	read_matrix(A6, A6_in, N, K, 5*N/CUs-K, 6*N/CUs);
	read_matrix(A7, A7_in, N, K, 6*N/CUs-K, 7*N/CUs);
	read_matrix(A8, A8_in, N, K, 7*N/CUs-K, 8*N/CUs);
	read_matrix(A9, A9_in, N, K, 8*N/CUs-K, 9*N/CUs);
	read_matrix(A10, A10_in, N, K, 9*N/CUs-K, 10*N/CUs);
	read_matrix(A11, A11_in, N, K, 10*N/CUs-K, 11*N/CUs);
	read_matrix(A12, A12_in, N, K, 11*N/CUs-K, 12*N/CUs);
	read_matrix(A13, A13_in, N, K, 12*N/CUs-K, 13*N/CUs);
	read_matrix(A14, A14_in, N, K, 13*N/CUs-K, 14*N/CUs);
	read_matrix(A15, A15_in, N, K, 14*N/CUs-K, 15*N/CUs);
	read_matrix(A16, A16_in, N, K, 15*N/CUs-K, 16*N/CUs);


	tbsv_sync1(B1_in, A1_in, X1_out, Bcast_1_to_2, Bcast_1_to_3, Bcast_1_to_4, Bcast_1_to_5, Bcast_1_to_6, Bcast_1_to_7, Bcast_1_to_8,
			Bcast_1_to_9, Bcast_1_to_10, Bcast_1_to_11, Bcast_1_to_12, Bcast_1_to_13, Bcast_1_to_14, Bcast_1_to_15, Bcast_1_to_16, N, K);
	tbsv_sync2(B2_in, A2_in, X2_out, Bcast_1_to_2, Bcast_2_to_3, Bcast_2_to_4, Bcast_2_to_5, Bcast_2_to_6, Bcast_2_to_7, Bcast_2_to_8,
			Bcast_2_to_9, Bcast_2_to_10, Bcast_2_to_11, Bcast_2_to_12, Bcast_2_to_13, Bcast_2_to_14, Bcast_2_to_15, Bcast_2_to_16, N, K);
	tbsv_sync3(B3_in, A3_in, X3_out, Bcast_1_to_3, Bcast_2_to_3, Bcast_3_to_4, Bcast_3_to_5, Bcast_3_to_6, Bcast_3_to_7, Bcast_3_to_8,
			Bcast_3_to_9, Bcast_3_to_10, Bcast_3_to_11, Bcast_3_to_12, Bcast_3_to_13, Bcast_3_to_14, Bcast_3_to_15, Bcast_3_to_16, N, K);
	tbsv_sync4(B4_in, A4_in, X4_out, Bcast_1_to_4, Bcast_2_to_4, Bcast_3_to_4, Bcast_4_to_5, Bcast_4_to_6, Bcast_4_to_7, Bcast_4_to_8,
			Bcast_4_to_9, Bcast_4_to_10, Bcast_4_to_11, Bcast_4_to_12, Bcast_4_to_13, Bcast_4_to_14, Bcast_4_to_15, Bcast_4_to_16, N, K);
	tbsv_sync5(B5_in, A5_in, X5_out, Bcast_1_to_5, Bcast_2_to_5, Bcast_3_to_5, Bcast_4_to_5, Bcast_5_to_6, Bcast_5_to_7, Bcast_5_to_8,
			Bcast_5_to_9, Bcast_5_to_10, Bcast_5_to_11, Bcast_5_to_12, Bcast_5_to_13, Bcast_5_to_14, Bcast_5_to_15, Bcast_5_to_16, N, K);
	tbsv_sync6(B6_in, A6_in, X6_out, Bcast_1_to_6, Bcast_2_to_6, Bcast_3_to_6, Bcast_4_to_6, Bcast_5_to_6, Bcast_6_to_7, Bcast_6_to_8,
			Bcast_6_to_9, Bcast_6_to_10, Bcast_6_to_11, Bcast_6_to_12, Bcast_6_to_13, Bcast_6_to_14, Bcast_6_to_15, Bcast_6_to_16, N, K);
	tbsv_sync7(B7_in, A7_in, X7_out, Bcast_1_to_7, Bcast_2_to_7, Bcast_3_to_7, Bcast_4_to_7, Bcast_5_to_7, Bcast_6_to_7, Bcast_7_to_8,
			Bcast_7_to_9, Bcast_7_to_10, Bcast_7_to_11, Bcast_7_to_12, Bcast_7_to_13, Bcast_7_to_14, Bcast_7_to_15, Bcast_7_to_16, N, K);
	tbsv_sync8(B8_in, A8_in, X8_out, Bcast_1_to_8, Bcast_2_to_8, Bcast_3_to_8, Bcast_4_to_8, Bcast_5_to_8, Bcast_6_to_8, Bcast_7_to_8,
			Bcast_8_to_9, Bcast_8_to_10, Bcast_8_to_11, Bcast_8_to_12, Bcast_8_to_13, Bcast_8_to_14, Bcast_8_to_15, Bcast_8_to_16, N, K);
	tbsv_sync9(B9_in, A9_in, X9_out, Bcast_1_to_9, Bcast_2_to_9, Bcast_3_to_9, Bcast_4_to_9, Bcast_5_to_9, Bcast_6_to_9, Bcast_7_to_9,
			Bcast_8_to_9, Bcast_9_to_10, Bcast_9_to_11, Bcast_9_to_12, Bcast_9_to_13, Bcast_9_to_14, Bcast_9_to_15, Bcast_9_to_16, N, K);
	tbsv_sync10(B10_in, A10_in, X10_out, Bcast_1_to_10, Bcast_2_to_10, Bcast_3_to_10, Bcast_4_to_10, Bcast_5_to_10, Bcast_6_to_10, Bcast_7_to_10,
			Bcast_8_to_10, Bcast_9_to_10, Bcast_10_to_11, Bcast_10_to_12, Bcast_10_to_13, Bcast_10_to_14, Bcast_10_to_15, Bcast_10_to_16, N, K);
	tbsv_sync11(B11_in, A11_in, X11_out, Bcast_1_to_11, Bcast_2_to_11, Bcast_3_to_11, Bcast_4_to_11, Bcast_5_to_11, Bcast_6_to_11, Bcast_7_to_11,
			Bcast_8_to_11, Bcast_9_to_11, Bcast_10_to_11, Bcast_11_to_12, Bcast_11_to_13, Bcast_11_to_14, Bcast_11_to_15, Bcast_11_to_16, N, K);
	tbsv_sync12(B12_in, A12_in, X12_out, Bcast_1_to_12, Bcast_2_to_12, Bcast_3_to_12, Bcast_4_to_12, Bcast_5_to_12, Bcast_6_to_12, Bcast_7_to_12,
			Bcast_8_to_12, Bcast_9_to_12, Bcast_10_to_12, Bcast_11_to_12, Bcast_12_to_13, Bcast_12_to_14, Bcast_12_to_15, Bcast_12_to_16, N, K);
	tbsv_sync13(B13_in, A13_in, X13_out, Bcast_1_to_13, Bcast_2_to_13, Bcast_3_to_13, Bcast_4_to_13, Bcast_5_to_13, Bcast_6_to_13, Bcast_7_to_13,
			Bcast_8_to_13, Bcast_9_to_13, Bcast_10_to_13, Bcast_11_to_13, Bcast_12_to_13, Bcast_13_to_14, Bcast_13_to_15, Bcast_13_to_16, N, K);
	tbsv_sync14(B14_in, A14_in, X14_out, Bcast_1_to_14, Bcast_2_to_14, Bcast_3_to_14, Bcast_4_to_14, Bcast_5_to_14, Bcast_6_to_14, Bcast_7_to_14,
			Bcast_8_to_14, Bcast_9_to_14, Bcast_10_to_14, Bcast_11_to_14, Bcast_12_to_14, Bcast_13_to_14, Bcast_14_to_15, Bcast_14_to_16, N, K);
	tbsv_sync15(B15_in, A15_in, X15_out, Bcast_1_to_15, Bcast_2_to_15, Bcast_3_to_15, Bcast_4_to_15, Bcast_5_to_15, Bcast_6_to_15, Bcast_7_to_15,
			Bcast_8_to_15, Bcast_9_to_15, Bcast_10_to_15, Bcast_11_to_15, Bcast_12_to_15, Bcast_13_to_15, Bcast_14_to_15, Bcast_15_to_16, N, K);
	tbsv_sync16(B16_in, A16_in, X16_out, Bcast_1_to_16, Bcast_2_to_16, Bcast_3_to_16, Bcast_4_to_16, Bcast_5_to_16, Bcast_6_to_16, Bcast_7_to_16,
			Bcast_8_to_16, Bcast_9_to_16, Bcast_10_to_16, Bcast_11_to_16, Bcast_12_to_16, Bcast_13_to_16, Bcast_14_to_16, Bcast_15_to_16, N, K);

	write_x(X1_out, X2_out, X3_out, X4_out, X5_out, X6_out, X7_out, X8_out,
			X9_out, X10_out, X11_out, X12_out, X13_out, X14_out, X15_out, X16_out, X, N);

}}
