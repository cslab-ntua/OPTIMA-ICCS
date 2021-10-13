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

void write_vector(float* out, hls::stream< float>& outStream, int N,int incy) {
mem_wr:
int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        out[index] = outStream.read();
        index=index+incy;
    }
}
static void rotm (hls::stream< float>& Xin, hls::stream< float>& Yin,hls::stream< float>& Xout,hls::stream< float>& Yout, int N, float* P) {

execute:
float sh[2][2];
	sh[1][1]=P[1];
	sh[1][2]=1;
	sh[2][1]=-1;
	sh[2][2]=P[4];
	if(P[0]==-1){
		sh[1][1]=P[1];
		sh[1][2]=P[3];
		sh[2][1]=P[2];
		sh[2][2]=P[4];
	}
	else if(P[0]==0){
			sh[1][1]=1;
			sh[1][2]=P[3];
			sh[2][1]=P[2];
			sh[2][2]=1;
	}
#pragma HLS array_partition variable=P block factor=5
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
    	Xout << (sh[1][1]*Xin.read()+sh[1][2]*Yin.read());
    	Yout << (sh[2][1]*Yin.read()+sh[2][2]*Xin.read());
    }
}


extern "C" {
void krnl_rotm(int N,float* X,int incx,  float* Y, int incy, float* P) {

#pragma HLS INTERFACE m_axi port = X offset = slave bundle = ddr0
#pragma HLS INTERFACE m_axi port = Y offset = slave bundle = ddr1
#pragma HLS INTERFACE m_axi port = P offset = slave bundle = params

#pragma HLS INTERFACE s_axilite port = N
#pragma HLS INTERFACE s_axilite port = incx
#pragma HLS INTERFACE s_axilite port = incy




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

