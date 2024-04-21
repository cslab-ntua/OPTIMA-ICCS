// #include "global.hpp"
// #include "common.hpp"
#include <stdio.h>
#include <hls_stream.h>
// #include <ap_int.h>
#include <hls_math.h>
#include "ap_int.h"

typedef float ValueType;

#define BRAM_SIZE 1792
#define W 16
#define __SYNTHESIS__
typedef struct v_datatype { ValueType data[W]; } v_dt;

void read_b(hls::stream<ValueType>& Bstream1, hls::stream<ValueType>& Bstream2, 
	const ValueType *B, 
	const int K, const int N, int row_break0)
{
	for(int n=0;n<N;n++){
		read_b:
		for(int i=0;i<K;i++){
			#pragma HLS pipeline II=1
			if(i<row_break0){
				Bstream1 << B[i*N+n];
				#ifndef __SYNTHESIS__
				printf("Bstream1\tforwarded i = %ld, value = %f\n", i*N+n, B[i*N+n]);
				#endif
			}
			else{
				Bstream2 << B[i*N+n];
				#ifndef __SYNTHESIS__
				printf("Bstream2\tforwarded i = %ld, value = %f\n", i*N+n, B[i*N+n]);
				#endif
			}
		}
	}
}

void read_matrix1(const v_dt *A, hls::stream<v_dt>& AStream, const int K, const int N, int row_break0)
{
	for(int n=0;n<N;n++){
		int i=0, j=0;

		wide_read1:
		while(i<row_break0){
			#pragma HLS pipeline II=1
			v_dt Atemp=A[i*(K/W)+j];
		
			#ifndef __SYNTHESIS__
			printf("index i*(K/W)+j = %d -> Atemp (from read_matrix1) [ ", i*(K/W)+j);
			for(int w = 0; w < W; w++)
				printf("%-5.1f ", Atemp.data[w]);
			printf("]\n");
			#endif
		
			AStream << Atemp;

			if(j>=row_break0/W-1){
				i++;
				j=i/W;
				printf(">>>>>>>>> in read_matrix1 special... n = %d, i = %d, j = %d\n", n, i, j);
			}
			else{
				j++;
			}
		}
	}
}

void read_matrix2(const v_dt *A, hls::stream<v_dt>& AStream, const int K, const int N, int row_break0)
{
	for(int n=0;n<N;n++){
		int i=0, j=hls::fmax(i,row_break0)/W;

		wide_read2:
		while(i<K){
			#pragma HLS pipeline II=1
			v_dt Atemp=A[i*(K/W)+j];

			#ifndef __SYNTHESIS__
			printf("index i*(K/W)+j = %d -> Atemp (from read_matrix2) [ ", i*(K/W)+j);
			for(int w = 0; w < W; w++)
				printf("%-5.1f ", Atemp.data[w]);
			printf("]\n");
			#endif
		
			AStream << Atemp;

			if(j>=K/W-1){
				i++;
				j=hls::fmax(i,row_break0)/W;
				printf(">>>>>>>>> in read_matrix2 special... n = %d, i = %d, j = %d\n", n, i, j);
			}
			else{
				j++;
			}
		}
	}
}

