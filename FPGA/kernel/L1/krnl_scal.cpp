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

void write_vector(float* out, hls::stream< float>& outStream, int N,int incy) {
mem_wr:
int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        out[index] = outStream.read();
        index=index+incy;
    }
}
static void scal(hls::stream< float>& Xin, hls::stream< float>& Yout, int N,float SA) {

execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        Yout << (Xin.read()*SA);
    }
}

extern "C" {
void krnl_scal(int N,float* X,int incx,  float* Y, int incy, float SA) {

#pragma HLS XinTERFACE m_aXini port = Xin offset = slave bundle = ddr0
#pragma HLS XinTERFACE m_aXini port = out offset = slave bundle = ddr0

#pragma HLS XinTERFACE s_aXinilite port = N
#pragma HLS XinTERFACE s_aXinilite port = incx
#pragma HLS XinTERFACE s_aXinilite port = incy
#pragma HLS XinTERFACE s_aXinilite port = SA

	static hls::stream<float> Xin;
	    static hls::stream<float> Yout;
#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yout depth = 128


#pragma HLS dataflow

	    read_vector(X, Xin, N,incx);
	    scal(Xin,Yout,N,SA);
	    write_vector(Y, Yout,N,incy);
}
}
