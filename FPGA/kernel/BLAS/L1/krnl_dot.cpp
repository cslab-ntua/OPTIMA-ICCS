#include "../libs/read_write.hpp"
void read_vector(float* in, hls::stream<float>& inStream, int N, int incx) {
mem_rd:
	int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        inStream << in[index];
        index=index+incx;
    }
}

static float dot(hls::stream< float>& Xin, hls::stream< float>& Yin, int N) {
	float ret=0;
execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        ret=ret+ ((Xin.read())*(Yin.read()));
    }
    return ret;
}

extern "C" {
void krnl_dot(int N,float* X,int incx,  float* Y, int incy,float* result) {
#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy
#pragma HLS INTERFACE s_axilite port = result

    static hls::stream<float> Xin;
    static hls::stream<float> Yin;
#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yin depth = 128

#pragma HLS dataflow

		read_vector(X, Xin, N,incx);
        read_vector(Y, Yin, N,incy);
	    *result=dot(Xin,Yin,N);


}
}
