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

static int iamax(hls::stream< float>& Xin, int N) {
	float max=Xin.read();
	int i_max=0;
	float item;
execute:
    for (int i = 1; i < N; i++) {
	#pragma HLS pipeline II=1
        item=Xin.read();
        if(item<0){
        	item=-item;
        }
        if (item>max){
        	max=item;
        	i_max=i;
        }
    }
    return i_max;
}

extern "C" {
void krnl_iamax(int N,float* X,int incx,int* result) {
#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = result

    static hls::stream<float> Xin;

#pragma HLS STREAM variable = Xin depth = 128

#pragma HLS dataflow

		read_vector(X, Xin, N,incx);
	    *result=iamax(Xin,N);


}
}
