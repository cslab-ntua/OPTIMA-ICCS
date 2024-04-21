#include <stdio.h>
#include <hls_stream.h>
#include "ap_int.h"
#include "global.hpp"
#include "common.hpp"

 void iamin(hls::stream< v_dt>& Xin,const  int N, int *result) {
	v_dt temp;
	int i_min[VDATA_SIZE];
	float mini[VDATA_SIZE];
	float temp_number[VDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable=i_min dim=1 complete
	#pragma HLS ARRAY_PARTITION variable=mini dim=1 complete
	#pragma HLS ARRAY_PARTITION variable=temp_number dim=1 complete

	temp=Xin.read();

	init_iamin:
	for(int j=0;j<VDATA_SIZE;j++){
		#pragma HLS unroll
		mini[j]=abs_float(temp.data[j]);
		i_min[j]=j;
	}

	execute_iamin:
	for (int i = VDATA_SIZE; i < N; i+=VDATA_SIZE) {
		#pragma HLS pipeline II=1
    	temp=Xin.read();
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma HLS unroll
			if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
				temp_number[j]=abs_float(temp.data[j]);
				if(temp_number[j]<mini[j]){
					mini[j]=temp_number[j];
					i_min[j]=i+j;
				}

			 }
		}
	}

	final:
	for (int i=1;i<VDATA_SIZE;i++){
		#pragma HLS pipeline II=1
		if(mini[i]<mini[0]){
				mini[0]=mini[i];
				i_min[0]=i_min[i];
		}
	}

    result[0]= i_min[0];
}

extern "C" {
void krnl_iamin(const int N,v_dt* X,const int incx,int* result) {
#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = result offset = slave bundle = hbm1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx


static hls::stream<v_dt> Xin;

#pragma HLS STREAM variable = Xin depth = 128

#pragma HLS dataflow

		read_vector_wide((v_dt*)X, Xin, N,incx);
	    iamin(Xin,N,result);


}
}
