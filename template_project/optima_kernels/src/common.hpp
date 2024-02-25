#ifndef COMMON_HPP
#define COMMON_HPP
#include "global.hpp"

/**
 * Find maximum between two numbers.
 */
inline float abs_float(float x){
    return (x>=0) ? x : -x;

}

inline float max_float(float x, float y){
    return (x>=y) ? x :y;
}

inline int max_(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

/**
 * Find minimum between two numbers.
 */
inline int min_(int num1, int num2)
{
    return (num1 > num2 ) ? num2 : num1;
}

/**
 * Set itteration (multiplication with incX/incY)
 */
inline int set_iterration(int i,const int incY){
	return i*incY;
}
inline float multiply_floats(float x,float y){
    return (x*y);
}
inline float multiply_float_stream(float x,hls::stream<float>& out){
    return(x*out.read());
}
inline float multiply_streams_of_floats(hls::stream<float>& out1,hls::stream<float>& out2){
    return (out1.read()*out2.read());
}
inline float add_floats(float x,float y){
	return (x+y);
}
inline float add_float_stream(float x,hls::stream<float>& out){
	return (x+out.read());
}
inline float add_streams_of_floats(hls::stream<float>& out1,hls::stream<float>& out2){
	return (out1.read()+out2.read());
}
inline void add_streams_to_stream(hls::stream<float>& in1,hls::stream<float>& in2,hls::stream<float>& out){
	out << (in1.read()+in2.read());
}
inline void add_streams_to_stream_N_items(hls::stream<float>& in1,hls::stream<float>& in2,hls::stream<float>& out,const int N){
	for (int i=0;i<N;i++){
		out << (in1.read()+in2.read());
	}
}


inline void scal(hls::stream< float>& Xin, hls::stream< float>& Xout,const int N,const float SA) {

execute:
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        Xout << (Xin.read()*SA);
    }
}

inline void gemv(hls::stream<float>& Ain,float *x_b,const int start,const int end,hls::stream<float>& Ytemp) {
    float Yout=0;
execute:
   for (int i = start; i < end; i++) {
	#pragma HLS pipeline II=1
	   //unroll
        Yout = Yout+(Ain.read()*x_b[i]);
    }
   Ytemp<<Yout;

}

inline void gemv_block(hls::stream<float>& Ain,float *x_b,const int rows,const int columns,hls::stream<float>& Ytemp) {
execute_gemv_block:
   for (int i = 0; i < rows; i++) {
    #pragma HLS pipeline II=1
       gemv(Ain,x_b,0,columns,Ytemp);
   }
}

#endif
