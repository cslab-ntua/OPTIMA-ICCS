#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define BRAM_SIZE 704
#define VDATA_SIZE 16


typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;

void read_b(const float *B, hls::stream<float>& Bstream1, hls::stream<float>& Bstream2, hls::stream<float>& Bstream3, hls::stream<float>& Bstream4,
							hls::stream<float>& Bstream5, hls::stream<float>& Bstream6, hls::stream<float>& Bstream7, hls::stream<float>& Bstream8,
							hls::stream<float>& Bstream9, hls::stream<float>& Bstream10, hls::stream<float>& Bstream11, hls::stream<float>& Bstream12,
							hls::stream<float>& Bstream13, hls::stream<float>& Bstream14, hls::stream<float>& Bstream15, hls::stream<float>& Bstream16,
							const int N, int break_row1, int break_row2, int break_row3, int break_row4, int break_row5, int break_row6, int break_row7,
							int break_row8, int break_row9, int break_row10, int break_row11, int break_row12, int break_row13, int break_row14, int break_row15)
{
	read_b:
	for(int i=0;i<N;i++){
		#pragma HLS pipelinee II=1
		if(i<break_row1)
		{
			Bstream1 << B[i];
		}
		else if(i<break_row2)
		{
			Bstream2 << B[i];
		}
		else if(i<break_row3)
		{
			Bstream3 << B[i];
		}
		else if (i<break_row4)
		{
			Bstream4 << B[i];
		}
		else if(i<break_row5)
		{
			Bstream5 << B[i];
		}
		else if(i<break_row6)
		{
			Bstream6 << B[i];
		}
		else if(i<break_row7)
		{
			Bstream7 << B[i];
		}
		else if(i<break_row8)
		{
			Bstream8 << B[i];
		}
		else if(i<break_row9)
		{
			Bstream9 << B[i];
		}
		else if(i<break_row10)
		{
			Bstream10 << B[i];
		}
		else if(i<break_row11)
		{
			Bstream11<< B[i];
		}
		else if(i<break_row12)
		{
			Bstream12<< B[i];
		}
		else if(i<break_row13)
		{
			Bstream13<< B[i];
		}
		else if(i<break_row14)
		{
			Bstream14<< B[i];
		}
		else if(i<break_row15)
		{
			Bstream15<< B[i];
		}
		else
		{
			Bstream16<< B[i];
		}

	}
}

void write_x(hls::stream<float>& Xstream1, hls::stream<float>& Xstream2, hls::stream<float>& Xstream3, hls::stream<float>& Xstream4,
			hls::stream<float>& Xstream5, hls::stream<float>& Xstream6, hls::stream<float>& Xstream7, hls::stream<float>& Xstream8,
			hls::stream<float>& Xstream9, hls::stream<float>& Xstream10, hls::stream<float>& Xstream11, hls::stream<float>& Xstream12,
			hls::stream<float>& Xstream13, hls::stream<float>& Xstream14, hls::stream<float>& Xstream15, hls::stream<float>& Xstream16,
			float *X, const int N, const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
			const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10, const int break_row11,
			const int break_row12, const int break_row13, const int break_row14, const int break_row15)
{
	write_x:
	for(int i=0;i<N;i++)
	{
		#pragma HLS pipelinee II=1

		if(i<break_row1)
		{
			X[i] = Xstream1.read();
		}
		else if(i<break_row2)
		{
			X[i] = Xstream2.read();
		}
		else if(i<break_row3)
		{
			X[i] = Xstream3.read();
		}
		else if(i<break_row4)
		{
			X[i] = Xstream4.read();
		}
		else if(i<break_row5)
		{
			X[i] = Xstream5.read();
		}
		else if(i<break_row6)
		{
			X[i] = Xstream6.read();
		}
		else if(i<break_row7)
		{
			X[i] = Xstream7.read();
		}
		else if(i<break_row8)
		{
			X[i] = Xstream8.read();
		}
		else if(i<break_row9)
		{
			X[i] = Xstream9.read();
		}
		else if(i<break_row10)
		{
			X[i] = Xstream10.read();
		}
		else if(i<break_row11)
		{
			X[i] = Xstream11.read();
		}
		else if(i<break_row12)
		{
			X[i] = Xstream12.read();
		}
		else if(i<break_row13)
		{
			X[i] = Xstream13.read();
		}
		else if(i<break_row14)
		{
			X[i] = Xstream14.read();
		}
		else if(i<break_row15)
		{
			X[i] = Xstream15.read();
		}
		else
		{
			X[i] = Xstream16.read();
		}

	}
}






void read_matrix_new_1(const v_dt *A, hls::stream<v_dt>& Astream, const int N,const int break_row1)
{

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - 15*cu_blk16_lim_fac;

	int blk16_idx = 0;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row1)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;

		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // reads only some blk16 block depending on the N and the number of CUs in the kernel
		{
			blk16_idx++;
		}
		else
		{
			row++;
			blk16_idx = row_blk16_idx_limit; // change the idx of blk16 to point to the start blk16 idx of the next row
			curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
			row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
			cu_blk16_idx_limit = row_blk16_idx_limit- 15*cu_blk16_lim_fac; // adjust the limit depending
		}


	}
}

void read_matrix_new_2(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row1, const int break_row2){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - 14*cu_blk16_lim_fac;


	int blk16_idx = cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row2)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row1)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 15*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit- 14*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit- 14*cu_blk16_lim_fac;
			}

		}

	}

}
void read_matrix_new_3(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row2, const int break_row3){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - 13*cu_blk16_lim_fac;

	int blk16_idx = 2*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns


	loop:
	while(row < break_row3)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row2)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 14*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit- 13*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit- 13*cu_blk16_lim_fac;
			}

		}

	}
}

void read_matrix_new_4(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row3, const int break_row4){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 12*cu_blk16_lim_fac ;

	int blk16_idx = 3*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row4)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row3)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 13*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 12*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit- 12*cu_blk16_lim_fac;

			}
		}

	}
}

void read_matrix_new_5(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row4, const int break_row5){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - 11*cu_blk16_lim_fac;

	int blk16_idx = 4*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row5)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row4)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 12*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit- 11*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit- 11*cu_blk16_lim_fac;
			}

		}

	}
}

void read_matrix_new_6(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row5, const int break_row6){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - 10*cu_blk16_lim_fac;


	int blk16_idx = 5*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns


	loop:
	while(row < break_row6)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row5)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 11*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit- 10*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit- 10*cu_blk16_lim_fac;
			}
		}

	}

}

void read_matrix_new_7(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row6, const int break_row7){


	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - 9*cu_blk16_lim_fac;


	int blk16_idx = 6*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row7)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row6)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 10*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit- 9*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit- 9*cu_blk16_lim_fac;
			}
		}

	}
}

void read_matrix_new_8(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row7, const int break_row8){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 8*cu_blk16_lim_fac ;


	int blk16_idx = 7*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row8)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;

		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{

			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row7)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 9*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 8*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 8*cu_blk16_lim_fac;
			}
		}

	}

}

void read_matrix_new_9(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row8, const int break_row9){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 7*cu_blk16_lim_fac ;


	int blk16_idx = 8*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row9)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row8)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 8*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 7*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 7*cu_blk16_lim_fac;
			}


		}

	}
}

void read_matrix_new_10(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row9, const int break_row10){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 6*cu_blk16_lim_fac ;


	int blk16_idx = 9*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns


	loop:
	while(row < break_row10)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row9)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 7*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 6*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 6*cu_blk16_lim_fac;
			}
		}

	}
}


void read_matrix_new_11(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row10, const int break_row11){

	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 5*cu_blk16_lim_fac ;


	int blk16_idx = 10*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns


	loop:
	while(row < break_row11)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{

			row++;
			if(row<break_row10)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 6*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 5*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 5*cu_blk16_lim_fac;
			}

		}

	}
}


void read_matrix_new_12(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row11, const int break_row12){
	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 4*cu_blk16_lim_fac ;

	int blk16_idx = 11*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row12)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row11)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 5*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 4*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 4*cu_blk16_lim_fac;

			}
		}

	}
}

void read_matrix_new_13(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row12, const int break_row13){
	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 3*cu_blk16_lim_fac ;


	int blk16_idx = 12*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row13)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row12)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 4*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 3*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 3*cu_blk16_lim_fac;
			}
		}

	}

}

void read_matrix_new_14(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row13, const int break_row14){
	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit- 2*cu_blk16_lim_fac ;

	int blk16_idx = 13*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row14)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row13)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 3*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - 2*cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - 2*cu_blk16_lim_fac;
			}
		}

	}
}

void read_matrix_new_15(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row14, const int break_row15){
	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit - cu_blk16_lim_fac ;

	int blk16_idx = 14*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < break_row15)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;


		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row14)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - 2*cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit - cu_blk16_lim_fac;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit - cu_blk16_lim_fac;
			}
		}

	}
}