void trsm_sync1(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_1_to_2,
	const int K, const int N, int row_break0)
{
	for(int n=0;n<N;n++){
		ValueType bram_ping[W][BRAM_SIZE];
		#pragma HLS array_partition variable=bram_ping complete dim=1

		ValueType bram_pong[W][BRAM_SIZE];
		#pragma HLS array_partition variable=bram_pong complete dim=1

		for(int k=0;k<BRAM_SIZE;k++){
			for(int j=0;j<W;j++){
				#pragma hls unroll
				bram_ping[j][k]=0;
				bram_pong[j][k]=0;
			}
		}

		trsm_sync1_loop_i__:
		for(int i=0;i<row_break0;i++){
			ValueType Xtemp=0;
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i)%W][(i)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(n=%d, i=%d) reading from ping buffer %f ( %f - %f )\n", n, i, Xtemp, bstream_tmp, bram_ping[(i)%W][(i)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i)%W][(i)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(n=%d, i=%d) reading from pong buffer %f ( %f - %f )\n", n, i, Xtemp, bstream_tmp, bram_pong[(i)%W][(i)/W]);
				#endif
			}

			int j=i;
			trsm_sync1_loop_j__:
			while(j<row_break0){
				#pragma hls pipeline ii=1

				v_dt Atemp = AStream.read();

				#ifndef __SYNTHESIS__
				printf("(n = %d, i = %d, j = %d) ->  [ ", n, i, j);
				#endif
				for(int k=0; k<W;k++){
					#pragma hls unroll
					#ifndef __SYNTHESIS__
					printf("%f ", Atemp.data[k]);
					#endif

					if(i%2==0){
						ValueType temp = bram_ping[k][j/W];
						ValueType temp2 = temp+Xtemp*Atemp.data[k];
						bram_pong[k][j/W] = temp2;
					}
					else{
						ValueType temp = bram_pong[k][j/W];
						ValueType temp2 = temp+Xtemp*Atemp.data[k];
						bram_ping[k][j/W] = temp2;
					}
				}
				#ifndef __SYNTHESIS__
				printf("]\n");
				#endif
				j+=W;
			}

			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(n=%d, i=%d) writing to XStream1     from trsm_sync1 %f\n", n, i, Xtemp);
			#endif
			Bcast_1_to_2 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(n=%d, i=%d) writing to Bcast_1_to_2 from trsm_sync1 %f\n", n, i, Xtemp);
			#endif
		}
	}
}

