#include <stdio.h>
#include <hls_stream.h>
#include "ap_int.h"

typedef float ValueType;

#define W 16

typedef ap_uint<32>  ColType;
const unsigned int c_dt_size = W;
typedef struct v_datatype { ValueType data[W]; } v_dt;

// typedef struct v_datatypeint { ColType data[W]; } v_dti;

typedef ap_uint<1> RowType;

typedef struct v_datatyperow { RowType data[W]; } v_dtr;

void read_input_streams_iat_coef(v_dtr *iat, hls::stream<v_dtr> &iatStream, v_dt *coef, hls::stream<v_dt> &coefStream, int nterm) {
	unsigned int vSize = ((nterm - 1) / W) + 1;
	mem_rd_ja_coef:
	for (int i = 0; i < vSize; i++) {
    	#pragma HLS PIPELINE II=1
		iatStream << iat[i];
		coefStream << coef[i];
	}
}

void read_x(v_dt *x, hls::stream<v_dt> &xStream, int nterm) {
	unsigned int vSize = ((nterm - 1) / W) + 1;
	mem_rd_x:
	for (int i = 0; i < vSize; i++) {
    	#pragma HLS PIPELINE II=1
		xStream << x[i];
	}
}


// void read_x(hls::stream<v_dti> &ja, ValueType *x, hls::stream<v_dt> &xStream, hls::stream<v_dtr> &iatStream, int nterm) {
// 	rd_x:
// 	ColType col;
// 	v_dti coltmp;
// 	v_dt xtmp;
// 	v_dtr rowtmp;

// 	LOOP_rd_x:
// 	for(int i = 0; i < nterm; i+=W) {
//   		#pragma HLS PIPELINE
// 		coltmp = ja.read();
// 		LOOP_INNER_rd_x:
// 		for(int k = 0; k < W; k++) {
//     		#pragma HLS UNROLL
// 			col = coltmp.data[k].range(0,30);
// 			rowtmp.data[k] = coltmp.data[k].range(31,31);
// 			xtmp.data[k] = x[col];
// 		}
// 		xStream << xtmp;
// 		iatStream << rowtmp;
// 	}
// }

void spmv_csr(hls::stream<v_dt> &coef, hls::stream<v_dtr> &iat, hls::stream<v_dt> &xStream, hls::stream<ValueType> &b, int nterm) {
	execute:
	RowType m1;
	ValueType sum=0;
	ColType col;

	v_dt datatmp;
	v_dt xtmp;
	v_dtr rowtmp;

	ValueType temp;
	ValueType temp2;

	ValueType sum_p[W];
	#pragma HLS ARRAY_PARTITION variable=sum_p complete dim=1
	LOOP_init_sum_p: 
	for(int k = 0; k < W; k++) {
		#pragma HLS UNROLL
		sum_p[k] = 0;
	}
	ValueType tmp_red1, tmp_red2, tmp_red3, tmp_red4, tmp_red5, tmp_red6, tmp_red7, tmp_red8, tmp_red9, tmp_red10, tmp_red11, tmp_red12, tmp_red13, tmp_red14;

	LOOP_execute:
	for(int i = 0; i < nterm; i+=W) {
	    #pragma HLS PIPELINE II=2 rewind
		// #pragma HLS PIPELINE II=16 rewind
		

		datatmp = coef.read();
		xtmp    = xStream.read();
		rowtmp   = iat.read();

		LOOP_INNER2_execute:
		for (int k = 0; k < W; k++){
			#pragma HLS UNROLL
			temp  = datatmp.data[k];
			temp2 = xtmp.data[k];
			// if(temp!=0){
			sum_p[k] += temp*temp2;
			// }
			m1 = rowtmp.data[k];
		}
		
		// LOOP_INNER_sum_p: 
		// for (int k = 0; k < W; k++) {
		// 	#pragma HLS UNROLL
		// 	sum += sum_p[k];
		// }
		tmp_red1 = sum_p[0] + sum_p[8];
		tmp_red2 = sum_p[1] + sum_p[9];
		tmp_red3 = sum_p[2] + sum_p[10];
		tmp_red4 = sum_p[3] + sum_p[11];
		tmp_red5 = sum_p[4] + sum_p[12];
		tmp_red6 = sum_p[5] + sum_p[13];
		tmp_red7 = sum_p[6] + sum_p[14];
		tmp_red8 = sum_p[7] + sum_p[15];

		tmp_red9  = tmp_red1 + tmp_red5;
		tmp_red10 = tmp_red2 + tmp_red6;
		tmp_red11 = tmp_red3 + tmp_red7;
		tmp_red12 = tmp_red4 + tmp_red8;

		tmp_red13 =  tmp_red9 + tmp_red11;
		tmp_red14 = tmp_red10 + tmp_red12;

		sum = tmp_red13 + tmp_red14;

		if(m1==1){
			b << sum;
			// sum = 0;
			LOOP_INNER_init_sum_p: 
			for(int k = 0; k < W; k++) {
				#pragma HLS UNROLL
				sum_p[k] = 0;
			}

		}
	}
}

void write_b(ValueType *b, hls::stream< ValueType> &bStream, int nrows) {
	mem_wr:
	for (int i = 0; i < nrows; i++) {
	    #pragma HLS PIPELINE II=1
		b[i] = bStream.read();
	}
}

extern "C" {

void krnl_SpMV(int num_runs, int nrows, int nterm, v_dtr *iat, v_dt *coef, v_dt *x, ValueType *b) {
	#pragma HLS INTERFACE m_axi port = iat  offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = coef offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = x num_read_outstanding=64  depth=512 offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = b num_write_outstanding=64 depth=512 offset = slave bundle = gmem0

	#pragma HLS INTERFACE s_axilite port = num_runs
	#pragma HLS INTERFACE s_axilite port = nrows
	#pragma HLS INTERFACE s_axilite port = nterm

	static hls::stream<v_dtr> iatStream;
	static hls::stream<v_dt> coefStream;
	static hls::stream<v_dt> xStream;
	static hls::stream<ValueType> bStream;

	#pragma HLS STREAM variable = coefStream    depth = 512
	#pragma HLS STREAM variable = bStream       depth = 512
	#pragma HLS STREAM variable = iatStream     depth = 512
	#pragma HLS STREAM variable = xStream       depth = 512

	for(int i=0; i<num_runs; i++){
		#pragma HLS dataflow
		read_input_streams_iat_coef(iat, iatStream, coef, coefStream, nterm);
		read_x(x, xStream, nterm);
		spmv_csr(coefStream, iatStream, xStream, bStream, nterm);
		write_b(b, bStream, nrows);
	}
}

}