void read_matrix_new_16(const v_dt *A, hls::stream<v_dt>& Astream, const int N, const int break_row15){
	wide_read:
	int row = 0;
	int cu_blk16_lim_fac = (N / VDATA_SIZE) / 16 ;
	int row_blk16_idx_limit = ( ( N - 1 ) >> 4 ) + 1 ;
	int cu_blk16_idx_limit = row_blk16_idx_limit ;

	int blk16_idx = 15*cu_blk16_lim_fac;
	int curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns

	loop:
	while(row < N)
	{
		v_dt Atemp = A[blk16_idx];
		Astream << Atemp;

		cond:
		if(blk16_idx < cu_blk16_idx_limit-1 ) // read from a specific blkidx of row until in the end
		{
			blk16_idx++;

		}
		else
		{
			row++;
			if(row<break_row15)
			{

				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				blk16_idx = row_blk16_idx_limit - cu_blk16_lim_fac; // adjust the limit depending on the NCUs and row
				cu_blk16_idx_limit = row_blk16_idx_limit ;

			}
			else
			{
				blk16_idx = row_blk16_idx_limit;
				curRowBlks16Num = (( N - row - 1 ) >>4 ) + 1 ;
				row_blk16_idx_limit +=  curRowBlks16Num; // Set the limit of blk16 for the cur row
				cu_blk16_idx_limit = row_blk16_idx_limit ;
			}
		}

	}

}


void tpsv_sync_p_1(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */
		/* to where we send 	data */ hls::stream<float>& Bcast_1_to_2, hls::stream<float>& Bcast_1_to_3, hls::stream<float>& Bcast_1_to_4,
		 	 	 	 	 	 	 	 	hls::stream<float>& Bcast_1_to_5, hls::stream<float>& Bcast_1_to_6, hls::stream<float>& Bcast_1_to_7, hls::stream<float>& Bcast_1_to_8,
										hls::stream<float>& Bcast_1_to_9, hls::stream<float>& Bcast_1_to_10, hls::stream<float>& Bcast_1_to_11, hls::stream<float>& Bcast_1_to_12,
										hls::stream<float>& Bcast_1_to_13, hls::stream<float>& Bcast_1_to_14, hls::stream<float>& Bcast_1_to_15, hls::stream<float>& Bcast_1_to_16,
										const int break_row1)
		{

			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			for( row=0; row<break_row1; row++)
			{
				float X_temp=0;

				if(row%2==0)
					X_temp = Bsteam.read() - bram_ping[(row)%VDATA_SIZE][(row)/VDATA_SIZE];
				else
					X_temp = Bsteam.read() - bram_pong[(row)%VDATA_SIZE][(row)/VDATA_SIZE];

				int col=row;
				tpsv2:
				while(col < break_row1 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats


					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				Xstream << X_temp;
				Bcast_1_to_2 << X_temp;
				Bcast_1_to_3 << X_temp;
				Bcast_1_to_4 << X_temp;
				Bcast_1_to_5 << X_temp;
				Bcast_1_to_6 << X_temp;
				Bcast_1_to_7 << X_temp;
				Bcast_1_to_8 << X_temp;
				Bcast_1_to_9 << X_temp;
				Bcast_1_to_10 << X_temp;
				Bcast_1_to_11 << X_temp;
				Bcast_1_to_12 << X_temp;
				Bcast_1_to_13 << X_temp;
				Bcast_1_to_14 << X_temp;
				Bcast_1_to_15 << X_temp;
				Bcast_1_to_16 << X_temp;


			}

		}

void tpsv_sync_p_2(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */ hls::stream<float>& Bcast_1_to_2,
		/* to where we send 	data */  hls::stream<float>& Bcast_2_to_3, hls::stream<float>& Bcast_2_to_4,hls::stream<float>& Bcast_2_to_5, hls::stream<float>& Bcast_2_to_6, hls::stream<float>& Bcast_2_to_7, hls::stream<float>& Bcast_2_to_8,
										 hls::stream<float>& Bcast_2_to_9, hls::stream<float>& Bcast_2_to_10, hls::stream<float>& Bcast_2_to_11, hls::stream<float>& Bcast_2_to_12,
										 hls::stream<float>& Bcast_2_to_13, hls::stream<float>& Bcast_2_to_14, hls::stream<float>& Bcast_2_to_15, hls::stream<float>& Bcast_2_to_16,
										 const int break_row1, const int break_row2	)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row2; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_2.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row1)%VDATA_SIZE][(row-break_row1)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row1)%VDATA_SIZE][(row-break_row1)/VDATA_SIZE];
				}



				int col=hls::fmax(row-break_row1,0);

				tpsv_sync_loop_col_:
				while(col < break_row2-break_row1 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row1){
					Xstream << X_temp;
					Bcast_2_to_3 << X_temp;
					Bcast_2_to_4 << X_temp;
					Bcast_2_to_5 << X_temp;
					Bcast_2_to_6 << X_temp;
					Bcast_2_to_7 << X_temp;
					Bcast_2_to_8 << X_temp;
					Bcast_2_to_9 << X_temp;
					Bcast_2_to_10 << X_temp;
					Bcast_2_to_11 << X_temp;
					Bcast_2_to_12 << X_temp;
					Bcast_2_to_13 << X_temp;
					Bcast_2_to_14 << X_temp;
					Bcast_2_to_15 << X_temp;
					Bcast_2_to_16 << X_temp;
				}


			}

		}


void tpsv_sync_p_3(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */ hls::stream<float>& Bcast_1_to_3, hls::stream<float>& Bcast_2_to_3,
		/* to where we send 	data */  hls::stream<float>& Bcast_3_to_4, hls::stream<float>& Bcast_3_to_5, hls::stream<float>& Bcast_3_to_6, hls::stream<float>& Bcast_3_to_7, hls::stream<float>& Bcast_3_to_8,
										 hls::stream<float>& Bcast_3_to_9, hls::stream<float>& Bcast_3_to_10, hls::stream<float>& Bcast_3_to_11, hls::stream<float>& Bcast_3_to_12,
										 hls::stream<float>& Bcast_3_to_13, hls::stream<float>& Bcast_3_to_14, hls::stream<float>& Bcast_3_to_15, hls::stream<float>& Bcast_3_to_16,
										const int break_row1, const int break_row2, const int break_row3	)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row3; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_3.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_3.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row2)%VDATA_SIZE][(row-break_row2)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row2)%VDATA_SIZE][(row-break_row2)/VDATA_SIZE];
				}



				int col=hls::fmax(row-break_row2,0);

				tpsv_sync_loop_col_:
				while(col < break_row3-break_row2 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row2){
					Xstream << X_temp;
					Bcast_3_to_4 << X_temp;
					Bcast_3_to_5 << X_temp;
					Bcast_3_to_6 << X_temp;
					Bcast_3_to_7 << X_temp;
					Bcast_3_to_8 << X_temp;
					Bcast_3_to_9 << X_temp;
					Bcast_3_to_10 << X_temp;
					Bcast_3_to_11 << X_temp;
					Bcast_3_to_12 << X_temp;
					Bcast_3_to_13 << X_temp;
					Bcast_3_to_14 << X_temp;
					Bcast_3_to_15 << X_temp;
					Bcast_3_to_16 << X_temp;

				}


			}

		}

