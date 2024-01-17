extern "C" {
void krnl_swap(const int N,v_dt* X,const int incx,  v_dt* Y, const int incy) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = hbm1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy

	static hls::stream<v_dt> Xtemp;
	static hls::stream<v_dt> Ytemp;

#pragma HLS STREAM variable = Ytemp depth = 128
#pragma HLS STREAM variable = Xtemp depth = 128

#pragma HLS dataflow
            read_vector_wide((v_dt*)X, Xtemp, N,incx);
            read_vector_wide((v_dt*)Y, Ytemp, N,incy);
            write_vector_wide((v_dt*)X, Ytemp,N,incx);
            write_vector_wide((v_dt*)Y, Xtemp,N,incy);
}
}
