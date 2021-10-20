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

static float iamax(hls::stream< float>& Xin,const int N) {
   float max=Yin.read();
   float i_max=0;
execute:
    for (int i = 1; i < N; i++) {
	#pragma HLS pipeline II=1
        item=Yin.read()
        if (item>max){
    	 max=item;
         i_max=i;
        }
    }
    return i_max;
}


extern "C" {
void krnl_iamax(const int N,const float* X,const int incx, float* result) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = result

    static hls::stream<float> Xin;

#pragma HLS STREAM variable = Xin depth = 128

#pragma HLS dataflow

    read_vector(X, Xin, N,incx);
    *result =iamax(Xin,N);
}
}
