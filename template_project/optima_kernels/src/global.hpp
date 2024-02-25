#ifndef READ_WRITE_HPP
#define READ_WRITE_HPP

#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include "common.hpp"

#define VDATA_SIZE 16
typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

inline void write_from_float(float *Y,int place,float item){
	Y[place]=item;
}

inline void write_from_fifo(float *Y,int place,hls::stream<float>& outStream){
	Y[place]=outStream.read();
}
inline void write_multiple_items_from_fifo(float *Y,hls::stream<float>& outStream, int start, int end,const int inc){
	for (int i= start ; i<end;i+=inc){
		Y[i]=outStream.read();
	}
}


inline void Y_init(float *Y,const float beta,const int size){
    init_Y:
    for(int i=0;i<size;i++){
        #pragma HLS unroll factor=4
        Y[i]=beta*Y[i];
    }
}
inline int set_x_b_and_x_zero_countdown(const int block_size,const int index,float *X,const int incX,float *x_b,const float alpha){
	int local_index;
	if (alpha==(float)1){
		set_x_1:
		local_index=index;
		for(int i=block_size-1;i>=0;i--){
			x_b[i]=X[local_index];
			X[local_index]=0;
			local_index-=incX;
		}
	}
	else{
		set_x_2:
		local_index=index;
	    for(int i=block_size-1;i>=0;i--){
	        x_b[i]=X[local_index]*alpha;
	        X[local_index]=0;
	        local_index-=incX;
	    }
	}
    return local_index;
}
inline int set_x_b_and_x_zero(const int block_size,int index,float *X,const int incX,float *x_b,const float alpha){
	int local_index;
	if (alpha==(float)1){
		set_x_1:
		local_index=index;
		for(int i=0;i<block_size;i++){
			x_b[i]=X[local_index];
			X[local_index]=0;
			local_index+=incX;
		}
	}
	else{
		set_x_2:
		local_index=index;
	    for(int i=0;i<block_size;i++){
	        x_b[i]=X[local_index]*alpha;
	        X[local_index]=0;
	        local_index+=incX;
	    }
	}

    return local_index;
}
inline int set_x_b_countdown(const int block_size,const int index,float *X,const int incX,float *x_b,const float alpha){
	int local_index;
	if (alpha==(float)1){
		set_x_1:
		local_index=index;
		for(int i=block_size-1;i>=0;i--){
			x_b[i]=X[local_index];
			local_index-=incX;
		}
	}
	else{
		set_x_2:
		local_index=index;
	    for(int i=block_size-1;i>=0;i--){
	        x_b[i]=X[local_index]*alpha;
	        local_index-=incX;
	    }
	}

    return local_index;
}

inline int set_x_b(const int block_size,const int index,const float *X,const int incX,float *x_b,const float alpha){
	int local_index;
	if (alpha==(float)1){
		set_x_1:
		local_index=index;
		for(int i=0;i<block_size;i++){
			x_b[i]=X[local_index];
			local_index+=incX;
		}
	}
	else{
		set_x_2:
		local_index=index;
		for(int i=0;i<block_size;i++){
			x_b[i]=X[local_index]*alpha;
			local_index+=incX;
		}
	}
    return local_index;
}

inline void read_vector_symmetric_matrix(const float* in, hls::stream<float>& inStream,  int start, int length,const char Uplo,const int N) {
mem_rd:
	int final_index,counter=0;
    if(Uplo=='U' or Uplo=='u'){
        for (int i = start; i < start+length; i++) {
        	final_index=i+counter*N;
            for (int j=i; j <=final_index ; j+=N){
                #pragma HLS pipeline II=1
                inStream << in[j];
            }
            counter++;
        }
    }
//needs fixing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    else if(Uplo=='L' or Uplo=='l'){
    }
}
inline void read_matrix_tile(const float* in, hls::stream<float>& inStream,  int start, int rows,int columns,const int N) {
mem_rd:
int final=start+N*rows;
    for (int i = start; i < final; i+=N) {
		for (int j = i; j < i+columns; j++) {
        	#pragma HLS pipeline II=1
			inStream << in[j];
		}
    }
}
inline void read_matrix_tile_transpose(const float* in, hls::stream<float>& inStream,  int start, int rows,int columns,const int N) {
mem_rd:
int final=0;
    for (int i = start; i < start+rows; i++) {
		final=i+N*columns;
		for (int j = i; j < final; j+=N) {
        	#pragma HLS pipeline II=1
			inStream << in[j];
		}
    }
}

inline void read_vector_matrix(const float* in, hls::stream<float>& inStream,  int start, int end,const int incx) {
mem_rd:
    for (int i = start; i < end; i+=incx) {
        #pragma HLS pipeline II=1
        inStream << in[i];
    }
}


inline void read_vector(float* in, hls::stream<float>& inStream,const int N, const int incx) {
mem_rd:
    for (int i = 0; i < N; i+=incx) {
	#pragma HLS pipeline II=1
        inStream << in[i];
    }
}

inline void write_vector(float* out, hls::stream< float>& outStream,const int N,const int incy) {
mem_wr:
int index=0;
    for (int i = 0; i < N; i++) {
	#pragma HLS pipeline II=1
        out[index] = outStream.read();
        index=index+incy;
    }
}
inline float fetch(float* out,int i){
	return out[i];
}
inline void set(float* out,int i,float item){
	out[i]=item;
}

inline void read_vector_wide(v_dt* in, hls::stream<v_dt>& inStream,const int N, const int incx) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
        inStream << in[i];
    }
}

inline void write_vector_wide(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int incy) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_wr:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
        out[i] = outStream.read();
    }
}

#endif
