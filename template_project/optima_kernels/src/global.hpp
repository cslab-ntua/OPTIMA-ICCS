#ifndef READ_WRITE_HPP
#define READ_WRITE_HPP

#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include "common.hpp"

#define VDATA_SIZE 16
typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;
typedef struct v_intdatatype { int data[VDATA_SIZE]; } v_idt;
#define LU_PL_ENGINES 4
#define LU_MAX_N 1024
#define LU_BRAM_DEPTH 720
#define LU_PL_ENGINE_0 0
#define LU_PL_ENGINE_1 1
#define LU_PL_ENGINE_2 2
#define LU_PL_ENGINE_3 3
#define LU_LROWS_BRAM LU_MAX_N

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

inline int read_vector_wide_lu(v_dt* in, hls::stream<v_dt>& inStream,const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_1:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        inStream << in[i+start];
    }

    return 0;
}

inline int read_vector_wide_lu_df(v_dt* in, hls::stream<v_dt>& inStream,const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_1_df:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        inStream << in[i+start];
    }

    return 0;
}

inline void read_vector_wide_in_float_bram2d_lu(v_dt* in, float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_2:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram[i][j] = in[i+start].data[j];
        }
    }
}

inline void read_vector_wide_in_float_bram2d_lu_12(v_dt* in, v_dt* bram,const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;
	#pragma HLS array_partition variable=tmpData type=complete dim=1

mem_rd_2b:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

    	tmpData = in[i+start];

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram[i].data[j] = tmpData.data[j];
        }
    }
}

inline void store_stream_wide_in_float_bram2d_lu(hls::stream<v_dt>& inStream, float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_3:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

    	v_dt tmpData = inStream.read();

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram[i][j] = tmpData.data[j];
        }
    }
}

inline void broadcast_vector_wide_in_4float_bram2d_lu(v_dt* in, float bram0[LU_BRAM_DEPTH][VDATA_SIZE], float bram1[LU_BRAM_DEPTH][VDATA_SIZE], float bram2[LU_BRAM_DEPTH][VDATA_SIZE], float bram3[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_4:

    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram0[i][j] = in[i+start].data[j];
        	bram1[i][j] = in[i+start].data[j];
        	bram2[i][j] = in[i+start].data[j];
        	bram3[i][j] = in[i+start].data[j];
        }
    }
}

inline void broadcast_vector_wide_in_2float_bram2d_lu(v_dt* in, float bram0[LU_BRAM_DEPTH][VDATA_SIZE], float bram1[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_5:

    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	bram0[i][j] = in[i+start].data[j];
        	bram1[i][j] = in[i+start].data[j];
        }
    }
}

inline void write_vector_wide_from_float_bram2d_lu(v_dt* out, float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_wr_1:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	out[i+start].data[j] = bram[i][j];
        }
    }
}

inline void write_vector_wide_from_float_bram1d_lu(v_dt* out, float bram[LU_LROWS_BRAM],const int N, const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_wr_2:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
	#pragma HLS loop_tripcount min=1 max=512

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
        	out[i+start].data[j] = bram[i*VDATA_SIZE+j];
        }
    }
}

inline int write_vector_wide_to_output_and_bram2d_lu(v_dt* out, hls::stream<v_dt>& outStream,float bram[LU_BRAM_DEPTH][VDATA_SIZE],const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

mem_wr_3:
    for (int i = 0; i < vSize; i++) {
		#pragma HLS pipeline II=1

    	tmpData = outStream.read();

        out[i+start] = tmpData;
        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
			bram[i][j] = tmpData.data[j];
		}
    }

	return 0;
}

inline int write_vector_wide_to_output_and_bram2d_lu_12(v_dt* out, hls::stream<v_dt>& outStream,v_dt* bram,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;
	#pragma HLS array_partition variable=tmpData type=complete dim=1

mem_wr_3b:
    for (int i = 0; i < vSize; i++) {
		#pragma HLS pipeline II=1

    	tmpData = outStream.read();

        for (int j=0;j<VDATA_SIZE;j++) {
			#pragma HLS unroll
			bram[i].data[j] = tmpData.data[j];
		}

        out[i+start] = tmpData;
    }

	return 0;
}

inline int write_vector_wide_lu(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

mem_wr_4:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();
        out[i+start] = tmpData;
    }

	return 0;
}

inline int write_vector_wide_lu_df(v_dt* out, hls::stream<v_dt>& outStream,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	v_dt tmpData;

mem_wr_4_df:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();

        out[i+start] = tmpData;
    }

	return 0;
}

inline int write_wide_float_stream_lu(v_dt* out, hls::stream<float>& outStream,const int N,const int start) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

	v_dt tmpData;
	#pragma HLS array_partition variable=tmpData type=complete dim=1

mem_wr_5:
    for (int i = 0; i < vSize; i++) {
    	for (int j=0;j<VDATA_SIZE;j++) {
		#pragma HLS pipeline II=1
		//#pragma HLS latency max=3
    		if (i*VDATA_SIZE+j < N) {
    			tmpData.data[j] = outStream.read();
    		}
    		else {
    			tmpData.data[j] = 0.0f;
    		}
    	}

    	out[i+start] = tmpData;
    }

    return 0;
}

inline void read_vector_wide_in_float_bram(v_dt* in, float *bram,const int N, const int incx) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_6:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
        for (int j=0;j<VDATA_SIZE;j++) {
        	int bramIdx = i*vSize+j;
        	bram[bramIdx] = in->data[j];
        }
    }
}

inline void read_vector_wide_in_int_bram(v_idt* in, int *bram,const int N, const int incx) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;

mem_rd_7:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1
        for (int j=0;j<VDATA_SIZE;j++) {
        	int bramIdx = i*vSize+j;
        	bram[bramIdx] = in->data[j];
        }
    }
}




inline int write_vector_wide_out_and_to_buffer_lu(v_dt* out, v_dt* offChipBuf, hls::stream<v_dt>& outStream,const int N,const int outStart,const int bufStart) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	static v_dt tmpData;

mem_wr_7:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();

    	offChipBuf[i+bufStart] = tmpData;
        out[i+outStart] = tmpData;
    }

	return 0;
}

inline int write_vector_wide_out_and_to_stream_lu(v_dt* out, hls::stream<v_dt>& bufStream, hls::stream<v_dt>& outStream,const int N,const int outStart) {
	unsigned int vSize = ((N - 1) / VDATA_SIZE) + 1;
	static v_dt tmpData;

mem_wr_8:
    for (int i = 0; i < vSize; i++) {
	#pragma HLS pipeline II=1

    	tmpData = outStream.read();

    	bufStream.write(tmpData);
        out[i+outStart] = tmpData;
    }

	return 0;
}

#endif
