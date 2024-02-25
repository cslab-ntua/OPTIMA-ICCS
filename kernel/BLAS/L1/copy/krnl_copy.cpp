#include "global.hpp"
#include "common.hpp"

extern "C" {
void krnl_copy(const int N, v_dt* X,const int incX,  v_dt* Y, const int incY) { //for incX and incY = 1 at the moment

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = hbm1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incX
#pragma HLS INTERFACE s_axilite port = incY





#pragma HLS dataflow
static hls::stream<v_dt> Xtemp;

#pragma HLS STREAM variable = Xtemp depth = 128
	read_vector_wide((v_dt*)X, Xtemp, N,incX);
    write_vector_wide((v_dt*)Y, Xtemp, N,incY);
}
}