void tpsv_sync_p_4(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_4, hls::stream<float>& Bcast_2_to_4, hls::stream<float>& Bcast_3_to_4,
 		/* to where we send 	data */ hls::stream<float>& Bcast_4_to_5, hls::stream<float>& Bcast_4_to_6, hls::stream<float>& Bcast_4_to_7, hls::stream<float>& Bcast_4_to_8,
										hls::stream<float>& Bcast_4_to_9, hls::stream<float>& Bcast_4_to_10, hls::stream<float>& Bcast_4_to_11, hls::stream<float>& Bcast_4_to_12,
										hls::stream<float>& Bcast_4_to_13, hls::stream<float>& Bcast_4_to_14, hls::stream<float>& Bcast_4_to_15, hls::stream<float>& Bcast_4_to_16,
		const int break_row1, const int break_row2, const int break_row3, const int break_row4	)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row4; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{

					X_temp = Bcast_1_to_4.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_4.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_4.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row3)%VDATA_SIZE][(row-break_row3)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row3)%VDATA_SIZE][(row-break_row3)/VDATA_SIZE];
				}

				int col=hls::fmax(row-break_row3,0);

				tpsv_sync_loop_col_:
				while(col < break_row4-break_row3 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row3){
					Xstream << X_temp;
					Bcast_4_to_5 << X_temp;
					Bcast_4_to_6 << X_temp;
					Bcast_4_to_7 << X_temp;
					Bcast_4_to_8 << X_temp;
					Bcast_4_to_9 << X_temp;
					Bcast_4_to_10 << X_temp;
					Bcast_4_to_11 << X_temp;
					Bcast_4_to_12 << X_temp;
					Bcast_4_to_13 << X_temp;
					Bcast_4_to_14 << X_temp;
					Bcast_4_to_15 << X_temp;
					Bcast_4_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_5(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_5, hls::stream<float>& Bcast_2_to_5, hls::stream<float>& Bcast_3_to_5, hls::stream<float>& Bcast_4_to_5,
 		/* to where we send 	data */ hls::stream<float>& Bcast_5_to_6, hls::stream<float>& Bcast_5_to_7, hls::stream<float>& Bcast_5_to_8,
										hls::stream<float>& Bcast_5_to_9, hls::stream<float>& Bcast_5_to_10, hls::stream<float>& Bcast_5_to_11, hls::stream<float>& Bcast_5_to_12,
										hls::stream<float>& Bcast_5_to_13, hls::stream<float>& Bcast_5_to_14, hls::stream<float>& Bcast_5_to_15, hls::stream<float>& Bcast_5_to_16,
		const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5	)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row5; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_5.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_5.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_5.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_5.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row4)%VDATA_SIZE][(row-break_row4)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row4)%VDATA_SIZE][(row-break_row4)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row4,0);

				tpsv_sync_loop_col_:
				while(col < break_row5-break_row4 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row4){
					Xstream << X_temp;
					Bcast_5_to_6 << X_temp;
					Bcast_5_to_7 << X_temp;
					Bcast_5_to_8 << X_temp;
					Bcast_5_to_9 << X_temp;
					Bcast_5_to_10 << X_temp;
					Bcast_5_to_11 << X_temp;
					Bcast_5_to_12 << X_temp;
					Bcast_5_to_13 << X_temp;
					Bcast_5_to_14 << X_temp;
					Bcast_5_to_15 << X_temp;
					Bcast_5_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_6(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_6, hls::stream<float>& Bcast_2_to_6, hls::stream<float>& Bcast_3_to_6, hls::stream<float>& Bcast_4_to_6, hls::stream<float>& Bcast_5_to_6,
 		/* to where we send 	data */ hls::stream<float>& Bcast_6_to_7, hls::stream<float>& Bcast_6_to_8,
										hls::stream<float>& Bcast_6_to_9, hls::stream<float>& Bcast_6_to_10, hls::stream<float>& Bcast_6_to_11, hls::stream<float>& Bcast_6_to_12,
										hls::stream<float>& Bcast_6_to_13, hls::stream<float>& Bcast_6_to_14, hls::stream<float>& Bcast_6_to_15, hls::stream<float>& Bcast_6_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5, const int break_row6)
		{

			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row6; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_6.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_6.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_6.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_6.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_6.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row5)%VDATA_SIZE][(row-break_row5)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row5)%VDATA_SIZE][(row-break_row5)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row5,0);

				tpsv_sync_loop_col_:
				while(col < break_row6-break_row5 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row5){
					Xstream << X_temp;
					Bcast_6_to_7 << X_temp;
					Bcast_6_to_8 << X_temp;
					Bcast_6_to_9 << X_temp;
					Bcast_6_to_10 << X_temp;
					Bcast_6_to_11 << X_temp;
					Bcast_6_to_12 << X_temp;
					Bcast_6_to_13 << X_temp;
					Bcast_6_to_14 << X_temp;
					Bcast_6_to_15 << X_temp;
					Bcast_6_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_7(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_7, hls::stream<float>& Bcast_2_to_7, hls::stream<float>& Bcast_3_to_7, hls::stream<float>& Bcast_4_to_7, hls::stream<float>& Bcast_5_to_7, hls::stream<float>& Bcast_6_to_7,
 		/* to where we send 	data */ hls::stream<float>& Bcast_7_to_8,
										hls::stream<float>& Bcast_7_to_9, hls::stream<float>& Bcast_7_to_10, hls::stream<float>& Bcast_7_to_11, hls::stream<float>& Bcast_7_to_12,
										hls::stream<float>& Bcast_7_to_13, hls::stream<float>& Bcast_7_to_14, hls::stream<float>& Bcast_7_to_15, hls::stream<float>& Bcast_7_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5, const int break_row6, const int break_row7)
		{

			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row7; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_7.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_7.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_7.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_7.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_7.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_7.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row6)%VDATA_SIZE][(row-break_row6)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row6)%VDATA_SIZE][(row-break_row6)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row6,0);

				tpsv_sync_loop_col_:
				while(col < break_row7-break_row6 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row6){
					Xstream << X_temp;
					Bcast_7_to_8 << X_temp;
					Bcast_7_to_9 << X_temp;
					Bcast_7_to_10 << X_temp;
					Bcast_7_to_11 << X_temp;
					Bcast_7_to_12 << X_temp;
					Bcast_7_to_13 << X_temp;
					Bcast_7_to_14 << X_temp;
					Bcast_7_to_15 << X_temp;
					Bcast_7_to_16 << X_temp;


				}


			}

		}

void tpsv_sync_p_8(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_8, hls::stream<float>& Bcast_2_to_8, hls::stream<float>& Bcast_3_to_8, hls::stream<float>& Bcast_4_to_8, hls::stream<float>& Bcast_5_to_8, hls::stream<float>& Bcast_6_to_8, hls::stream<float>& Bcast_7_to_8,
 		/* to where we send 	data */ hls::stream<float>& Bcast_8_to_9, hls::stream<float>& Bcast_8_to_10, hls::stream<float>& Bcast_8_to_11, hls::stream<float>& Bcast_8_to_12,
										hls::stream<float>& Bcast_8_to_13, hls::stream<float>& Bcast_8_to_14, hls::stream<float>& Bcast_8_to_15, hls::stream<float>& Bcast_8_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5, const int break_row6, const int break_row7, const int break_row8)
		{

			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row8; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_8.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_8.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_8.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_8.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_8.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_8.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_8.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row7)%VDATA_SIZE][(row-break_row7)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row7)%VDATA_SIZE][(row-break_row7)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row7,0);

				tpsv_sync_loop_col_:
				while(col < break_row8-break_row7 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row7){
					Xstream << X_temp;
					Bcast_8_to_9 << X_temp;
					Bcast_8_to_10 << X_temp;
					Bcast_8_to_11 << X_temp;
					Bcast_8_to_12 << X_temp;
					Bcast_8_to_13 << X_temp;
					Bcast_8_to_14 << X_temp;
					Bcast_8_to_15 << X_temp;
					Bcast_8_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_9(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_9, hls::stream<float>& Bcast_2_to_9, hls::stream<float>& Bcast_3_to_9, hls::stream<float>& Bcast_4_to_9,
										hls::stream<float>& Bcast_5_to_9, hls::stream<float>& Bcast_6_to_9, hls::stream<float>& Bcast_7_to_9, hls::stream<float>& Bcast_8_to_9
 		/* to where we send 	data */ , hls::stream<float>& Bcast_9_to_10, hls::stream<float>& Bcast_9_to_11, hls::stream<float>& Bcast_9_to_12,
										hls::stream<float>& Bcast_9_to_13, hls::stream<float>& Bcast_9_to_14, hls::stream<float>& Bcast_9_to_15, hls::stream<float>& Bcast_9_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row9; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_9.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_9.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_9.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_9.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_9.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_9.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_9.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_9.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row8)%VDATA_SIZE][(row-break_row8)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row8)%VDATA_SIZE][(row-break_row8)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row8,0);

				tpsv_sync_loop_col_:
				while(col < break_row9-break_row8 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row8){
					Xstream << X_temp;
					Bcast_9_to_10 << X_temp;
					Bcast_9_to_11 << X_temp;
					Bcast_9_to_12 << X_temp;
					Bcast_9_to_13 << X_temp;
					Bcast_9_to_14 << X_temp;
					Bcast_9_to_15 << X_temp;
					Bcast_9_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_10(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_10, hls::stream<float>& Bcast_2_to_10, hls::stream<float>& Bcast_3_to_10, hls::stream<float>& Bcast_4_to_10,
										hls::stream<float>& Bcast_5_to_10, hls::stream<float>& Bcast_6_to_10, hls::stream<float>& Bcast_7_to_10, hls::stream<float>& Bcast_8_to_10, hls::stream<float>& Bcast_9_to_10,
 		/* to where we send 	data */ hls::stream<float>& Bcast_10_to_11, hls::stream<float>& Bcast_10_to_12,
										hls::stream<float>& Bcast_10_to_13, hls::stream<float>& Bcast_10_to_14, hls::stream<float>& Bcast_10_to_15, hls::stream<float>& Bcast_10_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row10; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_10.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_10.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_10.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_10.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_10.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_10.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_10.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_10.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_10.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row9)%VDATA_SIZE][(row-break_row9)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row9)%VDATA_SIZE][(row-break_row9)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row9,0);

				tpsv_sync_loop_col_:
				while(col < break_row10-break_row9 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row9){
					Xstream << X_temp;
					Bcast_10_to_11 << X_temp;
					Bcast_10_to_12 << X_temp;
					Bcast_10_to_13 << X_temp;
					Bcast_10_to_14 << X_temp;
					Bcast_10_to_15 << X_temp;
					Bcast_10_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_11(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,

		/* from where we receive data */hls::stream<float>& Bcast_1_to_11, hls::stream<float>& Bcast_2_to_11, hls::stream<float>& Bcast_3_to_11, hls::stream<float>& Bcast_4_to_11,
										hls::stream<float>& Bcast_5_to_11, hls::stream<float>& Bcast_6_to_11, hls::stream<float>& Bcast_7_to_11, hls::stream<float>& Bcast_8_to_11,
										hls::stream<float>& Bcast_9_to_11,hls::stream<float>& Bcast_10_to_11,
 		/* to where we send 	data */ hls::stream<float>& Bcast_11_to_12, hls::stream<float>& Bcast_11_to_13, hls::stream<float>& Bcast_11_to_14, hls::stream<float>& Bcast_11_to_15, hls::stream<float>& Bcast_11_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10, const int break_row11)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row11; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_11.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_11.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_11.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_11.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_11.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_11.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_11.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_11.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_11.read();
				}
				else if(row<break_row10)
				{
					X_temp = Bcast_10_to_11.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row10)%VDATA_SIZE][(row-break_row10)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row10)%VDATA_SIZE][(row-break_row10)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row10,0);

				tpsv_sync_loop_col_:
				while(col < break_row11-break_row10 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats
					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row10){
					Xstream << X_temp;
					Bcast_11_to_12 << X_temp;
					Bcast_11_to_13 << X_temp;
					Bcast_11_to_14 << X_temp;
					Bcast_11_to_15 << X_temp;
					Bcast_11_to_16 << X_temp;
				}


			}

		}


