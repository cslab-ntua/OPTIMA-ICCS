// #include "../../include/global.hpp"
// #include "../../include/common.hpp"
#include <stdio.h>
#include <hls_stream.h>
// #include <ap_int.h>
#include <hls_math.h>
#include "ap_int.h"

typedef float ValueType;

#define BRAM_SIZE 1024 // it used to be 1792! had to change it for 16 "Compute Units"
#define W 16
typedef struct v_datatype { ValueType data[W]; } v_dt;

void read_b(hls::stream<ValueType>& Bstream1, hls::stream<ValueType>& Bstream2, hls::stream<ValueType>& Bstream3, hls::stream<ValueType>& Bstream4, hls::stream<ValueType>& Bstream5, hls::stream<ValueType>& Bstream6, hls::stream<ValueType>& Bstream7, hls::stream<ValueType>& Bstream8, hls::stream<ValueType>& Bstream9, hls::stream<ValueType>& Bstream10, hls::stream<ValueType>& Bstream11, hls::stream<ValueType>& Bstream12, hls::stream<ValueType>& Bstream13, hls::stream<ValueType>& Bstream14, hls::stream<ValueType>& Bstream15, hls::stream<ValueType>& Bstream16, 
	const ValueType *B, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	read_b:
	for(int i=0;i<K;i++){
		#pragma HLS pipeline II=1
		if(i<row_break0){
			Bstream1 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream1\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break1){
			Bstream2 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream2\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break2){
			Bstream3 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream3\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break3){
			Bstream4 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream4\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break4){
			Bstream5 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream5\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break5){
			Bstream6 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream6\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break6){
			Bstream7 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream7\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break7){
			Bstream8 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream8\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break8){
			Bstream9 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream9\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break9){
			Bstream10 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream10\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break10){
			Bstream11 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream11\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break11){
			Bstream12 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream12\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break12){
			Bstream13 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream13\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break13){
			Bstream14 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream14\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else if(i<row_break14){
			Bstream15 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream15\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
		else{
			Bstream16 << B[i];
			#ifndef __SYNTHESIS__
			printf("Bstream16\tforwarded i = %ld, value = %f\n", i, B[i]);
			#endif
		}
	}
}

void read_matrix1(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=0;

	wide_read1:
	while(i<row_break0){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix1) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		//if(j==K/W-1){
		if(j>=row_break0/W-1){
			i++;
			//j=hls::fmax(i-start_offset,0)/W;
			j=i/W;
			printf(">>>>>>>>> in read_matrix1 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix2(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break0)/W;

	wide_read2:
	while(i<row_break1){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix2) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break1/W-1){
			i++;
			j=hls::fmax(i,row_break0)/W;
			printf(">>>>>>>>> in read_matrix2 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix3(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break1)/W;

	wide_read3:
	while(i<row_break2){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix3) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break2/W-1){
			i++;
			j=hls::fmax(i,row_break1)/W;
			printf(">>>>>>>>> in read_matrix3 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix4(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break2)/W;

	wide_read4:
	while(i<row_break3){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];

		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix4) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break3/W-1){
			i++;
			j=hls::fmax(i,row_break2)/W;
			printf(">>>>>>>>> in read_matrix4 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix5(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break3)/W;;

	wide_read5:
	while(i<row_break4){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix5) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break4/W-1){
			i++;
			j=hls::fmax(i,row_break3)/W;
			printf(">>>>>>>>> in read_matrix5 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix6(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break4)/W;

	wide_read6:
	while(i<row_break5){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix6) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break5/W-1){
			i++;
			j=hls::fmax(i,row_break4)/W;
			printf(">>>>>>>>> in read_matrix6 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix7(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break5)/W;

	wide_read7:
	while(i<row_break6){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix7) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break6/W-1){
			i++;
			j=hls::fmax(i,row_break5)/W;
			printf(">>>>>>>>> in read_matrix7 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix8(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break6)/W;

	wide_read8:
	while(i<row_break7){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];

		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix8) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break7/W-1){
			i++;
			j=hls::fmax(i,row_break6)/W;
			printf(">>>>>>>>> in read_matrix8 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix9(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break7)/W;

	wide_read9:
	while(i<row_break8){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix9) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		//if(j==K/W-1){
		if(j>=row_break8/W-1){
			i++;
			//j=hls::fmax(i-start_offset,0)/W;
			j=hls::fmax(i,row_break7)/W;
			printf(">>>>>>>>> in read_matrix9 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix10(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break8)/W;

	wide_read10:
	while(i<row_break9){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix10) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break9/W-1){
			i++;
			j=hls::fmax(i,row_break8)/W;
			printf(">>>>>>>>> in read_matrix10 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix11(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break9)/W;

	wide_read11:
	while(i<row_break10){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix11) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break10/W-1){
			i++;
			j=hls::fmax(i,row_break9)/W;
			printf(">>>>>>>>> in read_matrix11 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix12(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break10)/W;

	wide_read12:
	while(i<row_break11){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];

		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix12) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break11/W-1){
			i++;
			j=hls::fmax(i,row_break10)/W;
			printf(">>>>>>>>> in read_matrix12 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix13(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break11)/W;;

	wide_read13:
	while(i<row_break12){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix13) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break12/W-1){
			i++;
			j=hls::fmax(i,row_break11)/W;
			printf(">>>>>>>>> in read_matrix13 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix14(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break12)/W;

	wide_read14:
	while(i<row_break13){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix14) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break13/W-1){
			i++;
			j=hls::fmax(i,row_break12)/W;
			printf(">>>>>>>>> in read_matrix14 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix15(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break13)/W;

	wide_read15:
	while(i<row_break14){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];
	
		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix15) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=row_break14/W-1){
			i++;
			j=hls::fmax(i,row_break13)/W;
			printf(">>>>>>>>> in read_matrix15 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void read_matrix16(const v_dt *A, hls::stream<v_dt>& AStream, const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	int i=0, j=hls::fmax(i,row_break14)/W;

	wide_read16:
	while(i<K){
		#pragma HLS pipeline II=1
		v_dt Atemp=A[i*(K/W)+j];

		#ifndef __SYNTHESIS__
		printf("index i*(K/W)+j = %d -> Atemp (from read_matrix16) [ ", i*(K/W)+j);
		for(int w = 0; w < W; w++)
			printf("%-5.1f ", Atemp.data[w]);
		printf("]\n");
		#endif
	
		AStream << Atemp;

		if(j>=K/W-1){
			i++;
			j=hls::fmax(i,row_break14)/W;
			printf(">>>>>>>>> in read_matrix16 special... i = %d, j = %d\n", i, j);
		}
		else{
			j++;
		}
	}
}

void trsv_sync1(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_1_to_2, hls::stream<ValueType>& Bcast_1_to_3, hls::stream<ValueType>& Bcast_1_to_4, hls::stream<ValueType>& Bcast_1_to_5, hls::stream<ValueType>& Bcast_1_to_6, hls::stream<ValueType>& Bcast_1_to_7, hls::stream<ValueType>& Bcast_1_to_8, hls::stream<ValueType>& Bcast_1_to_9, hls::stream<ValueType>& Bcast_1_to_10, hls::stream<ValueType>& Bcast_1_to_11, hls::stream<ValueType>& Bcast_1_to_12, hls::stream<ValueType>& Bcast_1_to_13, hls::stream<ValueType>& Bcast_1_to_14, hls::stream<ValueType>& Bcast_1_to_15, hls::stream<ValueType>& Bcast_1_to_16, 	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync1_loop_i__:
	for(int i=0;i<row_break0;i++){
		ValueType Xtemp=0;
		if(i%2==0){
			ValueType bstream_tmp = BStream.read();
			Xtemp = bstream_tmp - bram_ping[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i)%W][(i)/W]);
			#endif
		}
		else{
			ValueType bstream_tmp = BStream.read();
			Xtemp = bstream_tmp - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i)%W][(i)/W]);
			#endif
		}

		int j=i;
		trsv_sync1_loop_j__:
		while(j<row_break0){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

			
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream1     from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_2 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_2  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_3 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_3  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_4 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_4  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_5 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_5  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_6 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_6  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_7 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_7  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_8  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_9  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_10  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_11  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_12  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_13  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_14  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_15  from trsv_sync1 %f\n", i, Xtemp);
			#endif
			Bcast_1_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_1_to_16  from trsv_sync1 %f\n", i, Xtemp);
			#endif

	}
}

void trsv_sync2(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_2, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_2_to_3, hls::stream<ValueType>& Bcast_2_to_4, hls::stream<ValueType>& Bcast_2_to_5, hls::stream<ValueType>& Bcast_2_to_6, hls::stream<ValueType>& Bcast_2_to_7, hls::stream<ValueType>& Bcast_2_to_8, hls::stream<ValueType>& Bcast_2_to_9, hls::stream<ValueType>& Bcast_2_to_10, hls::stream<ValueType>& Bcast_2_to_11, hls::stream<ValueType>& Bcast_2_to_12, hls::stream<ValueType>& Bcast_2_to_13, hls::stream<ValueType>& Bcast_2_to_14, hls::stream<ValueType>& Bcast_2_to_15, hls::stream<ValueType>& Bcast_2_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync2_loop_i__:
	for(int i=0;i<row_break1;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_2.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_2.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_2 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break0)%W][(i-row_break0)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break0)%W][(i-row_break0)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break0)%W][(i-row_break0)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break0)%W][(i-row_break0)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break0,0);
		trsv_sync2_loop_j__:
		while(j<(row_break1-row_break0)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break0){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream2     from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_3 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_3  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_4 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_4  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_5 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_5  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_6 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_6  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_7 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_7  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_8  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_9  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_10  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_11  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_12  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_13  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_14  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_15  from trsv_sync2 %f\n", i, Xtemp);
			#endif
			Bcast_2_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_2_to_16  from trsv_sync2 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync3(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_3, hls::stream<ValueType>& Bcast_2_to_3, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_3_to_4, hls::stream<ValueType>& Bcast_3_to_5, hls::stream<ValueType>& Bcast_3_to_6, hls::stream<ValueType>& Bcast_3_to_7, hls::stream<ValueType>& Bcast_3_to_8, hls::stream<ValueType>& Bcast_3_to_9, hls::stream<ValueType>& Bcast_3_to_10, hls::stream<ValueType>& Bcast_3_to_11, hls::stream<ValueType>& Bcast_3_to_12, hls::stream<ValueType>& Bcast_3_to_13, hls::stream<ValueType>& Bcast_3_to_14, hls::stream<ValueType>& Bcast_3_to_15, hls::stream<ValueType>& Bcast_3_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync3_loop_i__:
	for(int i=0;i<row_break2;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_3.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_3.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_3 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_3.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_3.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_3 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break1)%W][(i-row_break1)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break1)%W][(i-row_break1)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break1)%W][(i-row_break1)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break1)%W][(i-row_break1)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break1,0);
		trsv_sync3_loop_j__:
		while(j<(row_break2-row_break1)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break1){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream3     from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_4 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_4  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_5 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_5  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_6 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_6  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_7 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_7  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_8  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_9  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_10  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_11  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_12  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_13  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_14  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_15  from trsv_sync3 %f\n", i, Xtemp);
			#endif
			Bcast_3_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_3_to_16  from trsv_sync3 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync4(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_4, hls::stream<ValueType>& Bcast_2_to_4, hls::stream<ValueType>& Bcast_3_to_4, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_4_to_5, hls::stream<ValueType>& Bcast_4_to_6, hls::stream<ValueType>& Bcast_4_to_7, hls::stream<ValueType>& Bcast_4_to_8, hls::stream<ValueType>& Bcast_4_to_9, hls::stream<ValueType>& Bcast_4_to_10, hls::stream<ValueType>& Bcast_4_to_11, hls::stream<ValueType>& Bcast_4_to_12, hls::stream<ValueType>& Bcast_4_to_13, hls::stream<ValueType>& Bcast_4_to_14, hls::stream<ValueType>& Bcast_4_to_15, hls::stream<ValueType>& Bcast_4_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync4_loop_i__:
	for(int i=0;i<row_break3;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_4.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_4.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_4 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_4.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_4.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_4 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_4.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_4.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_4 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break2)%W][(i-row_break2)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break2)%W][(i-row_break2)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break2)%W][(i-row_break2)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break2)%W][(i-row_break2)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break2,0);
		trsv_sync4_loop_j__:
		while(j<(row_break3-row_break2)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break2){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream4     from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_5 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_5  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_6 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_6  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_7 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_7  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_8  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_9  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_10  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_11  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_12  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_13  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_14  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_15  from trsv_sync4 %f\n", i, Xtemp);
			#endif
			Bcast_4_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_4_to_16  from trsv_sync4 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync5(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_5, hls::stream<ValueType>& Bcast_2_to_5, hls::stream<ValueType>& Bcast_3_to_5, hls::stream<ValueType>& Bcast_4_to_5, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_5_to_6, hls::stream<ValueType>& Bcast_5_to_7, hls::stream<ValueType>& Bcast_5_to_8, hls::stream<ValueType>& Bcast_5_to_9, hls::stream<ValueType>& Bcast_5_to_10, hls::stream<ValueType>& Bcast_5_to_11, hls::stream<ValueType>& Bcast_5_to_12, hls::stream<ValueType>& Bcast_5_to_13, hls::stream<ValueType>& Bcast_5_to_14, hls::stream<ValueType>& Bcast_5_to_15, hls::stream<ValueType>& Bcast_5_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync5_loop_i__:
	for(int i=0;i<row_break4;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_5.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_5.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_5 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_5.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_5.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_5 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_5.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_5.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_5 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_5.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_5.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_5 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break3)%W][(i-row_break3)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break3)%W][(i-row_break3)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break3)%W][(i-row_break3)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break3)%W][(i-row_break3)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break3,0);
		trsv_sync5_loop_j__:
		while(j<(row_break4-row_break3)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break3){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream5     from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_6 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_6  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_7 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_7  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_8  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_9  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_10  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_11  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_12  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_13  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_14  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_15  from trsv_sync5 %f\n", i, Xtemp);
			#endif
			Bcast_5_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_5_to_16  from trsv_sync5 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync6(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_6, hls::stream<ValueType>& Bcast_2_to_6, hls::stream<ValueType>& Bcast_3_to_6, hls::stream<ValueType>& Bcast_4_to_6, hls::stream<ValueType>& Bcast_5_to_6, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_6_to_7, hls::stream<ValueType>& Bcast_6_to_8, hls::stream<ValueType>& Bcast_6_to_9, hls::stream<ValueType>& Bcast_6_to_10, hls::stream<ValueType>& Bcast_6_to_11, hls::stream<ValueType>& Bcast_6_to_12, hls::stream<ValueType>& Bcast_6_to_13, hls::stream<ValueType>& Bcast_6_to_14, hls::stream<ValueType>& Bcast_6_to_15, hls::stream<ValueType>& Bcast_6_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync6_loop_i__:
	for(int i=0;i<row_break5;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_6.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_6.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_6 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_6.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_6.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_6 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_6.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_6.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_6 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_6.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_6.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_6 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_6.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_6.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_6 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break4)%W][(i-row_break4)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break4)%W][(i-row_break4)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break4)%W][(i-row_break4)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break4)%W][(i-row_break4)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break4,0);
		trsv_sync6_loop_j__:
		while(j<(row_break5-row_break4)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break4){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream6     from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_7 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_7  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_8  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_9  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_10  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_11  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_12  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_13  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_14  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_15  from trsv_sync6 %f\n", i, Xtemp);
			#endif
			Bcast_6_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_6_to_16  from trsv_sync6 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync7(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_7, hls::stream<ValueType>& Bcast_2_to_7, hls::stream<ValueType>& Bcast_3_to_7, hls::stream<ValueType>& Bcast_4_to_7, hls::stream<ValueType>& Bcast_5_to_7, hls::stream<ValueType>& Bcast_6_to_7, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_7_to_8, hls::stream<ValueType>& Bcast_7_to_9, hls::stream<ValueType>& Bcast_7_to_10, hls::stream<ValueType>& Bcast_7_to_11, hls::stream<ValueType>& Bcast_7_to_12, hls::stream<ValueType>& Bcast_7_to_13, hls::stream<ValueType>& Bcast_7_to_14, hls::stream<ValueType>& Bcast_7_to_15, hls::stream<ValueType>& Bcast_7_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync7_loop_i__:
	for(int i=0;i<row_break6;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_7.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_7.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_7 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_7.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_7.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_7 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_7.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_7.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_7 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_7.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_7.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_7 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_7.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_7.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_7 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_7.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_7.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_7 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break5)%W][(i-row_break5)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break5)%W][(i-row_break5)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break5)%W][(i-row_break5)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break5)%W][(i-row_break5)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break5,0);
		trsv_sync7_loop_j__:
		while(j<(row_break6-row_break5)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break5){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream7     from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_8 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_8  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_9  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_10  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_11  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_12  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_13  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_14  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_15  from trsv_sync7 %f\n", i, Xtemp);
			#endif
			Bcast_7_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_7_to_16  from trsv_sync7 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync8(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_8, hls::stream<ValueType>& Bcast_2_to_8, hls::stream<ValueType>& Bcast_3_to_8, hls::stream<ValueType>& Bcast_4_to_8, hls::stream<ValueType>& Bcast_5_to_8, hls::stream<ValueType>& Bcast_6_to_8, hls::stream<ValueType>& Bcast_7_to_8, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_8_to_9, hls::stream<ValueType>& Bcast_8_to_10, hls::stream<ValueType>& Bcast_8_to_11, hls::stream<ValueType>& Bcast_8_to_12, hls::stream<ValueType>& Bcast_8_to_13, hls::stream<ValueType>& Bcast_8_to_14, hls::stream<ValueType>& Bcast_8_to_15, hls::stream<ValueType>& Bcast_8_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync8_loop_i__:
	for(int i=0;i<row_break7;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_8 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_8 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_8 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_8 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_8 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_8 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_8.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_8.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_8 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break6)%W][(i-row_break6)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break6)%W][(i-row_break6)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break6)%W][(i-row_break6)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break6)%W][(i-row_break6)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break6,0);
		trsv_sync8_loop_j__:
		while(j<(row_break7-row_break6)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break6){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream8     from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_9 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_9  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_10  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_11  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_12  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_13  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_14  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_15  from trsv_sync8 %f\n", i, Xtemp);
			#endif
			Bcast_8_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_8_to_16  from trsv_sync8 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync9(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_9, hls::stream<ValueType>& Bcast_2_to_9, hls::stream<ValueType>& Bcast_3_to_9, hls::stream<ValueType>& Bcast_4_to_9, hls::stream<ValueType>& Bcast_5_to_9, hls::stream<ValueType>& Bcast_6_to_9, hls::stream<ValueType>& Bcast_7_to_9, hls::stream<ValueType>& Bcast_8_to_9, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_9_to_10, hls::stream<ValueType>& Bcast_9_to_11, hls::stream<ValueType>& Bcast_9_to_12, hls::stream<ValueType>& Bcast_9_to_13, hls::stream<ValueType>& Bcast_9_to_14, hls::stream<ValueType>& Bcast_9_to_15, hls::stream<ValueType>& Bcast_9_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync9_loop_i__:
	for(int i=0;i<row_break8;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_9 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_9 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_9 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_9 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_9 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_9 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_9 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_9.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_9.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_9 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break7)%W][(i-row_break7)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break7)%W][(i-row_break7)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break7)%W][(i-row_break7)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break7)%W][(i-row_break7)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break7,0);
		trsv_sync9_loop_j__:
		while(j<(row_break8-row_break7)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break7){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream9     from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_10 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_10  from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_11  from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_12  from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_13  from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_14  from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_15  from trsv_sync9 %f\n", i, Xtemp);
			#endif
			Bcast_9_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_9_to_16  from trsv_sync9 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync10(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_10, hls::stream<ValueType>& Bcast_2_to_10, hls::stream<ValueType>& Bcast_3_to_10, hls::stream<ValueType>& Bcast_4_to_10, hls::stream<ValueType>& Bcast_5_to_10, hls::stream<ValueType>& Bcast_6_to_10, hls::stream<ValueType>& Bcast_7_to_10, hls::stream<ValueType>& Bcast_8_to_10, hls::stream<ValueType>& Bcast_9_to_10, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_10_to_11, hls::stream<ValueType>& Bcast_10_to_12, hls::stream<ValueType>& Bcast_10_to_13, hls::stream<ValueType>& Bcast_10_to_14, hls::stream<ValueType>& Bcast_10_to_15, hls::stream<ValueType>& Bcast_10_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync10_loop_i__:
	for(int i=0;i<row_break9;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_10 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_10 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_10 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_10 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_10 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_10 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_10 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_10 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_10.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_10.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_10 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break8)%W][(i-row_break8)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break8)%W][(i-row_break8)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break8)%W][(i-row_break8)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break8)%W][(i-row_break8)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break8,0);
		trsv_sync10_loop_j__:
		while(j<(row_break9-row_break8)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break8){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream10     from trsv_sync10 %f\n", i, Xtemp);
			#endif
			Bcast_10_to_11 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_10_to_11  from trsv_sync10 %f\n", i, Xtemp);
			#endif
			Bcast_10_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_10_to_12  from trsv_sync10 %f\n", i, Xtemp);
			#endif
			Bcast_10_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_10_to_13  from trsv_sync10 %f\n", i, Xtemp);
			#endif
			Bcast_10_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_10_to_14  from trsv_sync10 %f\n", i, Xtemp);
			#endif
			Bcast_10_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_10_to_15  from trsv_sync10 %f\n", i, Xtemp);
			#endif
			Bcast_10_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_10_to_16  from trsv_sync10 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync11(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_11, hls::stream<ValueType>& Bcast_2_to_11, hls::stream<ValueType>& Bcast_3_to_11, hls::stream<ValueType>& Bcast_4_to_11, hls::stream<ValueType>& Bcast_5_to_11, hls::stream<ValueType>& Bcast_6_to_11, hls::stream<ValueType>& Bcast_7_to_11, hls::stream<ValueType>& Bcast_8_to_11, hls::stream<ValueType>& Bcast_9_to_11, hls::stream<ValueType>& Bcast_10_to_11, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_11_to_12, hls::stream<ValueType>& Bcast_11_to_13, hls::stream<ValueType>& Bcast_11_to_14, hls::stream<ValueType>& Bcast_11_to_15, hls::stream<ValueType>& Bcast_11_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync11_loop_i__:
	for(int i=0;i<row_break10;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_11 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_11 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_11 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_11 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_11 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_11 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_11 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_11 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_11 (from trsv_sync9) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break9){
			if(i%2==0)
				Xtemp = Bcast_10_to_11.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_10_to_11.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_10_to_11 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break9)%W][(i-row_break9)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break9)%W][(i-row_break9)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break9)%W][(i-row_break9)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break9)%W][(i-row_break9)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break9,0);
		trsv_sync11_loop_j__:
		while(j<(row_break10-row_break9)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break9){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream11     from trsv_sync11 %f\n", i, Xtemp);
			#endif
			Bcast_11_to_12 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_11_to_12  from trsv_sync11 %f\n", i, Xtemp);
			#endif
			Bcast_11_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_11_to_13  from trsv_sync11 %f\n", i, Xtemp);
			#endif
			Bcast_11_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_11_to_14  from trsv_sync11 %f\n", i, Xtemp);
			#endif
			Bcast_11_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_11_to_15  from trsv_sync11 %f\n", i, Xtemp);
			#endif
			Bcast_11_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_11_to_16  from trsv_sync11 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync12(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_12, hls::stream<ValueType>& Bcast_2_to_12, hls::stream<ValueType>& Bcast_3_to_12, hls::stream<ValueType>& Bcast_4_to_12, hls::stream<ValueType>& Bcast_5_to_12, hls::stream<ValueType>& Bcast_6_to_12, hls::stream<ValueType>& Bcast_7_to_12, hls::stream<ValueType>& Bcast_8_to_12, hls::stream<ValueType>& Bcast_9_to_12, hls::stream<ValueType>& Bcast_10_to_12, hls::stream<ValueType>& Bcast_11_to_12, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_12_to_13, hls::stream<ValueType>& Bcast_12_to_14, hls::stream<ValueType>& Bcast_12_to_15, hls::stream<ValueType>& Bcast_12_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync12_loop_i__:
	for(int i=0;i<row_break11;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_12 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_12 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_12 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_12 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_12 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_12 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_12 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_12 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_12 (from trsv_sync9) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break9){
			if(i%2==0)
				Xtemp = Bcast_10_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_10_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_10_to_12 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break10){
			if(i%2==0)
				Xtemp = Bcast_11_to_12.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_11_to_12.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_11_to_12 (from trsv_sync11) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break10)%W][(i-row_break10)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break10)%W][(i-row_break10)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break10)%W][(i-row_break10)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break10)%W][(i-row_break10)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break10,0);
		trsv_sync12_loop_j__:
		while(j<(row_break11-row_break10)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break10){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream12     from trsv_sync12 %f\n", i, Xtemp);
			#endif
			Bcast_12_to_13 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_12_to_13  from trsv_sync12 %f\n", i, Xtemp);
			#endif
			Bcast_12_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_12_to_14  from trsv_sync12 %f\n", i, Xtemp);
			#endif
			Bcast_12_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_12_to_15  from trsv_sync12 %f\n", i, Xtemp);
			#endif
			Bcast_12_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_12_to_16  from trsv_sync12 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync13(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_13, hls::stream<ValueType>& Bcast_2_to_13, hls::stream<ValueType>& Bcast_3_to_13, hls::stream<ValueType>& Bcast_4_to_13, hls::stream<ValueType>& Bcast_5_to_13, hls::stream<ValueType>& Bcast_6_to_13, hls::stream<ValueType>& Bcast_7_to_13, hls::stream<ValueType>& Bcast_8_to_13, hls::stream<ValueType>& Bcast_9_to_13, hls::stream<ValueType>& Bcast_10_to_13, hls::stream<ValueType>& Bcast_11_to_13, hls::stream<ValueType>& Bcast_12_to_13, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_13_to_14, hls::stream<ValueType>& Bcast_13_to_15, hls::stream<ValueType>& Bcast_13_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync13_loop_i__:
	for(int i=0;i<row_break12;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_13 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_13 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_13 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_13 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_13 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_13 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_13 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_13 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_13 (from trsv_sync9) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break9){
			if(i%2==0)
				Xtemp = Bcast_10_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_10_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_10_to_13 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break10){
			if(i%2==0)
				Xtemp = Bcast_11_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_11_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_11_to_13 (from trsv_sync11) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break11){
			if(i%2==0)
				Xtemp = Bcast_12_to_13.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_12_to_13.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_12_to_13 (from trsv_sync12) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break11)%W][(i-row_break11)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break11)%W][(i-row_break11)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break11)%W][(i-row_break11)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break11)%W][(i-row_break11)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break11,0);
		trsv_sync13_loop_j__:
		while(j<(row_break12-row_break11)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break11){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream13     from trsv_sync13 %f\n", i, Xtemp);
			#endif
			Bcast_13_to_14 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_13_to_14  from trsv_sync13 %f\n", i, Xtemp);
			#endif
			Bcast_13_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_13_to_15  from trsv_sync13 %f\n", i, Xtemp);
			#endif
			Bcast_13_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_13_to_16  from trsv_sync13 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync14(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_14, hls::stream<ValueType>& Bcast_2_to_14, hls::stream<ValueType>& Bcast_3_to_14, hls::stream<ValueType>& Bcast_4_to_14, hls::stream<ValueType>& Bcast_5_to_14, hls::stream<ValueType>& Bcast_6_to_14, hls::stream<ValueType>& Bcast_7_to_14, hls::stream<ValueType>& Bcast_8_to_14, hls::stream<ValueType>& Bcast_9_to_14, hls::stream<ValueType>& Bcast_10_to_14, hls::stream<ValueType>& Bcast_11_to_14, hls::stream<ValueType>& Bcast_12_to_14, hls::stream<ValueType>& Bcast_13_to_14, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_14_to_15, hls::stream<ValueType>& Bcast_14_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync14_loop_i__:
	for(int i=0;i<row_break13;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_14 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_14 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_14 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_14 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_14 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_14 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_14 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_14 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_14 (from trsv_sync9) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break9){
			if(i%2==0)
				Xtemp = Bcast_10_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_10_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_10_to_14 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break10){
			if(i%2==0)
				Xtemp = Bcast_11_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_11_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_11_to_14 (from trsv_sync11) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break11){
			if(i%2==0)
				Xtemp = Bcast_12_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_12_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_12_to_14 (from trsv_sync12) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break12){
			if(i%2==0)
				Xtemp = Bcast_13_to_14.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_13_to_14.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_13_to_14 (from trsv_sync13) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break12)%W][(i-row_break12)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break12)%W][(i-row_break12)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break12)%W][(i-row_break12)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break12)%W][(i-row_break12)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break12,0);
		trsv_sync14_loop_j__:
		while(j<(row_break13-row_break12)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break12){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream14     from trsv_sync14 %f\n", i, Xtemp);
			#endif
			Bcast_14_to_15 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_14_to_15  from trsv_sync14 %f\n", i, Xtemp);
			#endif
			Bcast_14_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_14_to_16  from trsv_sync14 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync15(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_15, hls::stream<ValueType>& Bcast_2_to_15, hls::stream<ValueType>& Bcast_3_to_15, hls::stream<ValueType>& Bcast_4_to_15, hls::stream<ValueType>& Bcast_5_to_15, hls::stream<ValueType>& Bcast_6_to_15, hls::stream<ValueType>& Bcast_7_to_15, hls::stream<ValueType>& Bcast_8_to_15, hls::stream<ValueType>& Bcast_9_to_15, hls::stream<ValueType>& Bcast_10_to_15, hls::stream<ValueType>& Bcast_11_to_15, hls::stream<ValueType>& Bcast_12_to_15, hls::stream<ValueType>& Bcast_13_to_15, hls::stream<ValueType>& Bcast_14_to_15, 
	/* to   where we send    data */ hls::stream<ValueType>& Bcast_15_to_16, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync15_loop_i__:
	for(int i=0;i<row_break14;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_15 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_15 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_15 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_15 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_15 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_15 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_15 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_15 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_15 (from trsv_sync9) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break9){
			if(i%2==0)
				Xtemp = Bcast_10_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_10_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_10_to_15 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break10){
			if(i%2==0)
				Xtemp = Bcast_11_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_11_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_11_to_15 (from trsv_sync11) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break11){
			if(i%2==0)
				Xtemp = Bcast_12_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_12_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_12_to_15 (from trsv_sync12) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break12){
			if(i%2==0)
				Xtemp = Bcast_13_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_13_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_13_to_15 (from trsv_sync13) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break13){
			if(i%2==0)
				Xtemp = Bcast_14_to_15.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_14_to_15.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_14_to_15 (from trsv_sync14) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break13)%W][(i-row_break13)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break13)%W][(i-row_break13)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break13)%W][(i-row_break13)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break13)%W][(i-row_break13)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break13,0);
		trsv_sync15_loop_j__:
		while(j<(row_break14-row_break13)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break13){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream15     from trsv_sync15 %f\n", i, Xtemp);
			#endif
			Bcast_15_to_16 << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to Bcast_15_to_16  from trsv_sync15 %f\n", i, Xtemp);
			#endif
		}
	}
}

