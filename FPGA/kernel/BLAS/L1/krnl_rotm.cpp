#include "../libs/read_write.hpp"

static void rotm (hls::stream< float>& Xin, hls::stream< float>& Yin,hls::stream< float>& Xout,hls::stream< float>& Yout, const int N,const float* P ) {

execute:
#pragma HLS array_partition variable=sh block factor=4 dim=1
float sh[2][2];
	sh[0][0]=P[1];
	sh[1][0]=P[2];
	sh[0][1]=P[3];
	sh[1][1]=P[4];

	if(P[0]==1){
		sh[0][0]=(float)1;
		sh[1][0]=(float)(-1);
	}
	else if(P[0]==0){
			sh[0][1]=(float)1;
			sh[1][0]=(float)1;
	}

    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
    	Xout << (sh[0][0]*Xin.read()+sh[0][1]*Yin.read());
    	Yout << (sh[1][0]*Yin.read()+sh[1][1]*Xin.read());
    }
}


extern "C" {
void krnl_rotm(const int N,float* X,const int incx,  float* Y, const int incy,const float* P) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr1


#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy
#pragma HLS INTERFACE s_axilite port = P


    static hls::stream<float> Xin;
    static hls::stream<float> Xout;
    static hls::stream<float> Yin;
    static hls::stream<float> Yout;
#pragma HLS STREAM variable = Xin depth = 128
#pragma HLS STREAM variable = Yin depth = 128
#pragma HLS STREAM variable = Xout depth = 128
#pragma HLS STREAM variable = Yout depth = 128

#pragma HLS dataflow

    read_vector(X, Xin, N,incx);
    read_vector(Y, Yin, N,incy);
    rotm(Xin,Yin,Xout,Yout,N,P);
    write_vector(X, Xout,N,incx);
    write_vector(Y, Yout,N,incy);

}
}