void tpsv_sync_p_12(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_12, hls::stream<float>& Bcast_2_to_12, hls::stream<float>& Bcast_3_to_12, hls::stream<float>& Bcast_4_to_12,
										hls::stream<float>& Bcast_5_to_12, hls::stream<float>& Bcast_6_to_12, hls::stream<float>& Bcast_7_to_12, hls::stream<float>& Bcast_8_to_12,
										hls::stream<float>& Bcast_9_to_12,hls::stream<float>& Bcast_10_to_12,
 		/* to where we send 	data */ hls::stream<float>& Bcast_11_to_12, hls::stream<float>& Bcast_12_to_13, hls::stream<float>& Bcast_12_to_14, hls::stream<float>& Bcast_12_to_15, hls::stream<float>& Bcast_12_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10, const int break_row11, const int break_row12)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row12; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_12.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_12.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_12.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_12.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_12.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_12.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_12.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_12.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_12.read();
				}
				else if(row<break_row10)
				{
					X_temp = Bcast_10_to_12.read();
				}
				else if(row<break_row11)
				{
					X_temp = Bcast_11_to_12.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row11)%VDATA_SIZE][(row-break_row11)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row11)%VDATA_SIZE][(row-break_row11)/VDATA_SIZE];
				}




				int col=hls::fmax(row-break_row11,0);

				tpsv_sync_loop_col_:
				while(col < break_row12-break_row11 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats
					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row11){
					Xstream << X_temp;
					Bcast_12_to_13 << X_temp;
					Bcast_12_to_14 << X_temp;
					Bcast_12_to_15 << X_temp;
					Bcast_12_to_16 << X_temp;
				}


			}

		}


void tpsv_sync_p_13(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_13, hls::stream<float>& Bcast_2_to_13, hls::stream<float>& Bcast_3_to_13, hls::stream<float>& Bcast_4_to_13,
										hls::stream<float>& Bcast_5_to_13, hls::stream<float>& Bcast_6_to_13, hls::stream<float>& Bcast_7_to_13, hls::stream<float>& Bcast_8_to_13,
										hls::stream<float>& Bcast_9_to_13,hls::stream<float>& Bcast_10_to_13, hls::stream<float>& Bcast_11_to_13,
 		/* to where we send 	data */ hls::stream<float>& Bcast_12_to_13, hls::stream<float>& Bcast_13_to_14, hls::stream<float>& Bcast_13_to_15, hls::stream<float>& Bcast_13_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10, const int break_row11, const int break_row12, const int break_row13)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row13; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_13.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_13.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_13.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_13.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_13.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_13.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_13.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_13.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_13.read();
				}
				else if(row<break_row10)
				{
					X_temp = Bcast_10_to_13.read();
				}
				else if(row<break_row11)
				{
					X_temp = Bcast_11_to_13.read();
				}
				else if(row<break_row12)
				{
					X_temp = Bcast_12_to_13.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row12)%VDATA_SIZE][(row-break_row12)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row12)%VDATA_SIZE][(row-break_row12)/VDATA_SIZE];
				}


				int col=hls::fmax(row-break_row12,0);

				tpsv_sync_loop_col_:
				while(col < break_row13-break_row12 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row12){
					Xstream << X_temp;
					Bcast_13_to_14 << X_temp;
					Bcast_13_to_15 << X_temp;
					Bcast_13_to_16 << X_temp;
				}


			}

		}


void tpsv_sync_p_14(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_14, hls::stream<float>& Bcast_2_to_14, hls::stream<float>& Bcast_3_to_14, hls::stream<float>& Bcast_4_to_14,
										hls::stream<float>& Bcast_5_to_14, hls::stream<float>& Bcast_6_to_14, hls::stream<float>& Bcast_7_to_14, hls::stream<float>& Bcast_8_to_14,
										hls::stream<float>& Bcast_9_to_14,hls::stream<float>& Bcast_10_to_14, hls::stream<float>& Bcast_11_to_14, hls::stream<float>& Bcast_12_to_14,
 		/* to where we send 	data */ hls::stream<float>& Bcast_13_to_14, hls::stream<float>& Bcast_14_to_15, hls::stream<float>& Bcast_14_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10, const int break_row11, const int break_row12, const int break_row13, const int break_row14)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row14; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_14.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_14.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_14.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_14.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_14.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_14.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_14.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_14.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_14.read();
				}
				else if(row<break_row10)
				{
					X_temp = Bcast_10_to_14.read();
				}
				else if(row<break_row11)
				{
					X_temp = Bcast_11_to_14.read();
				}
				else if(row<break_row12)
				{
					X_temp = Bcast_12_to_14.read();
				}
				else if(row<break_row13)
				{
					X_temp = Bcast_13_to_14.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row13)%VDATA_SIZE][(row-break_row13)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row13)%VDATA_SIZE][(row-break_row13)/VDATA_SIZE];
				}



				int col=hls::fmax(row-break_row13,0);

				tpsv_sync_loop_col_:
				while(col < break_row14-break_row13 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row13){
					Xstream << X_temp;
					Bcast_14_to_15 << X_temp;
					Bcast_14_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_15(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_15, hls::stream<float>& Bcast_2_to_15, hls::stream<float>& Bcast_3_to_15, hls::stream<float>& Bcast_4_to_15,
										hls::stream<float>& Bcast_5_to_15, hls::stream<float>& Bcast_6_to_15, hls::stream<float>& Bcast_7_to_15, hls::stream<float>& Bcast_8_to_15,
										hls::stream<float>& Bcast_9_to_15,hls::stream<float>& Bcast_10_to_15, hls::stream<float>& Bcast_11_to_15, hls::stream<float>& Bcast_12_to_15,
										hls::stream<float>& Bcast_13_to_15, hls::stream<float>& Bcast_14_to_15,
 		/* to where we send 	data */ hls::stream<float>& Bcast_15_to_16,
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10,
										const int break_row11, const int break_row12, const int break_row13, const int break_row14, const int break_row15)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;


			tpsv_sync_loop_row_:
			for( row=0; row<break_row15; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_15.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_15.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_15.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_15.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_15.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_15.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_15.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_15.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_15.read();
				}
				else if(row<break_row10)
				{
					X_temp = Bcast_10_to_15.read();
				}
				else if(row<break_row11)
				{
					X_temp = Bcast_11_to_15.read();
				}
				else if(row<break_row12)
				{
					X_temp = Bcast_12_to_15.read();
				}
				else if(row<break_row13)
				{
					X_temp = Bcast_13_to_15.read();
				}
				else if(row<break_row14)
				{
					X_temp = Bcast_14_to_15.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row14)%VDATA_SIZE][(row-break_row14)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row14)%VDATA_SIZE][(row-break_row14)/VDATA_SIZE];
				}


				int col=hls::fmax(row-break_row14,0);

				tpsv_sync_loop_col_:
				while(col < break_row15-break_row14 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row14){
					Xstream << X_temp;
					Bcast_15_to_16 << X_temp;
				}


			}

		}

