
#include "../libs/read_write.hpp"
void read_vector(const float* in, hls::stream<float>& inStream, const int N, const int incX) {
mem_rd:
	int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        inStream << in[index];
        index=index+incX;
    }
}

void write_vector(float* out, hls::stream< float>& outStream, const int N,const int incY) {
mem_wr:
int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        out[index] = outStream.read();
        index=index+incY;
    }
}

static void copy(hls::stream< float>& Xin, hls::stream< float>& Yout,const int N) {

execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        Yout << Xin.read();
    }
}


extern "C" {
void krnl_copy(const int N,const float* X,const int incX,  float* Y, const int incY) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr0

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incX
#pragma HLS INTERFACE s_axilite port = incY

static hls::stream<float> Xin;
static hls::stream<float> Yout;

#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yout depth = 128


#pragma HLS dataflow

    read_vector(X, Xin, N,incX);
    copy(Xin, Yout, N);
    write_vector(Y, Yout, N,incY);
}
}
