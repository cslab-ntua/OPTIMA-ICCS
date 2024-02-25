#include <stdio.h>
#include <hls_stream.h>
#include "ap_int.h"
#include "global.hpp"
#include "common.hpp"

void write_vector_wide_axpy(v_dt* out, hls::stream<v_dt>& Xtemp, hls::stream<v_dt>& Ytemp ,const int N,const int incy,const float alpha) {
	v_dt X;
	v_dt Y;
	v_dt temp;

mem_wr:
    for (int i = 0; i < N; i+=VDATA_SIZE) {
	#pragma HLS pipeline II=1
    	X=Xtemp.read();
    	Y=Ytemp.read();
    	for(int j=0;j<VDATA_SIZE;j++){
		#pragma HLS unroll
    		if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
        		temp.data[j]=Y.data[j] + alpha*X.data[j];
    		}
    		else{
    			temp.data[j]=0;
    		}
    	}
        out[i/VDATA_SIZE] = temp;
    }
}

extern "C" {
void krnl_axpy(const int N,v_dt* X,const int incx,  v_dt* Y,const  int incy,const float alpha) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = hbm1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy
#pragma HLS INTERFACE s_axilite port = alpha

    static hls::stream<v_dt> Xtemp;
    static hls::stream<v_dt> Ytemp;

#pragma HLS STREAM variable = Xtemp depth = 128
#pragma HLS STREAM variable = Ytemp depth = 128


#pragma HLS dataflow

    read_vector_wide((v_dt*)X, Xtemp, N,incx);
    read_vector_wide((v_dt*)Y, Ytemp, N,incy);
    write_vector_wide_axpy((v_dt*)Y, Xtemp, Ytemp,N,incy,alpha);
}
}
