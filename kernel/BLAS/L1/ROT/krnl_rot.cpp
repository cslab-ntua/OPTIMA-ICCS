void rot (hls::stream< v_dt>& Xin, hls::stream< v_dt>& Yin,hls::stream< v_dt>& Xout, hls::stream< v_dt>& Yout, const int N,const float C,const float S) {

	float temp_x[VDATA_SIZE];
	float temp_y[VDATA_SIZE];
	#pragma HLS ARRAY_PARTITION temp_x dim=1 complete
	#pragma HLS ARRAY_PARTITION temp_y dim=1 complete
    	v_dt x,y;
execute:
	for (int i = 0; i < N; i+=VDATA_SIZE) {
	#pragma HLS pipeline II=1
    	x=Xin.read();
		y=Yin.read();
        for(int j=0;j<VDATA_SIZE;j++){
        #pragma HLS unroll
        	if(i+VDATA_SIZE<=N || j<(N%VDATA_SIZE)){
        		temp_x[j]=x.data[j];
        		temp_y[j]=y.data[j];
        		x.data[j]= (C*temp_x[j]+S*temp_y[j]);
        		y.data[j]= (-S*temp_x[j]+C*temp_y[j]);
        	}
        	else{
        		x.data[j]=0;
        		y.data[j]=0;
        	}
        }
        Xout << x;
        Yout <<y;
	}

}


extern "C" {
void krnl_rot(const int N,v_dt* X,const int incx,  v_dt* Y, const int incy, const float C, const float S) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = hbm1


#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy
#pragma HLS INTERFACE s_axilite port = C
#pragma HLS INTERFACE s_axilite port = S

  static hls::stream<v_dt> Xin;
  static hls::stream<v_dt> Yin;
  static hls::stream<v_dt> Xout;
  static hls::stream<v_dt> Yout;

#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yin depth = 128
#pragma HLS STREAM variable = Xout depth = 128
#pragma HLS STREAM variable = Yout depth = 128

#pragma HLS dataflow

    read_vector_wide((v_dt*)X, Xin, N,incx);
    read_vector_wide((v_dt*)Y, Yin, N,incy);
    rot(Xin,Yin,Xout,Yout,N,C,S);
    write_vector_wide((v_dt*)X, Xout, N,incx);
    write_vector_wide((v_dt*)Y, Yout, N,incy);
}
}
