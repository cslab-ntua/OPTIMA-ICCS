#include "../libs/read_write.hpp"
void read_vector(const float* in, hls::stream<float>& inStream, const int N,const int incx) {
mem_rd:
	int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        inStream << in[index];
        index=index+incx;
    }
}

void write_vector(float* out, hls::stream< float>& outStream,const int N,const int incy) {
mem_wr:
int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        out[index] = outStream.read();
        index=index+incy;
    }
}

static void axpy(hls::stream< float>& Xin, hls::stream< float>& Yin,hls::stream< float>& Yout, const int N,const float SA) {
execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
    	Yout << (Yin.read()+SA*Xin.read());
    }
}


extern "C" {
void krnl_axpy(const int N,const float* X,const int incx,  float* Y, const int incy,const float SA) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy
#pragma HLS INTERFACE s_axilite port = SA

    static hls::stream<float> Xin;
    static hls::stream<float> Yin;
    static hls::stream<float> Yout;

#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yin depth = 128
#pragma HLS STREAM variable = Yout depth = 128


#pragma HLS dataflow

    read_vector(X, Xin, N,incx);
    read_vector(Y, Yin, N,incy);
    axpy(Xin,Yin,Yout,N,SA);
    write_vector(Y, Yout,N,incy);
}
}