void trsm_sync2(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_2,
	/* to   where we send    data */ 
	const int K, const int N, int row_break0)
{
	for(int n=0;n<N;n++){
		ValueType bram_ping[W][BRAM_SIZE];
		#pragma HLS array_partition variable=bram_ping complete dim=1

		ValueType bram_pong[W][BRAM_SIZE];
		#pragma HLS array_partition variable=bram_pong complete dim=1

		for(int k=0;k<BRAM_SIZE;k++){
			for(int j=0;j<W;j++){
				#pragma hls unroll
				bram_ping[j][k]=0;
				bram_pong[j][k]=0;
			}
		}

		trsm_sync2_loop_i__:
		for(int i=0;i<K;i++){
			ValueType Xtemp=0;
			if(i<row_break0){
				if(i%2==0)
					Xtemp = Bcast_1_to_2.read();// - bram_ping[(i)%W][(i)/W];
				else
					Xtemp = Bcast_1_to_2.read();// - bram_pong[(i)%W][(i)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(n=%d, i=%d) reading from Bcast_1_to_2 (from trsm_sync1) %f\n", n, i, Xtemp);
				#endif
			}
			else{		
				if(i%2==0){
					ValueType bstream_tmp = BStream.read();
					Xtemp = bstream_tmp - bram_ping[(i-row_break0)%W][(i-row_break0)/W];
					#ifndef __SYNTHESIS__
					printf("\n\t(n=%d, i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, n, bstream_tmp, bram_ping[(i-row_break0)%W][(i-row_break0)/W]);
					#endif
				}
				else{
					ValueType bstream_tmp = BStream.read();
					Xtemp = bstream_tmp - bram_pong[(i-row_break0)%W][(i-row_break0)/W];
					#ifndef __SYNTHESIS__
					printf("\n\t(n=%d, i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, n, bstream_tmp, bram_pong[(i-row_break0)%W][(i-row_break0)/W]);
					#endif
				}
			}

			int j=hls::fmax(i-row_break0,0);
			trsm_sync2_loop_j__:
			while(j<K-row_break0){
				#pragma hls pipeline ii=1

				v_dt Atemp = AStream.read();

				#ifndef __SYNTHESIS__
				printf("(n = %d, i = %d, j = %d) ->  [ ", n, i, j);
				#endif
				for(int k=0; k<W;k++){
					#pragma hls unroll
					#ifndef __SYNTHESIS__
					printf("%f ", Atemp.data[k]);
					#endif

					if(i%2==0){
						ValueType temp = bram_ping[k][j/W];
						ValueType temp2 = temp+Xtemp*Atemp.data[k];
						bram_pong[k][j/W] = temp2;
					}
					else{
						ValueType temp = bram_pong[k][j/W];
						ValueType temp2 = temp+Xtemp*Atemp.data[k];
						bram_ping[k][j/W] = temp2;
					}
				}
				#ifndef __SYNTHESIS__
				printf("]\n");
				#endif
				j+=W;
			}

			if(i>=row_break0){
				XStream << Xtemp;
				#ifndef __SYNTHESIS__
				printf("\t(n=%d, i=%d) writing to XStream2     from trsm_sync2 %f\n", n, i, Xtemp);
				#endif
			}
		}
	}
}

void write_x(hls::stream<ValueType>& Xstream1, hls::stream<ValueType>& Xstream2,
	ValueType *X, 
	const int K, const int N, int row_break0)
{
	for(int n=0;n<N;n++){
		write_y:
		for(int i=0;i<K;i++){
			#pragma HLS pipeline II=1
			if(i<row_break0){
				X[i*N+n] = Xstream1.read();
				#ifndef __SYNTHESIS__
				printf("Xstream1\twrote i = %ld, value = %f to X\n", i*N+n, X[i*N+n]);
				#endif
			}
			else{
				X[i*N+n] = Xstream2.read();
				#ifndef __SYNTHESIS__
				printf("Xstream2\twrote i = %ld, value = %f to X\n", i*N+n, X[i*N+n]);
				#endif
			}
		}
	}
}

extern "C" {

void krnl_trsm(const char Side, const char Uplo, const char TransA, const char Diag, 
	const int K, const int N, 
	const ValueType alpha, const v_dt *A1, const v_dt *A2, const int lda, 
	ValueType *B, ValueType *X, const int ldb, 
	const int row_break0){

	#pragma HLS INTERFACE m_axi port = B offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = X offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = gmem2

	#pragma HLS INTERFACE s_axilite port = Side
	#pragma HLS INTERFACE s_axilite port = Uplo
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = Diag
	#pragma HLS INTERFACE s_axilite port = K
	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = ldb
	#pragma HLS INTERFACE s_axilite port = row_break0

	static hls::stream<v_dt> A1_in("A1_in");
	static hls::stream<v_dt> A2_in("A2_in");

	static hls::stream<ValueType> B1_in("B1_in");
	static hls::stream<ValueType> B2_in("B2_in");

	static hls::stream<ValueType> X1_out("X1_out");
	static hls::stream<ValueType> X2_out("X2_out");

	static hls::stream<ValueType> Bcast_1_to_2("Bcast_1_to_2");

	#pragma HLS STREAM variable = A1_in depth = 2048
	#pragma HLS STREAM variable = A2_in depth = 2048
	#pragma HLS STREAM variable = B1_in depth = 2048
	#pragma HLS STREAM variable = B2_in depth = 2048
	#pragma HLS STREAM variable = X1_out depth = 2048
	#pragma HLS STREAM variable = X2_out depth = 2048
	#pragma HLS STREAM variable = Bcast_1_to_2 depth = 2048

	#pragma HLS DATAFLOW

	printf("------------------------------------------------------------------------------------------------------------\nread_b\n");	
	read_b(B1_in, B2_in, B, K, N, row_break0);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix1\n");
	read_matrix1(A1, A1_in, K, N, row_break0);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix2\n");
	read_matrix2(A2, A2_in, K, N, row_break0);

	printf("------------------------------------------------------------------------------------------------------------\ntrsm_sync1\n");
	trsm_sync1(B1_in, A1_in, X1_out, Bcast_1_to_2, K, N, row_break0);
	printf("------------------------------------------------------------------------------------------------------------\ntrsm_sync2\n");
	trsm_sync2(B2_in, A2_in, X2_out, Bcast_1_to_2, K, N, row_break0);

	printf("------------------------------------------------------------------------------------------------------------\nwrite_x\n");
	write_x(X1_out, X2_out, X, K, N, row_break0);
	printf("------------------------------------------------------------------------------------------------------------\n\n");


}

}