void trsv_sync16(hls::stream<ValueType>& BStream, hls::stream<v_dt>& AStream, hls::stream<ValueType>& XStream, 
	/* from where we receive data */ hls::stream<ValueType>& Bcast_1_to_16, hls::stream<ValueType>& Bcast_2_to_16, hls::stream<ValueType>& Bcast_3_to_16, hls::stream<ValueType>& Bcast_4_to_16, hls::stream<ValueType>& Bcast_5_to_16, hls::stream<ValueType>& Bcast_6_to_16, hls::stream<ValueType>& Bcast_7_to_16, hls::stream<ValueType>& Bcast_8_to_16, hls::stream<ValueType>& Bcast_9_to_16, hls::stream<ValueType>& Bcast_10_to_16, hls::stream<ValueType>& Bcast_11_to_16, hls::stream<ValueType>& Bcast_12_to_16, hls::stream<ValueType>& Bcast_13_to_16, hls::stream<ValueType>& Bcast_14_to_16, hls::stream<ValueType>& Bcast_15_to_16, 
	/* to   where we send    data */ 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	ValueType bram_ping[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	ValueType bram_pong[W][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	for(int k=0;k<BRAM_SIZE;k++){
		for(int j=0;j<W;j++){
			#pragma hls unroll
			bram_ping[j][k]=0;
			bram_pong[j][k]=0;
		}
	}

	trsv_sync16_loop_i__:
	for(int i=0;i<K;i++){
		ValueType Xtemp=0;
		if(i<row_break0){
			if(i%2==0)
				Xtemp = Bcast_1_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_1_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_1_to_16 (from trsv_sync1) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break1){
			if(i%2==0)
				Xtemp = Bcast_2_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_2_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_2_to_16 (from trsv_sync2) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break2){
			if(i%2==0)
				Xtemp = Bcast_3_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_3_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_3_to_16 (from trsv_sync3) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break3){
			if(i%2==0)
				Xtemp = Bcast_4_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_4_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_4_to_16 (from trsv_sync4) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break4){
			if(i%2==0)
				Xtemp = Bcast_5_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_5_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_5_to_16 (from trsv_sync5) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break5){
			if(i%2==0)
				Xtemp = Bcast_6_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_6_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_6_to_16 (from trsv_sync6) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break6){
			if(i%2==0)
				Xtemp = Bcast_7_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_7_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_7_to_16 (from trsv_sync7) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break7){
			if(i%2==0)
				Xtemp = Bcast_8_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_8_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_8_to_16 (from trsv_sync8) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break8){
			if(i%2==0)
				Xtemp = Bcast_9_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_9_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_9_to_16 (from trsv_sync9) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break9){
			if(i%2==0)
				Xtemp = Bcast_10_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_10_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_10_to_16 (from trsv_sync10) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break10){
			if(i%2==0)
				Xtemp = Bcast_11_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_11_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_11_to_16 (from trsv_sync11) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break11){
			if(i%2==0)
				Xtemp = Bcast_12_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_12_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_12_to_16 (from trsv_sync12) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break12){
			if(i%2==0)
				Xtemp = Bcast_13_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_13_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_13_to_16 (from trsv_sync13) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break13){
			if(i%2==0)
				Xtemp = Bcast_14_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_14_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_14_to_16 (from trsv_sync14) %f\n", i, Xtemp);
			#endif
		}
		else if(i<row_break14){
			if(i%2==0)
				Xtemp = Bcast_15_to_16.read();// - bram_ping[(i)%W][(i)/W];
			else
				Xtemp = Bcast_15_to_16.read();// - bram_pong[(i)%W][(i)/W];
			#ifndef __SYNTHESIS__
			printf("\n\t(i=%d) reading from Bcast_15_to_16 (from trsv_sync15) %f\n", i, Xtemp);
			#endif
		}
		else{		
			if(i%2==0){
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_ping[(i-row_break14)%W][(i-row_break14)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from ping buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_ping[(i-row_break14)%W][(i-row_break14)/W]);
				#endif
			}
			else{
				ValueType bstream_tmp = BStream.read();
				Xtemp = bstream_tmp - bram_pong[(i-row_break14)%W][(i-row_break14)/W];
				#ifndef __SYNTHESIS__
				printf("\n\t(i=%d) reading from pong buffer %f ( %f - %f )\n", i, Xtemp, bstream_tmp, bram_pong[(i-row_break14)%W][(i-row_break14)/W]);
				#endif
			}
		}

		int j=hls::fmax(i-row_break14,0);
		trsv_sync16_loop_j__:
		while(j<(K-row_break14)){
			#pragma hls pipeline ii=1

			v_dt Atemp = AStream.read();

			#ifndef __SYNTHESIS__
			printf("(i = %d, j = %d) ->  [", i, j);
			#endif
			for(int k=0; k<W;k++){
				#pragma hls unroll
				#ifndef __SYNTHESIS__
				printf("%f ", Atemp.data[k]);
				#endif

				if(i%2==0){
					ValueType temp = bram_ping[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_pong[k][j/W] = temp2;
				}
				else{
					ValueType temp = bram_pong[k][j/W];
					ValueType temp2 = temp+Xtemp*Atemp.data[k];
					bram_ping[k][j/W] = temp2;
				}
			}
			#ifndef __SYNTHESIS__
			printf("]\n");
			#endif
			j+=W;
		}

		if(i>=row_break14){
			XStream << Xtemp;
			#ifndef __SYNTHESIS__
			printf("\t(i=%d) writing to XStream16     from trsv_sync16 %f\n", i, Xtemp);
			#endif
		}
	}
}

void write_x(hls::stream<ValueType>& Xstream1, hls::stream<ValueType>& Xstream2, hls::stream<ValueType>& Xstream3, hls::stream<ValueType>& Xstream4, hls::stream<ValueType>& Xstream5, hls::stream<ValueType>& Xstream6, hls::stream<ValueType>& Xstream7, hls::stream<ValueType>& Xstream8, hls::stream<ValueType>& Xstream9, hls::stream<ValueType>& Xstream10, hls::stream<ValueType>& Xstream11, hls::stream<ValueType>& Xstream12, hls::stream<ValueType>& Xstream13, hls::stream<ValueType>& Xstream14, hls::stream<ValueType>& Xstream15, hls::stream<ValueType>& Xstream16,
	ValueType *X, 
	const int K, int row_break0, int row_break1, int row_break2, int row_break3, int row_break4, int row_break5, int row_break6, int row_break7, int row_break8, int row_break9, int row_break10, int row_break11, int row_break12, int row_break13, int row_break14)
{
	write_y:
	for(int i=0;i<K;i++){
		#pragma HLS pipeline II=1
		if(i<row_break0){
			X[i] = Xstream1.read();
			#ifndef __SYNTHESIS__
			printf("Xstream1\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break1){
			X[i] = Xstream2.read();
			#ifndef __SYNTHESIS__
			printf("Xstream2\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break2){
			X[i] = Xstream3.read();
			#ifndef __SYNTHESIS__
			printf("Xstream3\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break3){
			X[i] = Xstream4.read();
			#ifndef __SYNTHESIS__
			printf("Xstream4\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break4){
			X[i] = Xstream5.read();
			#ifndef __SYNTHESIS__
			printf("Xstream5\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break5){
			X[i] = Xstream6.read();
			#ifndef __SYNTHESIS__
			printf("Xstream6\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break6){
			X[i] = Xstream7.read();
			#ifndef __SYNTHESIS__
			printf("Xstream7\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break7){
			X[i] = Xstream8.read();
			#ifndef __SYNTHESIS__
			printf("Xstream8\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break8){
			X[i] = Xstream9.read();
			#ifndef __SYNTHESIS__
			printf("Xstream9\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break9){
			X[i] = Xstream10.read();
			#ifndef __SYNTHESIS__
			printf("Xstream10\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break10){
			X[i] = Xstream11.read();
			#ifndef __SYNTHESIS__
			printf("Xstream11\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break11){
			X[i] = Xstream12.read();
			#ifndef __SYNTHESIS__
			printf("Xstream12\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break12){
			X[i] = Xstream13.read();
			#ifndef __SYNTHESIS__
			printf("Xstream13\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break13){
			X[i] = Xstream14.read();
			#ifndef __SYNTHESIS__
			printf("Xstream14\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else if(i<row_break14){
			X[i] = Xstream15.read();
			#ifndef __SYNTHESIS__
			printf("Xstream15\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}
		else{
			X[i] = Xstream16.read();
			#ifndef __SYNTHESIS__
			printf("Xstream16\twrote i = %ld, value = %f to X\n", i, X[i]);
			#endif
		}

	}
}

extern "C" {

void krnl_trsv(const int uplo, const int transa, const int diag, 
	const int K, 
	const v_dt *A1, const v_dt *A2, const v_dt *A3, const v_dt *A4, const v_dt *A5, const v_dt *A6, const v_dt *A7, const v_dt *A8, const v_dt *A9, const v_dt *A10, const v_dt *A11, const v_dt *A12, const v_dt *A13, const v_dt *A14, const v_dt *A15, const v_dt *A16, 
	const int lda, 
	ValueType *B, ValueType *X, const int ldb, 
	const int row_break0, const int row_break1, const int row_break2, const int row_break3, const int row_break4, const int row_break5, const int row_break6, const int row_break7, const int row_break8, const int row_break9, const int row_break10, const int row_break11, const int row_break12, const int row_break13, const int row_break14)
{

	#pragma HLS INTERFACE m_axi port = B offset = slave bundle = gmem0
	#pragma HLS INTERFACE m_axi port = X offset = slave bundle = gmem1
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = gmem2
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = gmem3
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = gmem4
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = gmem5
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = gmem6
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = gmem7
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = gmem8
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = gmem9
	#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = gmem10
	#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = gmem11
	#pragma HLS INTERFACE m_axi port = A11 offset = slave bundle = gmem12
	#pragma HLS INTERFACE m_axi port = A12 offset = slave bundle = gmem13
	#pragma HLS INTERFACE m_axi port = A13 offset = slave bundle = gmem14
	#pragma HLS INTERFACE m_axi port = A14 offset = slave bundle = gmem15
	#pragma HLS INTERFACE m_axi port = A15 offset = slave bundle = gmem16
	#pragma HLS INTERFACE m_axi port = A16 offset = slave bundle = gmem17

	#pragma HLS INTERFACE s_axilite port = uplo
	#pragma HLS INTERFACE s_axilite port = transa
	#pragma HLS INTERFACE s_axilite port = diag
	#pragma HLS INTERFACE s_axilite port = K
	#pragma HLS INTERFACE s_axilite port = lda
	#pragma HLS INTERFACE s_axilite port = ldb
	#pragma HLS INTERFACE s_axilite port = row_break0
	#pragma HLS INTERFACE s_axilite port = row_break1
	#pragma HLS INTERFACE s_axilite port = row_break2
	#pragma HLS INTERFACE s_axilite port = row_break3
	#pragma HLS INTERFACE s_axilite port = row_break4
	#pragma HLS INTERFACE s_axilite port = row_break5
	#pragma HLS INTERFACE s_axilite port = row_break6
	#pragma HLS INTERFACE s_axilite port = row_break7
	#pragma HLS INTERFACE s_axilite port = row_break8
	#pragma HLS INTERFACE s_axilite port = row_break9
	#pragma HLS INTERFACE s_axilite port = row_break10
	#pragma HLS INTERFACE s_axilite port = row_break11
	#pragma HLS INTERFACE s_axilite port = row_break12
	#pragma HLS INTERFACE s_axilite port = row_break13
	#pragma HLS INTERFACE s_axilite port = row_break14

	static hls::stream<v_dt> A1_in("A1_in");
	static hls::stream<v_dt> A2_in("A2_in");
	static hls::stream<v_dt> A3_in("A3_in");
	static hls::stream<v_dt> A4_in("A4_in");
	static hls::stream<v_dt> A5_in("A5_in");
	static hls::stream<v_dt> A6_in("A6_in");
	static hls::stream<v_dt> A7_in("A7_in");
	static hls::stream<v_dt> A8_in("A8_in");
	static hls::stream<v_dt> A9_in("A9_in");
	static hls::stream<v_dt> A10_in("A10_in");
	static hls::stream<v_dt> A11_in("A11_in");
	static hls::stream<v_dt> A12_in("A12_in");
	static hls::stream<v_dt> A13_in("A13_in");
	static hls::stream<v_dt> A14_in("A14_in");
	static hls::stream<v_dt> A15_in("A15_in");
	static hls::stream<v_dt> A16_in("A16_in");

	static hls::stream<ValueType> B1_in("B1_in");
	static hls::stream<ValueType> B2_in("B2_in");
	static hls::stream<ValueType> B3_in("B3_in");
	static hls::stream<ValueType> B4_in("B4_in");
	static hls::stream<ValueType> B5_in("B5_in");
	static hls::stream<ValueType> B6_in("B6_in");
	static hls::stream<ValueType> B7_in("B7_in");
	static hls::stream<ValueType> B8_in("B8_in");
	static hls::stream<ValueType> B9_in("B9_in");
	static hls::stream<ValueType> B10_in("B10_in");
	static hls::stream<ValueType> B11_in("B11_in");
	static hls::stream<ValueType> B12_in("B12_in");
	static hls::stream<ValueType> B13_in("B13_in");
	static hls::stream<ValueType> B14_in("B14_in");
	static hls::stream<ValueType> B15_in("B15_in");
	static hls::stream<ValueType> B16_in("B16_in");

	static hls::stream<ValueType> X1_out("X1_out");
	static hls::stream<ValueType> X2_out("X2_out");
	static hls::stream<ValueType> X3_out("X3_out");
	static hls::stream<ValueType> X4_out("X4_out");
	static hls::stream<ValueType> X5_out("X5_out");
	static hls::stream<ValueType> X6_out("X6_out");
	static hls::stream<ValueType> X7_out("X7_out");
	static hls::stream<ValueType> X8_out("X8_out");
	static hls::stream<ValueType> X9_out("X9_out");
	static hls::stream<ValueType> X10_out("X10_out");
	static hls::stream<ValueType> X11_out("X11_out");
	static hls::stream<ValueType> X12_out("X12_out");
	static hls::stream<ValueType> X13_out("X13_out");
	static hls::stream<ValueType> X14_out("X14_out");
	static hls::stream<ValueType> X15_out("X15_out");
	static hls::stream<ValueType> X16_out("X16_out");

	static hls::stream<ValueType> Bcast_1_to_2("Bcast_1_to_2");
	static hls::stream<ValueType> Bcast_1_to_3("Bcast_1_to_3");
	static hls::stream<ValueType> Bcast_1_to_4("Bcast_1_to_4");
	static hls::stream<ValueType> Bcast_1_to_5("Bcast_1_to_5");
	static hls::stream<ValueType> Bcast_1_to_6("Bcast_1_to_6");
	static hls::stream<ValueType> Bcast_1_to_7("Bcast_1_to_7");
	static hls::stream<ValueType> Bcast_1_to_8("Bcast_1_to_8");
	static hls::stream<ValueType> Bcast_1_to_9("Bcast_1_to_9");
	static hls::stream<ValueType> Bcast_1_to_10("Bcast_1_to_10");
	static hls::stream<ValueType> Bcast_1_to_11("Bcast_1_to_11");
	static hls::stream<ValueType> Bcast_1_to_12("Bcast_1_to_12");
	static hls::stream<ValueType> Bcast_1_to_13("Bcast_1_to_13");
	static hls::stream<ValueType> Bcast_1_to_14("Bcast_1_to_14");
	static hls::stream<ValueType> Bcast_1_to_15("Bcast_1_to_15");
	static hls::stream<ValueType> Bcast_1_to_16("Bcast_1_to_16");

	static hls::stream<ValueType> Bcast_2_to_3("Bcast_2_to_3");
	static hls::stream<ValueType> Bcast_2_to_4("Bcast_2_to_4");
	static hls::stream<ValueType> Bcast_2_to_5("Bcast_2_to_5");
	static hls::stream<ValueType> Bcast_2_to_6("Bcast_2_to_6");
	static hls::stream<ValueType> Bcast_2_to_7("Bcast_2_to_7");
	static hls::stream<ValueType> Bcast_2_to_8("Bcast_2_to_8");
	static hls::stream<ValueType> Bcast_2_to_9("Bcast_2_to_9");
	static hls::stream<ValueType> Bcast_2_to_10("Bcast_2_to_10");
	static hls::stream<ValueType> Bcast_2_to_11("Bcast_2_to_11");
	static hls::stream<ValueType> Bcast_2_to_12("Bcast_2_to_12");
	static hls::stream<ValueType> Bcast_2_to_13("Bcast_2_to_13");
	static hls::stream<ValueType> Bcast_2_to_14("Bcast_2_to_14");
	static hls::stream<ValueType> Bcast_2_to_15("Bcast_2_to_15");
	static hls::stream<ValueType> Bcast_2_to_16("Bcast_2_to_16");

	static hls::stream<ValueType> Bcast_3_to_4("Bcast_3_to_4");
	static hls::stream<ValueType> Bcast_3_to_5("Bcast_3_to_5");
	static hls::stream<ValueType> Bcast_3_to_6("Bcast_3_to_6");
	static hls::stream<ValueType> Bcast_3_to_7("Bcast_3_to_7");
	static hls::stream<ValueType> Bcast_3_to_8("Bcast_3_to_8");
	static hls::stream<ValueType> Bcast_3_to_9("Bcast_3_to_9");
	static hls::stream<ValueType> Bcast_3_to_10("Bcast_3_to_10");
	static hls::stream<ValueType> Bcast_3_to_11("Bcast_3_to_11");
	static hls::stream<ValueType> Bcast_3_to_12("Bcast_3_to_12");
	static hls::stream<ValueType> Bcast_3_to_13("Bcast_3_to_13");
	static hls::stream<ValueType> Bcast_3_to_14("Bcast_3_to_14");
	static hls::stream<ValueType> Bcast_3_to_15("Bcast_3_to_15");
	static hls::stream<ValueType> Bcast_3_to_16("Bcast_3_to_16");

	static hls::stream<ValueType> Bcast_4_to_5("Bcast_4_to_5");
	static hls::stream<ValueType> Bcast_4_to_6("Bcast_4_to_6");
	static hls::stream<ValueType> Bcast_4_to_7("Bcast_4_to_7");
	static hls::stream<ValueType> Bcast_4_to_8("Bcast_4_to_8");
	static hls::stream<ValueType> Bcast_4_to_9("Bcast_4_to_9");
	static hls::stream<ValueType> Bcast_4_to_10("Bcast_4_to_10");
	static hls::stream<ValueType> Bcast_4_to_11("Bcast_4_to_11");
	static hls::stream<ValueType> Bcast_4_to_12("Bcast_4_to_12");
	static hls::stream<ValueType> Bcast_4_to_13("Bcast_4_to_13");
	static hls::stream<ValueType> Bcast_4_to_14("Bcast_4_to_14");
	static hls::stream<ValueType> Bcast_4_to_15("Bcast_4_to_15");
	static hls::stream<ValueType> Bcast_4_to_16("Bcast_4_to_16");

	static hls::stream<ValueType> Bcast_5_to_6("Bcast_5_to_6");
	static hls::stream<ValueType> Bcast_5_to_7("Bcast_5_to_7");
	static hls::stream<ValueType> Bcast_5_to_8("Bcast_5_to_8");
	static hls::stream<ValueType> Bcast_5_to_9("Bcast_5_to_9");
	static hls::stream<ValueType> Bcast_5_to_10("Bcast_5_to_10");
	static hls::stream<ValueType> Bcast_5_to_11("Bcast_5_to_11");
	static hls::stream<ValueType> Bcast_5_to_12("Bcast_5_to_12");
	static hls::stream<ValueType> Bcast_5_to_13("Bcast_5_to_13");
	static hls::stream<ValueType> Bcast_5_to_14("Bcast_5_to_14");
	static hls::stream<ValueType> Bcast_5_to_15("Bcast_5_to_15");
	static hls::stream<ValueType> Bcast_5_to_16("Bcast_5_to_16");

	static hls::stream<ValueType> Bcast_6_to_7("Bcast_6_to_7");
	static hls::stream<ValueType> Bcast_6_to_8("Bcast_6_to_8");
	static hls::stream<ValueType> Bcast_6_to_9("Bcast_6_to_9");
	static hls::stream<ValueType> Bcast_6_to_10("Bcast_6_to_10");
	static hls::stream<ValueType> Bcast_6_to_11("Bcast_6_to_11");
	static hls::stream<ValueType> Bcast_6_to_12("Bcast_6_to_12");
	static hls::stream<ValueType> Bcast_6_to_13("Bcast_6_to_13");
	static hls::stream<ValueType> Bcast_6_to_14("Bcast_6_to_14");
	static hls::stream<ValueType> Bcast_6_to_15("Bcast_6_to_15");
	static hls::stream<ValueType> Bcast_6_to_16("Bcast_6_to_16");

	static hls::stream<ValueType> Bcast_7_to_8("Bcast_7_to_8");
	static hls::stream<ValueType> Bcast_7_to_9("Bcast_7_to_9");
	static hls::stream<ValueType> Bcast_7_to_10("Bcast_7_to_10");
	static hls::stream<ValueType> Bcast_7_to_11("Bcast_7_to_11");
	static hls::stream<ValueType> Bcast_7_to_12("Bcast_7_to_12");
	static hls::stream<ValueType> Bcast_7_to_13("Bcast_7_to_13");
	static hls::stream<ValueType> Bcast_7_to_14("Bcast_7_to_14");
	static hls::stream<ValueType> Bcast_7_to_15("Bcast_7_to_15");
	static hls::stream<ValueType> Bcast_7_to_16("Bcast_7_to_16");

	static hls::stream<ValueType> Bcast_8_to_9("Bcast_8_to_9");
	static hls::stream<ValueType> Bcast_8_to_10("Bcast_8_to_10");
	static hls::stream<ValueType> Bcast_8_to_11("Bcast_8_to_11");
	static hls::stream<ValueType> Bcast_8_to_12("Bcast_8_to_12");
	static hls::stream<ValueType> Bcast_8_to_13("Bcast_8_to_13");
	static hls::stream<ValueType> Bcast_8_to_14("Bcast_8_to_14");
	static hls::stream<ValueType> Bcast_8_to_15("Bcast_8_to_15");
	static hls::stream<ValueType> Bcast_8_to_16("Bcast_8_to_16");

	static hls::stream<ValueType> Bcast_9_to_10("Bcast_9_to_10");
	static hls::stream<ValueType> Bcast_9_to_11("Bcast_9_to_11");
	static hls::stream<ValueType> Bcast_9_to_12("Bcast_9_to_12");
	static hls::stream<ValueType> Bcast_9_to_13("Bcast_9_to_13");
	static hls::stream<ValueType> Bcast_9_to_14("Bcast_9_to_14");
	static hls::stream<ValueType> Bcast_9_to_15("Bcast_9_to_15");
	static hls::stream<ValueType> Bcast_9_to_16("Bcast_9_to_16");

	static hls::stream<ValueType> Bcast_10_to_11("Bcast_10_to_11");
	static hls::stream<ValueType> Bcast_10_to_12("Bcast_10_to_12");
	static hls::stream<ValueType> Bcast_10_to_13("Bcast_10_to_13");
	static hls::stream<ValueType> Bcast_10_to_14("Bcast_10_to_14");
	static hls::stream<ValueType> Bcast_10_to_15("Bcast_10_to_15");
	static hls::stream<ValueType> Bcast_10_to_16("Bcast_10_to_16");

	static hls::stream<ValueType> Bcast_11_to_12("Bcast_11_to_12");
	static hls::stream<ValueType> Bcast_11_to_13("Bcast_11_to_13");
	static hls::stream<ValueType> Bcast_11_to_14("Bcast_11_to_14");
	static hls::stream<ValueType> Bcast_11_to_15("Bcast_11_to_15");
	static hls::stream<ValueType> Bcast_11_to_16("Bcast_11_to_16");

	static hls::stream<ValueType> Bcast_12_to_13("Bcast_12_to_13");
	static hls::stream<ValueType> Bcast_12_to_14("Bcast_12_to_14");
	static hls::stream<ValueType> Bcast_12_to_15("Bcast_12_to_15");
	static hls::stream<ValueType> Bcast_12_to_16("Bcast_12_to_16");

	static hls::stream<ValueType> Bcast_13_to_14("Bcast_13_to_14");
	static hls::stream<ValueType> Bcast_13_to_15("Bcast_13_to_15");
	static hls::stream<ValueType> Bcast_13_to_16("Bcast_13_to_16");

	static hls::stream<ValueType> Bcast_14_to_15("Bcast_14_to_15");
	static hls::stream<ValueType> Bcast_14_to_16("Bcast_14_to_16");

	static hls::stream<ValueType> Bcast_15_to_16("Bcast_15_to_16");

	#pragma HLS STREAM variable = A1_in depth = 1024
	#pragma HLS STREAM variable = A2_in depth = 1024
	#pragma HLS STREAM variable = A3_in depth = 1024
	#pragma HLS STREAM variable = A4_in depth = 1024
	#pragma HLS STREAM variable = A5_in depth = 1024
	#pragma HLS STREAM variable = A6_in depth = 1024
	#pragma HLS STREAM variable = A7_in depth = 1024
	#pragma HLS STREAM variable = A8_in depth = 1024
	#pragma HLS STREAM variable = A9_in depth = 1024
	#pragma HLS STREAM variable = A10_in depth = 1024
	#pragma HLS STREAM variable = A11_in depth = 1024
	#pragma HLS STREAM variable = A12_in depth = 1024
	#pragma HLS STREAM variable = A13_in depth = 1024
	#pragma HLS STREAM variable = A14_in depth = 1024
	#pragma HLS STREAM variable = A15_in depth = 1024
	#pragma HLS STREAM variable = A16_in depth = 1024
	#pragma HLS STREAM variable = B1_in depth = 1024
	#pragma HLS STREAM variable = B2_in depth = 1024
	#pragma HLS STREAM variable = B3_in depth = 1024
	#pragma HLS STREAM variable = B4_in depth = 1024
	#pragma HLS STREAM variable = B5_in depth = 1024
	#pragma HLS STREAM variable = B6_in depth = 1024
	#pragma HLS STREAM variable = B7_in depth = 1024
	#pragma HLS STREAM variable = B8_in depth = 1024
	#pragma HLS STREAM variable = B9_in depth = 1024
	#pragma HLS STREAM variable = B10_in depth = 1024
	#pragma HLS STREAM variable = B11_in depth = 1024
	#pragma HLS STREAM variable = B12_in depth = 1024
	#pragma HLS STREAM variable = B13_in depth = 1024
	#pragma HLS STREAM variable = B14_in depth = 1024
	#pragma HLS STREAM variable = B15_in depth = 1024
	#pragma HLS STREAM variable = B16_in depth = 1024
	#pragma HLS STREAM variable = X1_out depth = 1024
	#pragma HLS STREAM variable = X2_out depth = 1024
	#pragma HLS STREAM variable = X3_out depth = 1024
	#pragma HLS STREAM variable = X4_out depth = 1024
	#pragma HLS STREAM variable = X5_out depth = 1024
	#pragma HLS STREAM variable = X6_out depth = 1024
	#pragma HLS STREAM variable = X7_out depth = 1024
	#pragma HLS STREAM variable = X8_out depth = 1024
	#pragma HLS STREAM variable = X9_out depth = 1024
	#pragma HLS STREAM variable = X10_out depth = 1024
	#pragma HLS STREAM variable = X11_out depth = 1024
	#pragma HLS STREAM variable = X12_out depth = 1024
	#pragma HLS STREAM variable = X13_out depth = 1024
	#pragma HLS STREAM variable = X14_out depth = 1024
	#pragma HLS STREAM variable = X15_out depth = 1024
	#pragma HLS STREAM variable = X16_out depth = 1024

	#pragma HLS STREAM variable = Bcast_1_to_2 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_3 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_4 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_5 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_6 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_7 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_1_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_2_to_3 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_4 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_5 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_6 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_7 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_2_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_3_to_4 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_5 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_6 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_7 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_3_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_4_to_5 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_6 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_7 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_4_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_5_to_6 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_7 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_5_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_6_to_7 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_6_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_7_to_8 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_7_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_8_to_9 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_8_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_9_to_10 depth = 1024
	#pragma HLS STREAM variable = Bcast_9_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_9_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_9_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_9_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_9_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_9_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_10_to_11 depth = 1024
	#pragma HLS STREAM variable = Bcast_10_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_10_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_10_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_10_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_10_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_11_to_12 depth = 1024
	#pragma HLS STREAM variable = Bcast_11_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_11_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_11_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_11_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_12_to_13 depth = 1024
	#pragma HLS STREAM variable = Bcast_12_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_12_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_12_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_13_to_14 depth = 1024
	#pragma HLS STREAM variable = Bcast_13_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_13_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_14_to_15 depth = 1024
	#pragma HLS STREAM variable = Bcast_14_to_16 depth = 1024

	#pragma HLS STREAM variable = Bcast_15_to_16 depth = 1024	

	#pragma HLS DATAFLOW
	printf("------------------------------------------------------------------------------------------------------------\nread_b\n");	
	read_b(B1_in, B2_in, B3_in, B4_in, B5_in, B6_in, B7_in, B8_in, B9_in, B10_in, B11_in, B12_in, B13_in, B14_in, B15_in, B16_in, B, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix1\n");
	read_matrix1(A1, A1_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix2\n");
	read_matrix2(A2, A2_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix3\n");
	read_matrix3(A3, A3_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix4\n");
	read_matrix4(A4, A4_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix5\n");
	read_matrix5(A5, A5_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix6\n");
	read_matrix6(A6, A6_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix7\n");
	read_matrix7(A7, A7_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix8\n");
	read_matrix8(A8, A8_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix9\n");
	read_matrix9(A9, A9_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix10\n");
	read_matrix10(A10, A10_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix11\n");
	read_matrix11(A11, A11_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix12\n");
	read_matrix12(A12, A12_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix13\n");
	read_matrix13(A13, A13_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix14\n");
	read_matrix14(A14, A14_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix15\n");
	read_matrix15(A15, A15_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\nread_matrix16\n");
	read_matrix16(A16, A16_in, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync1\n");
	trsv_sync1(B1_in, A1_in, X1_out, Bcast_1_to_2, Bcast_1_to_3, Bcast_1_to_4, Bcast_1_to_5, Bcast_1_to_6, Bcast_1_to_7, Bcast_1_to_8, Bcast_1_to_9, Bcast_1_to_10, Bcast_1_to_11, Bcast_1_to_12, Bcast_1_to_13, Bcast_1_to_14, Bcast_1_to_15, Bcast_1_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync2\n");
	trsv_sync2(B2_in, A2_in, X2_out, Bcast_1_to_2, Bcast_2_to_3, Bcast_2_to_4, Bcast_2_to_5, Bcast_2_to_6, Bcast_2_to_7, Bcast_2_to_8, Bcast_2_to_9, Bcast_2_to_10, Bcast_2_to_11, Bcast_2_to_12, Bcast_2_to_13, Bcast_2_to_14, Bcast_2_to_15, Bcast_2_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync3\n");
	trsv_sync3(B3_in, A3_in, X3_out, Bcast_1_to_3, Bcast_2_to_3, Bcast_3_to_4, Bcast_3_to_5, Bcast_3_to_6, Bcast_3_to_7, Bcast_3_to_8, Bcast_3_to_9, Bcast_3_to_10, Bcast_3_to_11, Bcast_3_to_12, Bcast_3_to_13, Bcast_3_to_14, Bcast_3_to_15, Bcast_3_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync4\n");
	trsv_sync4(B4_in, A4_in, X4_out, Bcast_1_to_4, Bcast_2_to_4, Bcast_3_to_4, Bcast_4_to_5, Bcast_4_to_6, Bcast_4_to_7, Bcast_4_to_8, Bcast_4_to_9, Bcast_4_to_10, Bcast_4_to_11, Bcast_4_to_12, Bcast_4_to_13, Bcast_4_to_14, Bcast_4_to_15, Bcast_4_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync5\n");
	trsv_sync5(B5_in, A5_in, X5_out, Bcast_1_to_5, Bcast_2_to_5, Bcast_3_to_5, Bcast_4_to_5, Bcast_5_to_6, Bcast_5_to_7, Bcast_5_to_8, Bcast_5_to_9, Bcast_5_to_10, Bcast_5_to_11, Bcast_5_to_12, Bcast_5_to_13, Bcast_5_to_14, Bcast_5_to_15, Bcast_5_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync6\n");
	trsv_sync6(B6_in, A6_in, X6_out, Bcast_1_to_6, Bcast_2_to_6, Bcast_3_to_6, Bcast_4_to_6, Bcast_5_to_6, Bcast_6_to_7, Bcast_6_to_8, Bcast_6_to_9, Bcast_6_to_10, Bcast_6_to_11, Bcast_6_to_12, Bcast_6_to_13, Bcast_6_to_14, Bcast_6_to_15, Bcast_6_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync7\n");
	trsv_sync7(B7_in, A7_in, X7_out, Bcast_1_to_7, Bcast_2_to_7, Bcast_3_to_7, Bcast_4_to_7, Bcast_5_to_7, Bcast_6_to_7, Bcast_7_to_8, Bcast_7_to_9, Bcast_7_to_10, Bcast_7_to_11, Bcast_7_to_12, Bcast_7_to_13, Bcast_7_to_14, Bcast_7_to_15, Bcast_7_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync8\n");
	trsv_sync8(B8_in, A8_in, X8_out, Bcast_1_to_8, Bcast_2_to_8, Bcast_3_to_8, Bcast_4_to_8, Bcast_5_to_8, Bcast_6_to_8, Bcast_7_to_8, Bcast_8_to_9, Bcast_8_to_10, Bcast_8_to_11, Bcast_8_to_12, Bcast_8_to_13, Bcast_8_to_14, Bcast_8_to_15, Bcast_8_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync9\n");
	trsv_sync9(B9_in, A9_in, X9_out, Bcast_1_to_9, Bcast_2_to_9, Bcast_3_to_9, Bcast_4_to_9, Bcast_5_to_9, Bcast_6_to_9, Bcast_7_to_9, Bcast_8_to_9, Bcast_9_to_10, Bcast_9_to_11, Bcast_9_to_12, Bcast_9_to_13, Bcast_9_to_14, Bcast_9_to_15, Bcast_9_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync10\n");
	trsv_sync10(B10_in, A10_in, X10_out, Bcast_1_to_10, Bcast_2_to_10, Bcast_3_to_10, Bcast_4_to_10, Bcast_5_to_10, Bcast_6_to_10, Bcast_7_to_10, Bcast_8_to_10, Bcast_9_to_10, Bcast_10_to_11, Bcast_10_to_12, Bcast_10_to_13, Bcast_10_to_14, Bcast_10_to_15, Bcast_10_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync11\n");
	trsv_sync11(B11_in, A11_in, X11_out, Bcast_1_to_11, Bcast_2_to_11, Bcast_3_to_11, Bcast_4_to_11, Bcast_5_to_11, Bcast_6_to_11, Bcast_7_to_11, Bcast_8_to_11, Bcast_9_to_11, Bcast_10_to_11, Bcast_11_to_12, Bcast_11_to_13, Bcast_11_to_14, Bcast_11_to_15, Bcast_11_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync12\n");
	trsv_sync12(B12_in, A12_in, X12_out, Bcast_1_to_12, Bcast_2_to_12, Bcast_3_to_12, Bcast_4_to_12, Bcast_5_to_12, Bcast_6_to_12, Bcast_7_to_12, Bcast_8_to_12, Bcast_9_to_12, Bcast_10_to_12, Bcast_11_to_12, Bcast_12_to_13, Bcast_12_to_14, Bcast_12_to_15, Bcast_12_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync13\n");
	trsv_sync13(B13_in, A13_in, X13_out, Bcast_1_to_13, Bcast_2_to_13, Bcast_3_to_13, Bcast_4_to_13, Bcast_5_to_13, Bcast_6_to_13, Bcast_7_to_13, Bcast_8_to_13, Bcast_9_to_13, Bcast_10_to_13, Bcast_11_to_13, Bcast_12_to_13, Bcast_13_to_14, Bcast_13_to_15, Bcast_13_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync14\n");
	trsv_sync14(B14_in, A14_in, X14_out, Bcast_1_to_14, Bcast_2_to_14, Bcast_3_to_14, Bcast_4_to_14, Bcast_5_to_14, Bcast_6_to_14, Bcast_7_to_14, Bcast_8_to_14, Bcast_9_to_14, Bcast_10_to_14, Bcast_11_to_14, Bcast_12_to_14, Bcast_13_to_14, Bcast_14_to_15, Bcast_14_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync15\n");
	trsv_sync15(B15_in, A15_in, X15_out, Bcast_1_to_15, Bcast_2_to_15, Bcast_3_to_15, Bcast_4_to_15, Bcast_5_to_15, Bcast_6_to_15, Bcast_7_to_15, Bcast_8_to_15, Bcast_9_to_15, Bcast_10_to_15, Bcast_11_to_15, Bcast_12_to_15, Bcast_13_to_15, Bcast_14_to_15, Bcast_15_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\ntrsv_sync16\n");
	trsv_sync16(B16_in, A16_in, X16_out, Bcast_1_to_16, Bcast_2_to_16, Bcast_3_to_16, Bcast_4_to_16, Bcast_5_to_16, Bcast_6_to_16, Bcast_7_to_16, Bcast_8_to_16, Bcast_9_to_16, Bcast_10_to_16, Bcast_11_to_16, Bcast_12_to_16, Bcast_13_to_16, Bcast_14_to_16, Bcast_15_to_16, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);

	printf("------------------------------------------------------------------------------------------------------------\nwrite_x\n");
	write_x(X1_out, X2_out, X3_out, X4_out, X5_out, X6_out, X7_out, X8_out, X9_out, X10_out, X11_out, X12_out, X13_out, X14_out, X15_out, X16_out, X, K, row_break0, row_break1, row_break2, row_break3, row_break4, row_break5, row_break6, row_break7, row_break8, row_break9, row_break10, row_break11, row_break12, row_break13, row_break14);
	printf("------------------------------------------------------------------------------------------------------------\n\n");


}

}
