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
static void swap(hls::stream< float>& Xin, hls::stream< float>& Yin,hls::stream< float>& Xout, hls::stream< float>& Yout, int N) {

execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        Xout << Yin.read();
        Yout << Xin.read();
    }
}


extern "C" {
void krnl_swap(int N,float* X,int incx,  float* Y, int incy) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy

    static hls::stream<float> Xin;
    static hls::stream<float> Xout;
    static hls::stream<float> Yin;
    static hls::stream<float> Yout;
#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yin depth = 128
#pragma HLS STREAM variable = Xout depth = 128
#pragma HLS STREAM variable = Yout depth = 128


#pragma HLS dataflow
            read_vector(X, Xin, N,incx);
            read_vector(Y, Yin, N,incy);
            swap(Xin,Yin,Xout,Yout,N);
            write_vector(X, Xout,N,incx);
            write_vector(Y, Yout,N,incy);
}
}
