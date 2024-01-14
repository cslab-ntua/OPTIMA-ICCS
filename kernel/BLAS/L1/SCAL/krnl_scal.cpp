void write_vector_wide_scal(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int incy,const float alpha) {

mem_wr:
    for (int i = 0; i < N; i+=VDATA_SIZE) {
	#pragma HLS pipeline II=1
    	v_dt temp=outStream.read();
    	for(int j=0;j<VDATA_SIZE;j++){
    		#pragma HLS unroll
    		if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
        		temp.data[j]=temp.data[j]*alpha;
    		}
    		else{
    			temp.data[j]=0;
    		}
    	}
        out[i/VDATA_SIZE] = temp;
    }
}


extern "C" {
void krnl_scal(const int N,const float alpha,v_dt* X,const int incx) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = alpha


	static hls::stream<v_dt> Xtemp;

#pragma HLS STREAM variable = Xtemp depth = 1024


#pragma HLS dataflow

	    read_vector_wide((v_dt*)X, Xtemp, N,incx);
	    write_vector_wide_scal((v_dt*)X, Xtemp,N,incx,alpha);
}
}
