#include <hls_math.h>
#include "../include/global.hpp"
#include "../include/common.hpp"

inline void write_vector_wide_jacobi(v_dt* out, hls::stream<v_dt>& Xtemp,const int N) {
	v_dt X;
	v_dt temp;
	int outIdx = 0;

mem_wr:
	for (int i = 0; i < N; i+=VDATA_SIZE) {
		#pragma HLS pipeline II=1
    	X=Xtemp.read();

    	for(int j=0;j<VDATA_SIZE;j++){
			#pragma HLS unroll
    		if (X.data[j] == 0.0f) {
    			temp.data[j] = 0.0f;
    		}
    		else {
    			temp.data[j]=hls::recip(X.data[j]);
    		}
    	}

        out[outIdx] = temp;
        outIdx +=1;
    }
}

extern "C" {
	void krnl_jacobi(
			const int N0,v_dt* X0,v_dt* Y0,
			const int N1,v_dt* X1,v_dt* Y1,
			const int N2,v_dt* X2,v_dt* Y2,
			const int N3,v_dt* X3,v_dt* Y3,
			const int N4,v_dt* X4,v_dt* Y4,
			const int N5,v_dt* X5,v_dt* Y5,
			const int N6,v_dt* X6,v_dt* Y6,
			const int N7,v_dt* X7,v_dt* Y7,
			const int N8,v_dt* X8,v_dt* Y8,
			const int N9,v_dt* X9,v_dt* Y9,
			const int N10,v_dt* X10,v_dt* Y10,
			const int N11,v_dt* X11,v_dt* Y11,
			const int N12,v_dt* X12,v_dt* Y12,
			const int N13,v_dt* X13,v_dt* Y13,
			const int N14,v_dt* X14,v_dt* Y14,
			const int N15,v_dt* X15,v_dt* Y15
		) {

		#pragma HLS INTERFACE m_axi port = X0 offset = slave bundle = hbm0
		#pragma HLS INTERFACE m_axi port = Y0 offset = slave bundle = hbm1
		#pragma HLS INTERFACE s_axilite port = N0
		#pragma HLS INTERFACE m_axi port = X1 offset = slave bundle = hbm2
		#pragma HLS INTERFACE m_axi port = Y1 offset = slave bundle = hbm3
		#pragma HLS INTERFACE s_axilite port = N1
		#pragma HLS INTERFACE m_axi port = X2 offset = slave bundle = hbm4
		#pragma HLS INTERFACE m_axi port = Y2 offset = slave bundle = hbm5
		#pragma HLS INTERFACE s_axilite port = N2
		#pragma HLS INTERFACE m_axi port = X3 offset = slave bundle = hbm6
		#pragma HLS INTERFACE m_axi port = Y3 offset = slave bundle = hbm7
		#pragma HLS INTERFACE s_axilite port = N3
		#pragma HLS INTERFACE m_axi port = X4 offset = slave bundle = hbm8
		#pragma HLS INTERFACE m_axi port = Y4 offset = slave bundle = hbm9
		#pragma HLS INTERFACE s_axilite port = N4
		#pragma HLS INTERFACE m_axi port = X5 offset = slave bundle = hbm10
		#pragma HLS INTERFACE m_axi port = Y5 offset = slave bundle = hbm11
		#pragma HLS INTERFACE s_axilite port = N5
		#pragma HLS INTERFACE m_axi port = X6 offset = slave bundle = hbm12
		#pragma HLS INTERFACE m_axi port = Y6 offset = slave bundle = hbm13
		#pragma HLS INTERFACE s_axilite port = N6
		#pragma HLS INTERFACE m_axi port = X7 offset = slave bundle = hbm14
		#pragma HLS INTERFACE m_axi port = Y7 offset = slave bundle = hbm15
		#pragma HLS INTERFACE s_axilite port = N7
		#pragma HLS INTERFACE m_axi port = X8 offset = slave bundle = hbm16
		#pragma HLS INTERFACE m_axi port = Y8 offset = slave bundle = hbm17
		#pragma HLS INTERFACE s_axilite port = N8
		#pragma HLS INTERFACE m_axi port = X9 offset = slave bundle = hbm18
		#pragma HLS INTERFACE m_axi port = Y9 offset = slave bundle = hbm19
		#pragma HLS INTERFACE s_axilite port = N9
		#pragma HLS INTERFACE m_axi port = X10 offset = slave bundle = hbm20
		#pragma HLS INTERFACE m_axi port = Y10 offset = slave bundle = hbm21
		#pragma HLS INTERFACE s_axilite port = N10
		#pragma HLS INTERFACE m_axi port = X11 offset = slave bundle = hbm22
		#pragma HLS INTERFACE m_axi port = Y11 offset = slave bundle = hbm23
		#pragma HLS INTERFACE s_axilite port = N11
		#pragma HLS INTERFACE m_axi port = X12 offset = slave bundle = hbm24
		#pragma HLS INTERFACE m_axi port = Y12 offset = slave bundle = hbm25
		#pragma HLS INTERFACE s_axilite port = N12
		#pragma HLS INTERFACE m_axi port = X13 offset = slave bundle = hbm26
		#pragma HLS INTERFACE m_axi port = Y13 offset = slave bundle = hbm27
		#pragma HLS INTERFACE s_axilite port = N13
		#pragma HLS INTERFACE m_axi port = X14 offset = slave bundle = hbm28
		#pragma HLS INTERFACE m_axi port = Y14 offset = slave bundle = hbm29
		#pragma HLS INTERFACE s_axilite port = N14
		#pragma HLS INTERFACE m_axi port = X15 offset = slave bundle = hbm30
		#pragma HLS INTERFACE m_axi port = Y15 offset = slave bundle = hbm31
		#pragma HLS INTERFACE s_axilite port = N15

		#pragma HLS dataflow

		static hls::stream<v_dt> Xtemp0;
		#pragma HLS STREAM variable = Xtemp0 depth = 128
		static hls::stream<v_dt> Xtemp1;
		#pragma HLS STREAM variable = Xtemp1 depth = 128
		static hls::stream<v_dt> Xtemp2;
		#pragma HLS STREAM variable = Xtemp2 depth = 128
		static hls::stream<v_dt> Xtemp3;
		#pragma HLS STREAM variable = Xtemp3 depth = 128
		static hls::stream<v_dt> Xtemp4;
		#pragma HLS STREAM variable = Xtemp4 depth = 128
		static hls::stream<v_dt> Xtemp5;
		#pragma HLS STREAM variable = Xtemp5 depth = 128
		static hls::stream<v_dt> Xtemp6;
		#pragma HLS STREAM variable = Xtemp6 depth = 128
		static hls::stream<v_dt> Xtemp7;
		#pragma HLS STREAM variable = Xtemp7 depth = 128
		static hls::stream<v_dt> Xtemp8;
		#pragma HLS STREAM variable = Xtemp8 depth = 128
		static hls::stream<v_dt> Xtemp9;
		#pragma HLS STREAM variable = Xtemp9 depth = 128
		static hls::stream<v_dt> Xtemp10;
		#pragma HLS STREAM variable = Xtemp10 depth = 128
		static hls::stream<v_dt> Xtemp11;
		#pragma HLS STREAM variable = Xtemp11 depth = 128
		static hls::stream<v_dt> Xtemp12;
		#pragma HLS STREAM variable = Xtemp12 depth = 128
		static hls::stream<v_dt> Xtemp13;
		#pragma HLS STREAM variable = Xtemp13 depth = 128
		static hls::stream<v_dt> Xtemp14;
		#pragma HLS STREAM variable = Xtemp14 depth = 128
		static hls::stream<v_dt> Xtemp15;
		#pragma HLS STREAM variable = Xtemp15 depth = 128

		read_vector_wide(X0,Xtemp0,N0,1);
		write_vector_wide_jacobi(Y0,Xtemp0,N0);
		read_vector_wide(X1,Xtemp1,N1,1);
		write_vector_wide_jacobi(Y1,Xtemp1,N1);
		read_vector_wide(X2,Xtemp2,N2,1);
		write_vector_wide_jacobi(Y2,Xtemp2,N2);
		read_vector_wide(X3,Xtemp3,N3,1);
		write_vector_wide_jacobi(Y3,Xtemp3,N3);
		read_vector_wide(X4,Xtemp4,N4,1);
		write_vector_wide_jacobi(Y4,Xtemp4,N4);
		read_vector_wide(X5,Xtemp5,N5,1);
		write_vector_wide_jacobi(Y5,Xtemp5,N5);
		read_vector_wide(X6,Xtemp6,N6,1);
		write_vector_wide_jacobi(Y6,Xtemp6,N6);
		read_vector_wide(X7,Xtemp7,N7,1);
		write_vector_wide_jacobi(Y7,Xtemp7,N7);
		read_vector_wide(X8,Xtemp8,N8,1);
		write_vector_wide_jacobi(Y8,Xtemp8,N8);
		read_vector_wide(X9,Xtemp9,N9,1);
		write_vector_wide_jacobi(Y9,Xtemp9,N9);
		read_vector_wide(X10,Xtemp10,N10,1);
		write_vector_wide_jacobi(Y10,Xtemp10,N10);
		read_vector_wide(X11,Xtemp11,N11,1);
		write_vector_wide_jacobi(Y11,Xtemp11,N11);
		read_vector_wide(X12,Xtemp12,N12,1);
		write_vector_wide_jacobi(Y12,Xtemp12,N12);
		read_vector_wide(X13,Xtemp13,N13,1);
		write_vector_wide_jacobi(Y13,Xtemp13,N13);
		read_vector_wide(X14,Xtemp14,N14,1);
		write_vector_wide_jacobi(Y14,Xtemp14,N14);
		read_vector_wide(X15,Xtemp15,N15,1);
		write_vector_wide_jacobi(Y15,Xtemp15,N15);
	}
}
