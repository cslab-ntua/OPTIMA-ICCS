// #include "global.hpp"
// #include "common.hpp"
#include <stdio.h>
#include <hls_stream.h>
// #include <ap_int.h>
#include <hls_math.h>
#include "ap_int.h"

typedef float ValueType;

#define D 4

#define W 16
#define TN 64
// #define __SYNTHESIS__
typedef struct v_datatype { ValueType data[W]; } v_dt;

void read_A(const ValueType *A, hls::stream<ValueType> &A_Stream, const int M, const int K, const int N, const ValueType alpha, const ValueType beta)
{
	read_A_loop1_bm___:
	for (int bm = 0; bm < M / D; bm++) {
		read_A_loop2_bn___:
		for (int bn = 0; bn < N / (TN * W); bn++) {
			read_A_loop3_k___:
			for (int k = 0; k < K; k++) {
				read_A_loop4_tm___:
				for (int tm = 0; tm < D; tm++) {
					#pragma HLS LOOP_FLATTEN
					#pragma HLS PIPELINE II=1
					int index = bm * D * K + tm * K + k;
					ValueType A_curr = A[index];
					A_Stream << A_curr;
					
					#ifndef __SYNTHESIS__
					printf("index A = %d (%d,%d) [ %lf ]\n", index, index/K, index%K, A[index]);
					#endif
				}
			}
		}
	}
}

void read_B(const v_dt *B, hls::stream<v_dt> &B_Stream, const int M, const int K, const int N, const ValueType alpha, const ValueType beta)
{
	read_B_loop1_bm___:
	for (int bm = 0; bm < M / D; bm++) {
		read_B_loop2_bn___:
		for (int bn = 0; bn < N / (TN * W); bn++) {
			read_B_loop3_k___:
			for (int k = 0; k < K; k++) {
				read_B_loop4_tn___:
				for (int tn = 0; tn < TN; tn++) {
					#pragma HLS LOOP_FLATTEN
					#pragma HLS PIPELINE II=1
					int index = k * (N / W) + bn * TN + tn;
					v_dt B_curr = B[index];
					B_Stream << B_curr;
					
					#ifndef __SYNTHESIS__
					printf("index B = %d (%d,%d-%d) [ ", index, (index*W)/N, (index*W)%N, (index*W)%N+W-1);
					for(int w = 0; w < W; w++)
						printf("%lf ", B_curr.data[w]);
					printf("]\n");
					#endif
				}
			}
		}
	}
}

void read_C_in(const v_dt *C_in, hls::stream<v_dt> &C_in_Stream, const int M, const int N, const ValueType alpha, const ValueType beta)
{
	read_C_in_loop1_bm___:
	for (int bm = 0; bm < M / D; bm++) {
		read_C_in_loop2_bn___:
		for (int bn = 0; bn < N / (TN * W); bn++) {
			read_C_in_loop3_tm___:
			for (int tm = 0; tm < D; tm++) {
				read_C_in_loop4_tn___:
				for (int tn = 0; tn < TN; tn++) {
					#pragma HLS LOOP_FLATTEN
					#pragma HLS PIPELINE II=1
					int index = bm * D * (N / W) + tm * (N / W) + bn * TN + tn;
					v_dt C_curr = C_in[index];
					C_in_Stream << C_curr;
					
					#ifndef __SYNTHESIS__
					printf("index C_in = %d (%d,%d-%d) [ ", index, (index*W)/N, (index*W)%N, (index*W)%N+W-1);
					for(int w = 0; w < W; w++)
						printf("%lf ", C_curr.data[w]);
					printf("]\n");
					#endif
				}
			}
		}
	}
}

// void read_C_in(v_dt *C_in, hls::stream<v_dt> &C_in_Stream, int M, int N, const ValueType alpha, const ValueType beta)
// {
// 	read_C_in_loop1_i:
// 	for(int i=0; i<(M*N)/W; i++){
// 		#pragma HLS PIPELINE II=1
// 		v_dt C_curr = C_in[i];
// 		C_in_Stream << C_curr;
// 		#ifndef __SYNTHESIS__
// 		printf("index C_in = %d (%d,%d-%d) [ ", i, (i*W)/N, (i*W)%N, (i*W)%N+W-1);
// 		for(int w = 0; w < W; w++)
// 			printf("%lf ", C_curr.data[w]);
// 		printf("]\n");
// 		#endif
// 	}
// }

