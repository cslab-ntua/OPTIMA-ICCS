#include "../libs/read_write.hpp"
void read_vector(const float* in, hls::stream<float>& inStream, const int N, const int incx) {
mem_rd:
	int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        inStream << in[index];
        index=index+incx;
    }
}

static float ddot(hls::stream< float>& Xin, hls::stream< float>& Yin,const int N) {
	double result_d=0;
	float result=0;
execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        result_d=result_d+((Xin.read())*(Yin.read()));
    }
    result=result_d;
    return result;
}


extern "C" {
void krnl_ddot(const int N,const float alpha,const float* X,const int incx,const float* Y, const int incy, float* result) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = alpha
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
            *result=alpha+ddot(Xin,Yin,N);

}
}
