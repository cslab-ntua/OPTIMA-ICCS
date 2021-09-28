// Includes
#include "read_write.h"

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

