 void iamax(hls::stream< v_dt>& Xin,const  int N, int *result) {
	v_dt temp;
	int i_max[VDATA_SIZE];
	float maxi[VDATA_SIZE];
	float temp_number[VDATA_SIZE];
	#pragma HLS ARRAY_PARTITION variable=i_max dim=1 complete
	#pragma HLS ARRAY_PARTITION variable=maxi dim=1 complete
	#pragma HLS ARRAY_PARTITION variable=temp_number dim=1 complete

	temp=Xin.read();

	init_iamax:
	for(int j=0;j<VDATA_SIZE;j++){
		#pragma HLS unroll
		maxi[j]=abs_float(temp.data[j]);
		i_max[j]=j;
	}

	execute_iamax:
	for (int i = VDATA_SIZE; i < N; i+=VDATA_SIZE) {
		#pragma HLS pipeline II=1
    	temp=Xin.read();
		for(int j=0;j<VDATA_SIZE;j++){
			#pragma HLS unroll
			if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
				temp_number[j]=abs_float(temp.data[j]);
				if(temp_number[j]>maxi[j]){
					maxi[j]=temp_number[j];
					i_max[j]=i+j;
				}

			 }
		}
	}

	final:
	for (int i=1;i<VDATA_SIZE;i++){
		#pragma HLS pipeline II=1
		if(maxi[i]>maxi[0]){
				maxi[0]=maxi[i];
				i_max[0]=i_max[i];
		}
	}

    result[0]= i_max[0];
}

extern "C" {
void krnl_iamax(const int N,v_dt* X,const int incx,int* result) {
#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = result offset = slave bundle = hbm1

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx


static hls::stream<v_dt> Xin;

#pragma HLS STREAM variable = Xin depth = 128

#pragma HLS dataflow

		read_vector_wide((v_dt*)X, Xin, N,incx);
	    iamax(Xin,N,result);


}
}
