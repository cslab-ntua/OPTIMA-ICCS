#include <stdio.h>
#include <hls_stream.h>
#include "ap_int.h"
#include "global.hpp"
#include "common.hpp"

void rotm (hls::stream< v_dt>& Xin, hls::stream< v_dt>& Yin,hls::stream< v_dt>& Xout, hls::stream< v_dt>& Yout, const int N,const float *P) {

	float pis[5];
	float temp_x[VDATA_SIZE];
	float temp_y[VDATA_SIZE];
	#pragma HLS ARRAY_PARTITION temp_x dim=1 complete
	#pragma HLS ARRAY_PARTITION temp_y dim=1 complete
	#pragma HLS ARRAY_PARTITION pis dim=1 complete

	fetch_P:
	for (int i=0;i<5;i++){
		#pragma HLS pipeline II=1
		pis[i]=P[i];
	}

	if(pis[0]==0){
		pis[1]=1;
		pis[4]=1;
		}
	else if(pis[0]==1){
		pis[2]=1;
		pis[3]=-1;
	}
	else if(pis[0]==-2){
		pis[1]=1;
		pis[2]=0;
		pis[3]=0;
		pis[4]=1;
	}

	v_dt x,y;
	execute:
	for (int i = 0; i < N; i+=VDATA_SIZE) {
	#pragma HLS pipeline II=1
    	x=Xin.read();
		y=Yin.read();
		for(int j=0;j<VDATA_SIZE;j++){
		#pragma HLS unroll
			if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
				temp_x[j]=x.data[j];
				temp_y[j]=y.data[j];
				x.data[j]= (pis[1]*temp_x[j]+pis[2]*temp_y[j]);
				y.data[j]= (pis[3]*temp_x[j]+pis[4]*temp_y[j]);
		 	}
		   	else{
		   		x.data[j]=0;
		   		y.data[j]=0;
		   	}
		}
		Xout <<x;
		Yout<<y;
	}
}


extern "C" {
void krnl_rotm(const int N,v_dt* X,const int incx,  v_dt* Y, const int incy, const float *P) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = hbm1
#pragma HLS INTERFACE m_axi port = P offset = slave bundle = hbm2

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy

static hls::stream<v_dt> Xin;
static hls::stream<v_dt> Yin;
static hls::stream<v_dt> Xout;
static hls::stream<v_dt> Yout;
#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yin depth = 128
#pragma HLS STREAM variable = Xout depth = 128
#pragma HLS STREAM variable = Yout depth = 128

#pragma HLS dataflow

    read_vector_wide((v_dt*)X, Xin, N,incx);
    read_vector_wide((v_dt*)Y, Yin, N,incy);
    rotm(Xin,Yin,Xout,Yout,N,P);
    write_vector_wide((v_dt*)X, Xout, N,incx);
    write_vector_wide((v_dt*)Y, Yout, N,incy);
}
}
