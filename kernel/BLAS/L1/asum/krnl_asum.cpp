void asum(hls::stream< v_dt>& Xin, int N, float* final_result) {

	float result[VDATA_SIZE];
	float temp_final=0;
	#pragma HLS ARRAY_PARTITION result dim=1 complete
	v_dt temp;
	init_asum:
	for(int i=0;i<VDATA_SIZE;i++){
		#pragma HLS unroll
		result[i]=0;
	}
	excecute_asum:
	for (int i = 0; i < N; i+=VDATA_SIZE) {
		#pragma HLS pipeline II=1
    	temp=Xin.read();

    	for(int j=0;j<VDATA_SIZE;j++){
			#pragma HLS unroll
			if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
				result[j] +=abs_float(temp.data[j]);
			}
			else{
				result[j] +=0;
			}
		}
	}

	excecute_final_of_sum:
	for (int i=0;i<VDATA_SIZE;i++){
		#pragma HLS pipeline II=1
		temp_final+=result[i];
	}

	final_result[0]=temp_final;
}

extern "C" {
void krnl_asum(int N,v_dt* X,int incx,float* result) {
#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = result offset = slave bundle = hbm1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx

    static hls::stream<v_dt> Xin;

#pragma HLS STREAM variable = Xin depth = 128

#pragma HLS dataflow
		read_vector_wide((v_dt*)X, Xin, N,incx);
	    asum(Xin,N,result);


}
}