void write_C_out(hls::stream<v_dt> &C_Stream, hls::stream<v_dt> &C_Stream_in, v_dt *C_out, int M, int N, const ValueType alpha, const ValueType beta)
{
	write_C_out_loop1_bm___:
	for (int bm = 0; bm < M / D; bm++) {
		write_C_out_loop2_bn___:
		for (int bn = 0; bn < N / (TN * W); bn++) {
			write_C_out_loop3_tm___:
			for (int tm = 0; tm < D; tm++) {
				write_C_out_loop4_tn___:
				for (int tn = 0; tn < TN; tn++) {
					#pragma HLS LOOP_FLATTEN
					#pragma HLS PIPELINE II=1
					v_dt C_curr = C_Stream.read();
					v_dt C_in = C_Stream_in.read();
					v_dt C_new;
					for(int w = 0; w < W; w++){
						#pragma HLS UNROLL
						C_new.data[w] = C_curr.data[w] + beta * C_in.data[w];
						
						#ifndef __SYNTHESIS__
						printf("---> C_new.data[%d] = %lf + %lf * %lf = %lf\n", w, C_curr.data[w], beta, C_in.data[w], C_new.data[w]);
						#endif
					}
					int index = bm * D * (N / W) + tm * (N / W) + bn * TN + tn;
					C_out[index] = C_new;
					
					#ifndef __SYNTHESIS__
					printf("index C_out = %d (%d,%d-%d) [ ", index, (index*W)/N, (index*W)%N, (index*W)%N+W-1);
					for(int w = 0; w < W; w++)
						printf("%lf ", C_new.data[w]);
					printf("]\n");
					#endif
				}
			}
		}
	}
}

// void write_C_out(hls::stream<v_dt> &C_Stream, hls::stream<v_dt> &C_Stream_in, v_dt *C_out, int M, int N, const ValueType alpha, const ValueType beta)
// {
// 	write_C_out_loop1_i:
// 	for(int i=0; i<(M*N)/W; i++){
// 		#pragma HLS PIPELINE II=1
// 		v_dt C_curr = C_Stream.read();
// 		v_dt C_in = C_Stream_in.read();
// 		v_dt C_new;
// 		for(int w = 0; w < W; w++){
// 			#pragma HLS UNROLL
// 			C_new.data[w] = C_curr.data[w] + beta * C_in.data[w];
			
// 			#ifndef __SYNTHESIS__
// 			printf("---> C_new.data[%d] = %lf + %lf * %lf = %lf\n", w, C_curr.data[w], beta, C_in.data[w], C_new.data[w]);
// 			#endif
// 		}
// 		C_out[i] = C_new;
		
// 		#ifndef __SYNTHESIS__
// 		printf("index C_out = %d (%d,%d-%d) [ ", i, (i*W)/N, (i*W)%N, (i*W)%N+W-1);
// 		for(int w = 0; w < W; w++)
// 			printf("%lf ", C_new.data[w]);
// 		printf("]\n");
// 		#endif
// 	}
// }

void computation_part(hls::stream<ValueType> &A_Stream_in, hls::stream<ValueType> &A_Stream_out,
					  hls::stream<v_dt> &B_Stream_in,      hls::stream<v_dt> &B_Stream_out,
					  hls::stream<v_dt> &C_Stream_in,      hls::stream<v_dt> &C_Stream_out,
					  const int M, const int K, const int N,
					  ValueType alpha, ValueType beta, 
					  int d) {

	// Loop over tiles of C
	computation_part_loop1_bm___:
	for (int bm = 0; bm < M / D; bm++) {
		computation_part_loop2_bn___:
		for (int bn = 0; bn < N / (TN * W); bn++) {
			#pragma HLS LOOP_FLATTEN

			v_dt c_buffer[TN]; // Local result buffer

			// Loop over collapsed dimension
			computation_part_loop3_k___:
			for (int k = 0; k < K; k++) {

				ValueType a_buffer; // Local buffer of A

				// Buffer own value and forward columns to saturate all PEs
				computation_part_loop4_nd___:
				for (int nd = 0; nd < D - d; nd++) {
					#pragma HLS PIPELINE II=1
					a_buffer = A_Stream_in.read();
					if (nd < D - d - 1)
						A_Stream_out << a_buffer;

					#ifndef __SYNTHESIS__
					printf("A_BUFFER. DIAVASA TO stoixeio = [ %lf ]\n", a_buffer);
					#endif
				}

				// Stream row of B and apply it to the stored value of A
				computation_part_loop4_tn___:
				for (int tn = 0; tn < TN; tn++) {
					#pragma HLS PIPELINE II=1
					v_dt b_read = B_Stream_in.read();
					
					#ifndef __SYNTHESIS__
					printf("B_BUFFER. DIAVASA TA stoixeia = [ ");
					for(int w = 0; w < W; w++) 
						printf("%lf ", b_read.data[w]);
					printf("]\n");
					#endif

					v_dt c_prev = (k > 0) ? c_buffer[tn] : v_dt{static_cast<ValueType>(0.)};
					
					#ifndef __SYNTHESIS__
					printf("---> k = %d, c_prev = [ ", k);
					for(int w = 0; w < W; w++)
						printf("%lf ", c_prev.data[w]);
					printf("]\n");
					#endif

					// c_buffer[tn] = c_prev + a_buffer * b_read;
					v_dt c_buf_curr;
					computation_part_loop5_w___:
					for(int w = 0; w < W; w++) {
						#pragma HLS UNROLL
						c_buf_curr.data[w] = c_prev.data[w] + alpha * a_buffer * b_read.data[w];
						
						#ifndef __SYNTHESIS__
						printf("---> c_buf_curr.data[%d] = %lf + %lf * %lf = %lf\n", w, c_prev.data[w], a_buffer, b_read.data[w], c_buf_curr.data[w]);
						#endif
					}
					c_buffer[tn] = c_buf_curr;
					#pragma HLS DEPENDENCE variable=c_buffer false
					// Forward to subsequent PE, if any
					if (d < D - 1) {
						B_Stream_out << b_read;
					}
				}

			} // End loop over K

			// Write out result block
			computation_part_loop3_tm___:
			for (int tm = 0; tm < d + 1; tm++) { // Different for each PE
				computation_part_loop4_tn_2___:
				for (int tn = 0; tn < TN; tn++) {
					#pragma HLS PIPELINE II=1
					#pragma HLS LOOP_FLATTEN
					v_dt c_val{};
					if (d > 0 && tm > 0) {
						c_val = C_Stream_in.read();
					} else {
						c_val = c_buffer[tn];
					}
					C_Stream_out << c_val;
					
					#ifndef __SYNTHESIS__
					printf("writing C_val = [ ");
					for(int w = 0; w < W; w++)
						printf("%lf ", c_val.data[w]);
					printf("] to C_Stream_out\n");
					#endif
				}
			}
		}
	}
}

