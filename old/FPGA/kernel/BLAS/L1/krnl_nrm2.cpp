#include "../libs/read_write.hpp"
#include <math.h>
void read_vector(float* in, hls::stream<float>& inStream, int N, int incx) {
mem_rd:
	int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        inStream << in[index];
        index=index+incx;
    }
}


static float nrm2(hls::stream< float>& Xin, int N) {
	float ret=0;
	float item;
execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
    	item=Xin.read();
        ret=ret+(item*item); // check performance for that
    }
    ret =sqrtf(ret);
    return ret;
}

extern "C" {
void krnl_nrm2(int N,float* X,int incx,float* result) {
#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = result

    static hls::stream<float> Xin;

#pragma HLS STREAM variable = Xin depth = 128

#pragma HLS dataflow

		read_vector(X, Xin, N,incx);
	    *result=nrm2(Xin,N);


}
}
