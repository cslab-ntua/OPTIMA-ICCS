#include "read_write.h"

static void copy(hls::stream< float>& inStream, hls::stream< float>& outStream, int size) {

execute:
#pragma HLS pipeline II=1
    for (int i = 0; i < size; i++) {

        outStream << inStream.read();
    }
}

static void read(float* in, hls::stream<float>& inStream, int size, int incx) {
mem_rd:
#pragma HLS pipeline II=1
	int index=0;
    for (int i = 0; i < size; i++) {
        inStream << in[index];
        index=index+incx;
    }
}

static void write(float* out, hls::stream< float>& outStream, int size,int incy) {
mem_wr:
#pragma HLS pipeline II=1
int index=0;
    for (int i = 0; i < size; i++) {
        out[index] = outStream.read();
        index=index+incy;
    }
}
extern "C" {
void krnl_copy(int size,float* in,int incx,  float* out, int incy) {

#pragma HLS INTERFACE m_axi port = in offset = slave bundle = one
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = one

#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy

    static hls::stream<float> inStream;
    static hls::stream<float> outStream;
#pragma HLS STREAM variable = inStream depth = 100
#pragma HLS STREAM variable = outStream depth = 100


#pragma HLS dataflow

    read(in, inStream, size,incx);
    copy(inStream, outStream, size);
    write(out, outStream, size,incy);
}
}