extern "C" {

// (int M,     int K,   int N, 
// ValueType alpha, ValueType beta,
// ValueType *A, v_dt *B, v_dt *C_in, v_dt *C_out)
void krnl_gemm(const char TransA, const char TransB,
	const int M, const int N, const int K, 
	const ValueType alpha, const ValueType *A, const int lda, 
	const v_dt *B, const int ldb, 
	const ValueType beta, const v_dt *C_in, v_dt *C_out, const int ldc)
{
	#pragma HLS INTERFACE m_axi port = A offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = B offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = C_in offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = C_out offset = slave bundle = gmem3

	// #pragma HLS INTERFACE s_axilite port = Layout
	#pragma HLS INTERFACE s_axilite port = TransA
	#pragma HLS INTERFACE s_axilite port = TransB
	#pragma HLS INTERFACE s_axilite port = M
	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = K
	#pragma HLS INTERFACE s_axilite port = alpha
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = ldb
	#pragma HLS INTERFACE s_axilite port = beta
	#pragma HLS INTERFACE s_axilite port = ldc

	static hls::stream<ValueType> A_Stream[D + 1];
	static hls::stream<v_dt>      B_Stream[D + 1];
	static hls::stream<v_dt>      C_Stream[D + 1];
	
	static hls::stream<v_dt>      C_Stream_in;
	#pragma HLS STREAM variable = C_Stream_in    depth = 256

	#pragma HLS STREAM variable = A_Stream[0]    depth = 256
	#pragma HLS STREAM variable = B_Stream[0]    depth = 256
	#pragma HLS STREAM variable = C_Stream[0]    depth = 256

	#pragma HLS STREAM variable = A_Stream[1]    depth = 256
	#pragma HLS STREAM variable = B_Stream[1]    depth = 256
	#pragma HLS STREAM variable = C_Stream[1]    depth = 256

	#pragma HLS STREAM variable = A_Stream[2]    depth = 256
	#pragma HLS STREAM variable = B_Stream[2]    depth = 256
	#pragma HLS STREAM variable = C_Stream[2]    depth = 256

	#pragma HLS STREAM variable = A_Stream[3]    depth = 256
	#pragma HLS STREAM variable = B_Stream[3]    depth = 256
	#pragma HLS STREAM variable = C_Stream[3]    depth = 256
	
	#pragma HLS STREAM variable = A_Stream[4]    depth = 256
	#pragma HLS STREAM variable = B_Stream[4]    depth = 256
	#pragma HLS STREAM variable = C_Stream[4]    depth = 256

	#ifndef __SYNTHESIS__
	if( M % D != 0){
		printf("M mod D != 0 ! ABORT\n");
		return;
	}
	if( N % (TN*W) != 0){
		printf("N mod (TN*W) != 0 ! ABORT\n");
		return;
	}
	#endif

	#pragma HLS DATAFLOW
	read_A(A, A_Stream[0], M, K, N, alpha, beta);
	read_B(B, B_Stream[0], M, K, N, alpha, beta);
	read_C_in(C_in, C_Stream_in, M, N, alpha, beta);

	computation_part(A_Stream[0], A_Stream[1],    B_Stream[0], B_Stream[1],    C_Stream[0], C_Stream[1],    M,K,N,    alpha, beta,   0);
	computation_part(A_Stream[1], A_Stream[2],    B_Stream[1], B_Stream[2],    C_Stream[1], C_Stream[2],    M,K,N,    alpha, beta,   1);
	computation_part(A_Stream[2], A_Stream[3],    B_Stream[2], B_Stream[3],    C_Stream[2], C_Stream[3],    M,K,N,    alpha, beta,   2);
	computation_part(A_Stream[3], A_Stream[4],    B_Stream[3], B_Stream[4],    C_Stream[3], C_Stream[4],    M,K,N,    alpha, beta,   3);

	write_C_out(C_Stream[D], C_Stream_in, C_out, M, N, alpha, beta);
}

}
