void dot(hls::stream< v_dt>& Xin, hls::stream< v_dt>& Yin, int N,float* result) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

	v_dt Xtemp;
	v_dt Ytemp;
	float temp_result[VDATA_SIZE];
	float final_result = 0;
	#pragma HLS ARRAY_PARTITION variable=temp_result dim=1 complete

	for(int i=0;i<VDATA_SIZE;i++){
		#pragma HLS unroll
		temp_result[i]=0;
	}

	execute:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
    	Xtemp=Xin.read();
    	Ytemp=Yin.read();
    	for (int j=0;j<VDATA_SIZE;j++){
			#pragma HLS unroll
    		if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
    			temp_result[j] +=(Xtemp.data[j]*Ytemp.data[j]);
    		}
    		else{
    			temp_result[j] +=0;
    		}
    	}
    }

    accum:
    for (int j=0;j<VDATA_SIZE;j++){
		#pragma HLS pipeline II=1
    	final_result+=temp_result[j];
    }

    result[0]=final_result;
}

extern "C" {
void krnl_dot(const int N,v_dt* X,const int incx,  v_dt* Y, const int incy,float* result) {
	#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
	#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = hbm1
	#pragma HLS INTERFACE m_axi port = result offset = slave bundle = hbm2

	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = incx
	#pragma HLS INTERFACE s_axilite port = incy


	static hls::stream<v_dt> Xin;
	static hls::stream<v_dt> Yin;
	#pragma HLS STREAM variable = Xin depth = 128
	#pragma HLS STREAM variable = Yin depth = 128
	#pragma HLS dataflow

		read_vector_wide((v_dt*)X, Xin, N,incx);
        read_vector_wide((v_dt*)Y, Yin, N,incy);
	    dot(Xin,Yin,N,result);


}
}