void tpsv_sync_p_16(hls::stream<v_dt>& Astream, hls::stream<float>& Bsteam, hls::stream<float>& Xstream, const  int N,
		/* from where we receive data */hls::stream<float>& Bcast_1_to_16, hls::stream<float>& Bcast_2_to_16, hls::stream<float>& Bcast_3_to_16, hls::stream<float>& Bcast_4_to_16,
										hls::stream<float>& Bcast_5_to_16, hls::stream<float>& Bcast_6_to_16, hls::stream<float>& Bcast_7_to_16, hls::stream<float>& Bcast_8_to_16,
										hls::stream<float>& Bcast_9_to_16,hls::stream<float>& Bcast_10_to_16, hls::stream<float>& Bcast_11_to_16, hls::stream<float>& Bcast_12_to_16,
										hls::stream<float>& Bcast_13_to_16, hls::stream<float>& Bcast_14_to_16, hls::stream<float>& Bcast_15_to_16,
 		/* to where we send 	data */
										const int break_row1, const int break_row2, const int break_row3, const int break_row4, const int break_row5,
										const int break_row6, const int break_row7, const int break_row8, const int break_row9, const int break_row10,
										const int break_row11, const int break_row12, const int break_row13, const int break_row14, const int break_row15)
		{
			float bram_ping[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_ping complete dim=1

			float bram_pong[VDATA_SIZE][BRAM_SIZE];
			#pragma HLS array_partition variable=bram_pong complete dim=1

			for (int i = 0; i < VDATA_SIZE; i++) {
				#pragma HLS unroll
				for (int j = 0; j < BRAM_SIZE; j++) {
				#pragma hls pipeline II=1
					bram_ping[i][j] = 0.0f;
					bram_pong[i][j] = 0.0f;
				}
			}
			int row = 0;

			tpsv_sync_loop_row_:
			for( row=0; row<N; row++)
			{
				float X_temp=0;

				if(row<break_row1)
				{
					X_temp = Bcast_1_to_16.read();
				}
				else if(row<break_row2)
				{
					X_temp = Bcast_2_to_16.read();
				}
				else if(row<break_row3)
				{
					X_temp = Bcast_3_to_16.read();
				}
				else if(row<break_row4)
				{
					X_temp = Bcast_4_to_16.read();
				}
				else if(row<break_row5)
				{
					X_temp = Bcast_5_to_16.read();
				}
				else if(row<break_row6)
				{
					X_temp = Bcast_6_to_16.read();
				}
				else if(row<break_row7)
				{
					X_temp = Bcast_7_to_16.read();
				}
				else if(row<break_row8)
				{
					X_temp = Bcast_8_to_16.read();
				}
				else if(row<break_row9)
				{
					X_temp = Bcast_9_to_16.read();
				}
				else if(row<break_row10)
				{
					X_temp = Bcast_10_to_16.read();
				}
				else if(row<break_row11)
				{
					X_temp = Bcast_11_to_16.read();
				}
				else if(row<break_row12)
				{
					X_temp = Bcast_12_to_16.read();
				}
				else if(row<break_row13)
				{
					X_temp = Bcast_13_to_16.read();
				}
				else if(row<break_row14)
				{
					X_temp = Bcast_14_to_16.read();
				}
				else if(row<break_row15)
				{
					X_temp = Bcast_15_to_16.read();
				}
				else
				{
					if(row%2==0)
						X_temp = Bsteam.read() - bram_ping[(row-break_row15)%VDATA_SIZE][(row-break_row15)/VDATA_SIZE];
					else
						X_temp = Bsteam.read() - bram_pong[(row-break_row15)%VDATA_SIZE][(row-break_row15)/VDATA_SIZE];
				}


				int col=hls::fmax(row-break_row15,0);

				tpsv_sync_loop_col_:
				while(col < N-break_row15 )
				{
					#pragma hls pipeline II=1
					v_dt Atemp = Astream.read(); // reads a block of 16 floats

					for(int k=0;k<VDATA_SIZE;k++)
					{
						#pragma HLS unroll
						if(row%2==0)
						{
							float temp = bram_ping[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_pong[k][col/VDATA_SIZE] = temp2;
						}
						else
						{
							float temp = bram_pong[k][col/VDATA_SIZE];
							float temp2 = temp + X_temp*Atemp.data[k];
							bram_ping[k][col/VDATA_SIZE] = temp2;
						}
					}
					col+=VDATA_SIZE;
				}
				if(row>=break_row15){
					Xstream << X_temp;
				}


			}

		}



extern "C" {

	void krnl_tpsv(const int N,
					const v_dt  *A1, const int break_row1,
					const v_dt  *A2, const int break_row2,
					const v_dt  *A3, const int break_row3,
					const v_dt  *A4, const int break_row4,
					const v_dt  *A5, const int break_row5,
					const v_dt  *A6, const int break_row6,
					const v_dt  *A7, const int break_row7,
					const v_dt  *A8, const int break_row8,
					const v_dt  *A9, const int break_row9,
					const v_dt  *A10, const int break_row10,
					const v_dt  *A11, const int break_row11,
					const v_dt  *A12, const int break_row12,
					const v_dt  *A13, const int break_row13,
					const v_dt  *A14, const int break_row14,
					const v_dt  *A15, const int break_row15,
					const v_dt  *A16,
					float *B, float *X)
	{

		#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = hbm0
		#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = hbm1
		#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = hbm2
		#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = hbm3
		#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = hbm4
		#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = hbm5
		#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = hbm6
		#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = hbm7
		#pragma HLS INTERFACE m_axi port = A9 offset = slave bundle = hbm8
		#pragma HLS INTERFACE m_axi port = A10 offset = slave bundle = hbm9
		#pragma HLS INTERFACE m_axi port = A11 offset = slave bundle = hbm10
		#pragma HLS INTERFACE m_axi port = A12 offset = slave bundle = hbm11
		#pragma HLS INTERFACE m_axi port = A13 offset = slave bundle = hbm12
		#pragma HLS INTERFACE m_axi port = A14 offset = slave bundle = hbm13
		#pragma HLS INTERFACE m_axi port = A15 offset = slave bundle = hbm15
		#pragma HLS INTERFACE m_axi port = A16 offset = slave bundle = hbm16
		#pragma HLS INTERFACE m_axi port = B offset = slave bundle = hbm17
		#pragma HLS INTERFACE m_axi port = X offset = slave bundle = hbm18

		#pragma HLS INTERFACE s_axilite port = N
		#pragma HLS INTERFACE s_axilite port = break_row1
		#pragma HLS INTERFACE s_axilite port = break_row2
		#pragma HLS INTERFACE s_axilite port = break_row3
		#pragma HLS INTERFACE s_axilite port = break_row4
		#pragma HLS INTERFACE s_axilite port = break_row5
		#pragma HLS INTERFACE s_axilite port = break_row6
		#pragma HLS INTERFACE s_axilite port = break_row7
		#pragma HLS INTERFACE s_axilite port = break_row8
		#pragma HLS INTERFACE s_axilite port = break_row9
		#pragma HLS INTERFACE s_axilite port = break_row10
		#pragma HLS INTERFACE s_axilite port = break_row11
		#pragma HLS INTERFACE s_axilite port = break_row12
		#pragma HLS INTERFACE s_axilite port = break_row13
		#pragma HLS INTERFACE s_axilite port = break_row14
		#pragma HLS INTERFACE s_axilite port = break_row15




			#pragma HLS dataflow
			/// CU1 streams
			static hls::stream<v_dt> Astream_1("Astream_1");
			static hls::stream<float> Bstream_1("Bstream_1");
			static hls::stream<float> Xstream_1("Xstream_1");
			static hls::stream<float> Bcast_1_to_2("Bcast_1_to_2");
			static hls::stream<float> Bcast_1_to_3("Bcast_1_to_3");
			static hls::stream<float> Bcast_1_to_4("Bcast_1_to_4");
			static hls::stream<float> Bcast_1_to_5("Bcast_1_to_5");
			static hls::stream<float> Bcast_1_to_6("Bcast_1_to_6");
			static hls::stream<float> Bcast_1_to_7("Bcast_1_to_7");
			static hls::stream<float> Bcast_1_to_8("Bcast_1_to_8");
			static hls::stream<float> Bcast_1_to_9("Bcast_1_to_9");
			static hls::stream<float> Bcast_1_to_10("Bcast_1_to_10");
			static hls::stream<float> Bcast_1_to_11("Bcast_1_to_11");
			static hls::stream<float> Bcast_1_to_12("Bcast_1_to_12");
			static hls::stream<float> Bcast_1_to_13("Bcast_1_to_13");
			static hls::stream<float> Bcast_1_to_14("Bcast_1_to_14");
			static hls::stream<float> Bcast_1_to_15("Bcast_1_to_15");
			static hls::stream<float> Bcast_1_to_16("Bcast_1_to_16");
			#pragma HLS stream variable = Astream_1 depth = 2048
			#pragma HLS stream variable = Bstream_1 depth = 2048
			#pragma HLS stream variable = Xstream_1 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_2 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_3 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_4 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_5 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_6 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_7 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_1_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_1_to_16 depth = 2048

			/// CU2 streams
			static hls::stream<v_dt> Astream_2("Astream_2");
			static hls::stream<float> Bstream_2("Bstream_2");
			static hls::stream<float> Xstream_2("Xstream_2");
			#pragma HLS stream variable = Astream_2 depth = 2048
			#pragma HLS stream variable = Bstream_2 depth = 2048
			#pragma HLS stream variable = Xstream_2 depth = 2048
			static hls::stream<float> Bcast_2_to_3("Bcast_2_to_3");
			static hls::stream<float> Bcast_2_to_4("Bcast_2_to_4");
			static hls::stream<float> Bcast_2_to_5("Bcast_2_to_5");
			static hls::stream<float> Bcast_2_to_6("Bcast_2_to_6");
			static hls::stream<float> Bcast_2_to_7("Bcast_2_to_7");
			static hls::stream<float> Bcast_2_to_8("Bcast_2_to_8");
			static hls::stream<float> Bcast_2_to_9("Bcast_2_to_9");
			static hls::stream<float> Bcast_2_to_10("Bcast_2_to_10");
			static hls::stream<float> Bcast_2_to_11("Bcast_2_to_11");
			static hls::stream<float> Bcast_2_to_12("Bcast_2_to_12");
			static hls::stream<float> Bcast_2_to_13("Bcast_2_to_13");
			static hls::stream<float> Bcast_2_to_14("Bcast_2_to_14");
			static hls::stream<float> Bcast_2_to_15("Bcast_2_to_15");
			static hls::stream<float> Bcast_2_to_16("Bcast_2_to_16");
			#pragma HLS stream variable = Bcast_2_to_3 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_4 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_5 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_6 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_7 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_2_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_2_to_16 depth = 2048


			/// CU3 streams
			static hls::stream<v_dt> Astream_3("Astream_3");
			static hls::stream<float> Bstream_3("Bstream_3");
			static hls::stream<float> Xstream_3("Xstream_3");
			#pragma HLS stream variable = Astream_3 depth = 2048
			#pragma HLS stream variable = Bstream_3 depth = 2048
			#pragma HLS stream variable = Xstream_3 depth = 2048
			static hls::stream<float> Bcast_3_to_4("Bcast_3_to_4");
			static hls::stream<float> Bcast_3_to_5("Bcast_3_to_5");
			static hls::stream<float> Bcast_3_to_6("Bcast_3_to_6");
			static hls::stream<float> Bcast_3_to_7("Bcast_3_to_7");
			static hls::stream<float> Bcast_3_to_8("Bcast_3_to_8");
			static hls::stream<float> Bcast_3_to_9("Bcast_3_to_9");
			static hls::stream<float> Bcast_3_to_10("Bcast_3_to_10");
			static hls::stream<float> Bcast_3_to_11("Bcast_3_to_11");
			static hls::stream<float> Bcast_3_to_12("Bcast_3_to_12");
			static hls::stream<float> Bcast_3_to_13("Bcast_3_to_13");
			static hls::stream<float> Bcast_3_to_14("Bcast_3_to_14");
			static hls::stream<float> Bcast_3_to_15("Bcast_3_to_15");
			static hls::stream<float> Bcast_3_to_16("Bcast_3_to_16");
			#pragma HLS stream variable = Bcast_3_to_4 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_5 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_6 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_7 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_3_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_3_to_16 depth = 2048


			/// CU4 streams
			static hls::stream<v_dt> Astream_4("Astream_4");
			static hls::stream<float> Bstream_4("Bstream_4");
			static hls::stream<float> Xstream_4("Xstream_4");
			#pragma HLS stream variable = Astream_4 depth = 2048
			#pragma HLS stream variable = Bstream_4 depth = 2048
			#pragma HLS stream variable = Xstream_4 depth = 2048
			static hls::stream<float> Bcast_4_to_5("Bcast_4_to_5");
			static hls::stream<float> Bcast_4_to_6("Bcast_4_to_6");
			static hls::stream<float> Bcast_4_to_7("Bcast_4_to_7");
			static hls::stream<float> Bcast_4_to_8("Bcast_4_to_8");
			static hls::stream<float> Bcast_4_to_9("Bcast_4_to_9");
			static hls::stream<float> Bcast_4_to_10("Bcast_4_to_10");
			static hls::stream<float> Bcast_4_to_11("Bcast_4_to_11");
			static hls::stream<float> Bcast_4_to_12("Bcast_4_to_12");
			static hls::stream<float> Bcast_4_to_13("Bcast_4_to_13");
			static hls::stream<float> Bcast_4_to_14("Bcast_4_to_14");
			static hls::stream<float> Bcast_4_to_15("Bcast_4_to_15");
			static hls::stream<float> Bcast_4_to_16("Bcast_4_to_16");
			#pragma HLS stream variable = Bcast_4_to_5 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_6 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_7 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_4_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_4_to_16 depth = 2048


			//CU5 streams
			static hls::stream<v_dt> Astream_5("Astream_5");
			static hls::stream<float> Bstream_5("Bstream_5");
			static hls::stream<float> Xstream_5("Xstream_5");
			#pragma HLS stream variable = Astream_5 depth = 2048
			#pragma HLS stream variable = Bstream_5 depth = 2048
			#pragma HLS stream variable = Xstream_5 depth = 2048
			static hls::stream<float> Bcast_5_to_6("Bcast_5_to_6");
			static hls::stream<float> Bcast_5_to_7("Bcast_5_to_7");
			static hls::stream<float> Bcast_5_to_8("Bcast_5_to_8");
			static hls::stream<float> Bcast_5_to_9("Bcast_5_to_9");
			static hls::stream<float> Bcast_5_to_10("Bcast_5_to_10");
			static hls::stream<float> Bcast_5_to_11("Bcast_5_to_11");
			static hls::stream<float> Bcast_5_to_12("Bcast_5_to_12");
			static hls::stream<float> Bcast_5_to_13("Bcast_5_to_13");
			static hls::stream<float> Bcast_5_to_14("Bcast_5_to_14");
			static hls::stream<float> Bcast_5_to_15("Bcast_5_to_15");
			static hls::stream<float> Bcast_5_to_16("Bcast_5_to_16");
			#pragma HLS stream variable = Bcast_5_to_6 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_7 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_5_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_5_to_16 depth = 2048


			//CU6 streams
			static hls::stream<v_dt> Astream_6("Astream_6");
			static hls::stream<float> Bstream_6("Bstream_6");
			static hls::stream<float> Xstream_6("Xstream_6");
			#pragma HLS stream variable = Astream_6 depth = 2048
			#pragma HLS stream variable = Bstream_6 depth = 2048
			#pragma HLS stream variable = Xstream_6 depth = 2048
			static hls::stream<float> Bcast_6_to_7("Bcast_6_to_7");
			static hls::stream<float> Bcast_6_to_8("Bcast_6_to_8");
			static hls::stream<float> Bcast_6_to_9("Bcast_6_to_9");
			static hls::stream<float> Bcast_6_to_10("Bcast_6_to_10");
			static hls::stream<float> Bcast_6_to_11("Bcast_6_to_11");
			static hls::stream<float> Bcast_6_to_12("Bcast_6_to_12");
			static hls::stream<float> Bcast_6_to_13("Bcast_6_to_13");
			static hls::stream<float> Bcast_6_to_14("Bcast_6_to_14");
			static hls::stream<float> Bcast_6_to_15("Bcast_6_to_15");
			static hls::stream<float> Bcast_6_to_16("Bcast_6_to_16");
			#pragma HLS stream variable = Bcast_6_to_7 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_6_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_6_to_16 depth = 2048


			//CU7
			static hls::stream<v_dt> Astream_7("Astream_7");
			static hls::stream<float> Bstream_7("Bstream_7");
			static hls::stream<float> Xstream_7("Xstream_7");
			#pragma HLS stream variable = Astream_7 depth = 2048
			#pragma HLS stream variable = Bstream_7 depth = 2048
			#pragma HLS stream variable = Xstream_7 depth = 2048
			static hls::stream<float> Bcast_7_to_8("Bcast_7_to_8");
			static hls::stream<float> Bcast_7_to_9("Bcast_7_to_9");
			static hls::stream<float> Bcast_7_to_10("Bcast_7_to_10");
			static hls::stream<float> Bcast_7_to_11("Bcast_7_to_11");
			static hls::stream<float> Bcast_7_to_12("Bcast_7_to_12");
			static hls::stream<float> Bcast_7_to_13("Bcast_7_to_13");
			static hls::stream<float> Bcast_7_to_14("Bcast_7_to_14");
			static hls::stream<float> Bcast_7_to_15("Bcast_7_to_15");
			static hls::stream<float> Bcast_7_to_16("Bcast_7_to_16");
			#pragma HLS stream variable = Bcast_7_to_8 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_7_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_7_to_16 depth = 2048


			//CU8
			static hls::stream<v_dt> Astream_8("Astream_8");
			static hls::stream<float> Bstream_8("Bstream_8");
			static hls::stream<float> Xstream_8("Xstream_8");
			#pragma HLS stream variable = Astream_8 depth = 2048
			#pragma HLS stream variable = Bstream_8 depth = 2048
			#pragma HLS stream variable = Xstream_8 depth = 2048
			static hls::stream<float> Bcast_8_to_9("Bcast_8_to_9");
			static hls::stream<float> Bcast_8_to_10("Bcast_8_to_10");
			static hls::stream<float> Bcast_8_to_11("Bcast_8_to_11");
			static hls::stream<float> Bcast_8_to_12("Bcast_8_to_12");
			static hls::stream<float> Bcast_8_to_13("Bcast_8_to_13");
			static hls::stream<float> Bcast_8_to_14("Bcast_8_to_14");
			static hls::stream<float> Bcast_8_to_15("Bcast_8_to_15");
			static hls::stream<float> Bcast_8_to_16("Bcast_8_to_16");
			#pragma HLS stream variable = Bcast_8_to_9  depth = 2048
			#pragma HLS stream variable = Bcast_8_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_8_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_8_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_8_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_8_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_8_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_8_to_16 depth = 2048


			//CU_9
			static hls::stream<v_dt> Astream_9("Astream_9");
			static hls::stream<float> Bstream_9("Bstream_9");
			static hls::stream<float> Xstream_9("Xstream_9");
			#pragma HLS stream variable = Astream_9 depth = 2048
			#pragma HLS stream variable = Bstream_9 depth = 2048
			#pragma HLS stream variable = Xstream_9 depth = 2048
			static hls::stream<float> Bcast_9_to_10("Bcast_9_to_10");
			static hls::stream<float> Bcast_9_to_11("Bcast_9_to_11");
			static hls::stream<float> Bcast_9_to_12("Bcast_9_to_12");
			static hls::stream<float> Bcast_9_to_13("Bcast_9_to_13");
			static hls::stream<float> Bcast_9_to_14("Bcast_9_to_14");
			static hls::stream<float> Bcast_9_to_15("Bcast_9_to_15");
			static hls::stream<float> Bcast_9_to_16("Bcast_9_to_16");
			#pragma HLS stream variable = Bcast_9_to_10 depth = 2048
			#pragma HLS stream variable = Bcast_9_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_9_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_9_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_9_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_9_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_9_to_16 depth = 2048

			//CU_10
			static hls::stream<v_dt> Astream_10("Astream_10");
			static hls::stream<float> Bstream_10("Bstream_10");
			static hls::stream<float> Xstream_10("Xstream_10");
			#pragma HLS stream variable = Astream_10 depth = 2048
			#pragma HLS stream variable = Bstream_10 depth = 2048
			#pragma HLS stream variable = Xstream_10 depth = 2048
			static hls::stream<float> Bcast_10_to_11("Bcast_10_to_11");
			static hls::stream<float> Bcast_10_to_12("Bcast_10_to_12");
			static hls::stream<float> Bcast_10_to_13("Bcast_10_to_13");
			static hls::stream<float> Bcast_10_to_14("Bcast_10_to_14");
			static hls::stream<float> Bcast_10_to_15("Bcast_10_to_15");
			static hls::stream<float> Bcast_10_to_16("Bcast_10_to_16");
			#pragma HLS stream variable = Bcast_10_to_11 depth = 2048
			#pragma HLS stream variable = Bcast_10_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_10_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_10_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_10_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_10_to_16 depth = 2048

			//CU_11
			static hls::stream<v_dt> Astream_11("Astream_11");
			static hls::stream<float> Bstream_11("Bstream_11");
			static hls::stream<float> Xstream_11("Xstream_11");
			#pragma HLS stream variable = Astream_11 depth = 2048
			#pragma HLS stream variable = Bstream_11 depth = 2048
			#pragma HLS stream variable = Xstream_11 depth = 2048
			static hls::stream<float> Bcast_11_to_12("Bcast_11_to_12");
			static hls::stream<float> Bcast_11_to_13("Bcast_11_to_13");
			static hls::stream<float> Bcast_11_to_14("Bcast_11_to_14");
			static hls::stream<float> Bcast_11_to_15("Bcast_11_to_15");
			static hls::stream<float> Bcast_11_to_16("Bcast_11_to_16");
			#pragma HLS stream variable = Bcast_11_to_12 depth = 2048
			#pragma HLS stream variable = Bcast_11_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_11_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_11_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_11_to_16 depth = 2048

			//CU_12
			static hls::stream<v_dt> Astream_12("Astream_12");
			static hls::stream<float> Bstream_12("Bstream_12");
			static hls::stream<float> Xstream_12("Xstream_12");
			#pragma HLS stream variable = Astream_12 depth = 2048
			#pragma HLS stream variable = Bstream_12 depth = 2048
			#pragma HLS stream variable = Xstream_12 depth = 2048
			static hls::stream<float> Bcast_12_to_13("Bcast_12_to_13");
			static hls::stream<float> Bcast_12_to_14("Bcast_12_to_14");
			static hls::stream<float> Bcast_12_to_15("Bcast_12_to_15");
			static hls::stream<float> Bcast_12_to_16("Bcast_12_to_16");
			#pragma HLS stream variable = Bcast_12_to_13 depth = 2048
			#pragma HLS stream variable = Bcast_12_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_12_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_12_to_16 depth = 2048

			//CU_13
			static hls::stream<v_dt> Astream_13("Astream_13");
			static hls::stream<float> Bstream_13("Bstream_13");
			static hls::stream<float> Xstream_13("Xstream_13");
			#pragma HLS stream variable = Astream_13 depth = 2048
			#pragma HLS stream variable = Bstream_13 depth = 2048
			#pragma HLS stream variable = Xstream_13 depth = 2048
			static hls::stream<float> Bcast_13_to_14("Bcast_13_to_14");
			static hls::stream<float> Bcast_13_to_15("Bcast_13_to_15");
			static hls::stream<float> Bcast_13_to_16("Bcast_13_to_16");
			#pragma HLS stream variable = Bcast_13_to_14 depth = 2048
			#pragma HLS stream variable = Bcast_13_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_13_to_16 depth = 2048

			//CU_14
			static hls::stream<v_dt> Astream_14("Astream_14");
			static hls::stream<float> Bstream_14("Bstream_14");
			static hls::stream<float> Xstream_14("Xstream_14");
			#pragma HLS stream variable = Astream_14 depth = 2048
			#pragma HLS stream variable = Bstream_14 depth = 2048
			#pragma HLS stream variable = Xstream_14 depth = 2048
			static hls::stream<float> Bcast_14_to_15("Bcast_14_to_15");
			static hls::stream<float> Bcast_14_to_16("Bcast_14_to_16");
			#pragma HLS stream variable = Bcast_14_to_15 depth = 2048
			#pragma HLS stream variable = Bcast_14_to_16 depth = 2048

			//CU_15
			static hls::stream<v_dt> Astream_15("Astream_15");
			static hls::stream<float> Bstream_15("Bstream_15");
			static hls::stream<float> Xstream_15("Xstream_15");
			#pragma HLS stream variable = Astream_15 depth = 2048
			#pragma HLS stream variable = Bstream_15 depth = 2048
			#pragma HLS stream variable = Xstream_15 depth = 2048
			static hls::stream<float> Bcast_15_to_16("Bcast_15_to_16");
			#pragma HLS stream variable = Bcast_15_to_16 depth = 2048

			//CU_16
			static hls::stream<v_dt> Astream_16("Astream_16");
			static hls::stream<float> Bstream_16("Bstream_16");
			static hls::stream<float> Xstream_16("Xstream_16");
			#pragma HLS stream variable = Astream_16 depth = 2048
			#pragma HLS stream variable = Bstream_16 depth = 2048
			#pragma HLS stream variable = Xstream_16 depth = 2048

			read_b(B, Bstream_1,Bstream_2,Bstream_3,Bstream_4,Bstream_5,Bstream_6,Bstream_7,Bstream_8,
					Bstream_9,Bstream_10,Bstream_11,Bstream_12,Bstream_13,Bstream_14,Bstream_15,Bstream_16,
					N, break_row1, break_row2, break_row3, break_row4, break_row5, break_row6, break_row7, break_row8,
					break_row9, break_row10, break_row11, break_row12, break_row13, break_row14, break_row15);



			read_matrix_new_1(A1, Astream_1, N, break_row1);
			read_matrix_new_2(A2, Astream_2, N, break_row1, break_row2);
			read_matrix_new_3(A3, Astream_3, N, break_row2, break_row3);
			read_matrix_new_4(A4, Astream_4, N, break_row3, break_row4);
			read_matrix_new_5(A5, Astream_5, N, break_row4, break_row5);
			read_matrix_new_6(A6, Astream_6, N, break_row5, break_row6);
			read_matrix_new_7(A7, Astream_7, N, break_row6, break_row7);
			read_matrix_new_8(A8, Astream_8, N, break_row7, break_row8);
			read_matrix_new_9(A9, Astream_9, N, break_row8, break_row9);
			read_matrix_new_10(A10, Astream_10, N, break_row9, break_row10);
			read_matrix_new_11(A11, Astream_11, N, break_row10, break_row11);
			read_matrix_new_12(A12, Astream_12, N, break_row11, break_row12);
			read_matrix_new_13(A13, Astream_13, N, break_row12, break_row13);
			read_matrix_new_14(A14, Astream_14, N, break_row13, break_row14);
			read_matrix_new_15(A15, Astream_15, N, break_row14, break_row15);
			read_matrix_new_16(A16, Astream_16, N, break_row15);


			tpsv_sync_p_1(Astream_1, Bstream_1, Xstream_1, N,
						  Bcast_1_to_2, Bcast_1_to_3, Bcast_1_to_4, Bcast_1_to_5,
						  Bcast_1_to_6, Bcast_1_to_7, Bcast_1_to_8, Bcast_1_to_9,
						  Bcast_1_to_10, Bcast_1_to_11, Bcast_1_to_12, Bcast_1_to_13,
						  Bcast_1_to_14, Bcast_1_to_15, Bcast_1_to_16,
						  break_row1 );

			tpsv_sync_p_2(Astream_2, Bstream_2, Xstream_2, N,
						  Bcast_1_to_2, Bcast_2_to_3, Bcast_2_to_4, Bcast_2_to_5,
						  Bcast_2_to_6, Bcast_2_to_7, Bcast_2_to_8, Bcast_2_to_9,
						  Bcast_2_to_10, Bcast_2_to_11, Bcast_2_to_12, Bcast_2_to_13,
						  Bcast_2_to_14, Bcast_2_to_15, Bcast_2_to_16,
						  break_row1, break_row2 );

			tpsv_sync_p_3(Astream_3, Bstream_3, Xstream_3, N,
						  Bcast_1_to_3, Bcast_2_to_3, Bcast_3_to_4, Bcast_3_to_5,
						  Bcast_3_to_6, Bcast_3_to_7, Bcast_3_to_8, Bcast_3_to_9,
						  Bcast_3_to_10, Bcast_3_to_11, Bcast_3_to_12, Bcast_3_to_13,
						  Bcast_3_to_14, Bcast_3_to_15, Bcast_3_to_16,
						  break_row1, break_row2,break_row3 );

			tpsv_sync_p_4(Astream_4, Bstream_4, Xstream_4, N,
						  Bcast_1_to_4, Bcast_2_to_4, Bcast_3_to_4, Bcast_4_to_5,
						  Bcast_4_to_6, Bcast_4_to_7, Bcast_4_to_8, Bcast_4_to_9,
						  Bcast_4_to_10, Bcast_4_to_11, Bcast_4_to_12, Bcast_4_to_13,
						  Bcast_4_to_14, Bcast_4_to_15, Bcast_4_to_16,
						  break_row1, break_row2, break_row3, break_row4 );

			tpsv_sync_p_5(Astream_5, Bstream_5, Xstream_5, N,
						  Bcast_1_to_5, Bcast_2_to_5, Bcast_3_to_5, Bcast_4_to_5,
						  Bcast_5_to_6, Bcast_5_to_7, Bcast_5_to_8, Bcast_5_to_9,
						  Bcast_5_to_10, Bcast_5_to_11, Bcast_5_to_12, Bcast_5_to_13,
						  Bcast_5_to_14, Bcast_5_to_15, Bcast_5_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5 );

			tpsv_sync_p_6(Astream_6, Bstream_6, Xstream_6, N,
						  Bcast_1_to_6, Bcast_2_to_6, Bcast_3_to_6, Bcast_4_to_6,
						  Bcast_5_to_6, Bcast_6_to_7, Bcast_6_to_8, Bcast_6_to_9,
						  Bcast_6_to_10, Bcast_6_to_11, Bcast_6_to_12, Bcast_6_to_13,
						  Bcast_6_to_14, Bcast_6_to_15, Bcast_6_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6 );

			tpsv_sync_p_7(Astream_7, Bstream_7, Xstream_7, N,
						  Bcast_1_to_7, Bcast_2_to_7, Bcast_3_to_7, Bcast_4_to_7,
						  Bcast_5_to_7, Bcast_6_to_7, Bcast_7_to_8, Bcast_7_to_9,
						  Bcast_7_to_10, Bcast_7_to_11, Bcast_7_to_12, Bcast_7_to_13,
						  Bcast_7_to_14, Bcast_7_to_15, Bcast_7_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7 );

			tpsv_sync_p_8(Astream_8, Bstream_8, Xstream_8, N,
						  Bcast_1_to_8, Bcast_2_to_8, Bcast_3_to_8, Bcast_4_to_8,
						  Bcast_5_to_8, Bcast_6_to_8, Bcast_7_to_8, Bcast_8_to_9,
						  Bcast_8_to_10, Bcast_8_to_11, Bcast_8_to_12, Bcast_8_to_13,
						  Bcast_8_to_14, Bcast_8_to_15, Bcast_8_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8 );

			tpsv_sync_p_9(Astream_9, Bstream_9, Xstream_9, N,
						  Bcast_1_to_9, Bcast_2_to_9, Bcast_3_to_9, Bcast_4_to_9,
						  Bcast_5_to_9, Bcast_6_to_9, Bcast_7_to_9, Bcast_8_to_9,
						  Bcast_9_to_10, Bcast_9_to_11, Bcast_9_to_12, Bcast_9_to_13,
						  Bcast_9_to_14, Bcast_9_to_15, Bcast_9_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9 );

			tpsv_sync_p_10(Astream_10, Bstream_10, Xstream_10, N,
						  Bcast_1_to_10, Bcast_2_to_10, Bcast_3_to_10, Bcast_4_to_10,
						  Bcast_5_to_10, Bcast_6_to_10, Bcast_7_to_10, Bcast_8_to_10,
						  Bcast_9_to_10, Bcast_10_to_11, Bcast_10_to_12, Bcast_10_to_13,
						  Bcast_10_to_14, Bcast_10_to_15, Bcast_10_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10 );

			tpsv_sync_p_11(Astream_11, Bstream_11, Xstream_11, N,
						  Bcast_1_to_11, Bcast_2_to_11, Bcast_3_to_11, Bcast_4_to_11,
						  Bcast_5_to_11, Bcast_6_to_11, Bcast_7_to_11, Bcast_8_to_11,
						  Bcast_9_to_11, Bcast_10_to_11, Bcast_11_to_12, Bcast_11_to_13,
						  Bcast_11_to_14, Bcast_11_to_15, Bcast_11_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10, break_row11 );

			tpsv_sync_p_12(Astream_12, Bstream_12, Xstream_12, N,
						  Bcast_1_to_12, Bcast_2_to_12, Bcast_3_to_12, Bcast_4_to_12,
						  Bcast_5_to_12, Bcast_6_to_12, Bcast_7_to_12, Bcast_8_to_12,
						  Bcast_9_to_12, Bcast_10_to_12, Bcast_11_to_12, Bcast_12_to_13,
						  Bcast_12_to_14, Bcast_12_to_15, Bcast_12_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10, break_row11, break_row12 );

			tpsv_sync_p_13(Astream_13, Bstream_13, Xstream_13, N,
						  Bcast_1_to_13, Bcast_2_to_13, Bcast_3_to_13, Bcast_4_to_13,
						  Bcast_5_to_13, Bcast_6_to_13, Bcast_7_to_13, Bcast_8_to_13,
						  Bcast_9_to_13, Bcast_10_to_13, Bcast_11_to_13, Bcast_12_to_13,
						  Bcast_13_to_14, Bcast_13_to_15, Bcast_13_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10, break_row11, break_row12,
						  break_row13 );

			tpsv_sync_p_14(Astream_14, Bstream_14, Xstream_14, N,
						  Bcast_1_to_14, Bcast_2_to_14, Bcast_3_to_14, Bcast_4_to_14,
						  Bcast_5_to_14, Bcast_6_to_14, Bcast_7_to_14, Bcast_8_to_14,
						  Bcast_9_to_14, Bcast_10_to_14, Bcast_11_to_14, Bcast_12_to_14,
						  Bcast_13_to_14, Bcast_14_to_15, Bcast_14_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10, break_row11, break_row12,
						  break_row13, break_row14 );

			tpsv_sync_p_15(Astream_15, Bstream_15, Xstream_15, N,
						  Bcast_1_to_15, Bcast_2_to_15, Bcast_3_to_15, Bcast_4_to_15,
						  Bcast_5_to_15, Bcast_6_to_15, Bcast_7_to_15, Bcast_8_to_15,
						  Bcast_9_to_15, Bcast_10_to_15, Bcast_11_to_15, Bcast_12_to_15,
						  Bcast_13_to_15, Bcast_14_to_15, Bcast_15_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10, break_row11, break_row12,
						  break_row13, break_row14, break_row15 );

			tpsv_sync_p_16(Astream_16, Bstream_16, Xstream_16, N,
						  Bcast_1_to_16, Bcast_2_to_16, Bcast_3_to_16, Bcast_4_to_16,
						  Bcast_5_to_16, Bcast_6_to_16, Bcast_7_to_16, Bcast_8_to_16,
						  Bcast_9_to_16, Bcast_10_to_16, Bcast_11_to_16, Bcast_12_to_16,
						  Bcast_13_to_16, Bcast_14_to_16, Bcast_15_to_16,
						  break_row1, break_row2, break_row3, break_row4,
						  break_row5, break_row6, break_row7, break_row8,
						  break_row9, break_row10, break_row11, break_row12,
						  break_row13, break_row14, break_row15 );



			write_x(Xstream_1, Xstream_2, Xstream_3, Xstream_4, Xstream_5, Xstream_6, Xstream_7, Xstream_8,
					Xstream_9, Xstream_10, Xstream_11, Xstream_12, Xstream_13, Xstream_14, Xstream_15, Xstream_16,
					X, N, break_row1, break_row2, break_row3, break_row4, break_row5, break_row6, break_row7,
					break_row8, break_row9, break_row10, break_row11, break_row12, break_row13, break_row14, break_row15);
	}

}

