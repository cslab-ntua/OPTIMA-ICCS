#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define BRAM_SIZE 704// changed from 1792 to 512// changed form 512 to 704
#define VDATA_SIZE 16
typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;


/************************ READ_X START*************************************************************************/
void read_x_Rows1(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}

}

void read_x_Rows2(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}

}

void read_x_Rows3(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}

}

void read_x_Rows4(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}

}

void read_x_Rows5(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}

}

void read_x_Rows6(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j];
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}

}

void read_x_Rows7(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}

}

void read_x_Rows8(const v_dt *X, hls::stream<v_dt>& XstreamRows, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X_rows:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			XstreamRows << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}

}

/******************************** READ Y SCAL BETA ********************************************/

void read_y_scal_beta1( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}

void read_y_scal_beta2( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}

void read_y_scal_beta3( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}

void read_y_scal_beta4( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}


void read_y_scal_beta5( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}

void read_y_scal_beta6( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}

void read_y_scal_beta7( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){



	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}

void read_y_scal_beta8( float *Y, const float beta, hls::stream<float>& ScalYBeta, const int end_row){


	read_y:
	for (int i=0; i<end_row; i++)
	{
		#pragma HLS pipeline II=1
		ScalYBeta << Y[i]*beta;
	}
}


/******************************************** SCAL AX ROWS  ************************************************************************/

void scal_ax_rows1(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows2(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1
		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;


		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows3(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1
		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows4(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1
		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows5(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1
		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows6(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1
		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;


		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows7(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1
		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}

void scal_ax_rows8(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& AmulX,const float alpha, const int vds_blk_end_idx){
	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;


		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=alpha*X_temp.data[k]*A_temp.data[k];
			}

		AmulX << temp;
		i++;
		}
}


/************************************ ACUM ROWS *************************************************************/
void accum_rows1(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;

	int blk16_idx = vds_blk_str_idx;


	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}

void accum_rows2(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){
	
	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;

	int blk16_idx = vds_blk_str_idx;


	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];

			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}

void accum_rows3(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;

	int blk16_idx = vds_blk_str_idx;


	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}

void accum_rows4(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;
	int blk16_idx = vds_blk_str_idx;
	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}
void accum_rows5(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;
	int blk16_idx = vds_blk_str_idx;
	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}

void accum_rows6(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){
	
	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;
	int blk16_idx = vds_blk_str_idx;
	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}

void accum_rows7(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){
	
	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;
	int blk16_idx = vds_blk_str_idx;
	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}

void accum_rows8(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){
	
	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ;
	int row_limit_incr = 0;
	int k = 0;
	int blk16_idx = vds_blk_str_idx;
	int row = start_row;

	non_pipelined:
	while(row < end_row)
	{
		v_dt X_temp;
		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
			X_temp.data[k]=0.0f;
		}

		pipelined:
		while(blk16_idx < row_blk16_idx_limit )
		{
			#pragma hls pipeline II=1
			v_dt temp = In.read(); // reads a block of 16 floats

			for(int k=0;k<VDATA_SIZE;k++){ // sums all the elements of the row to VDATA_SIZE vector. The sum is done by idx
				#pragma HLS unroll
				X_temp.data[k] +=temp.data[k];
			}
			blk16_idx++;
		}

		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;

		Out << X_temp;
	}


}
/**************************************************REDUCTION ROWS  *******************************************************************/
void reduction_rows1(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;
		Out << tmp15;
	}
}

void reduction_rows2(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}

void reduction_rows3(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}

void reduction_rows4(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}

void reduction_rows5(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}

void reduction_rows6(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}

void reduction_rows7(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}

void reduction_rows8(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){
	
	reduction:
	for(int i=0;i<end_row;i++){
		#pragma hls pipeline II=1

		v_dt temp = In.read();

		float tmp1 = temp.data[0] + temp.data[1];
		float tmp2 = temp.data[2] + temp.data[3];
		float tmp3 = temp.data[4] + temp.data[5];
		float tmp4 = temp.data[6] + temp.data[7];
		float tmp5 = temp.data[8] + temp.data[9];
		float tmp6 = temp.data[10] + temp.data[11];
		float tmp7 = temp.data[12] + temp.data[13];
		float tmp8 = temp.data[14] + temp.data[15];

		float tmp9 = tmp1 + tmp2;
		float tmp10 = tmp3 + tmp4;
		float tmp11 = tmp5 + tmp6;
		float tmp12 = tmp7 + tmp8;

		float tmp13 = tmp9 + tmp10;
		float tmp14 = tmp11 + tmp12;
		float tmp15 = tmp13 + tmp14;

		Out << tmp15;
	}
}
/******************************READ X COLS ***************************************************************************/
void read_x_Cols1(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){
	
	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;
	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;
			while (k<VDATA_SIZE)
			{
				innerLoop = 0;
				while(innerLoop<repLim)
				{
					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];
					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}
			i+=VDATA_SIZE;


	}
}

void read_x_Cols2(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){

	int i=start_row;
	int k=0;
	int innerLoop = 0;

	int repLim = 0;

	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;
			while (k<VDATA_SIZE)
			{

				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;


	}
}

void read_x_Cols3(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){

	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;
	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;

			while (k<VDATA_SIZE)
			{

				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;


	}
}

void read_x_Cols4(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){
	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;
	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;

			while (k<VDATA_SIZE)
			{

				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;


	}
}
void read_x_Cols5(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){

	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;

	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;
			while (k<VDATA_SIZE)
			{
				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;


	}
}

void read_x_Cols6(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){
	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;

	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;

			while (k<VDATA_SIZE)
			{

				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;
	}
}

void read_x_Cols7(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){
	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;

	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;

			while (k<VDATA_SIZE)
			{

				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;
	}
}

void read_x_Cols8(const v_dt *X, hls::stream<v_dt>& XstreamCols, const int N, const int start_row, const int end_row){
	int i=start_row;
	int k=0;
	int innerLoop = 0;
	int repLim = 0;

	read_X_cols:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[i/VDATA_SIZE]; 

			k=0;
			repLim = ((N-1-i) / VDATA_SIZE) + 1 ;

			while (k<VDATA_SIZE)
			{
				innerLoop = 0;
				while(innerLoop<repLim)
				{

					v_dt ColTmp;
					for(int m=0;m<VDATA_SIZE;m++)
					{
						#pragma HLS unroll
						ColTmp.data[m] = Xtemp.data[k];

					}
					XstreamCols << ColTmp;
					innerLoop++;
				}
				k++;
			}

			i+=VDATA_SIZE;
	}
}
/********************************READ MATRIX RNC************************************************************************************************/

void read_matrix_RnC1(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;

		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC2(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1 

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;

		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC3(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1 

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;

		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC4(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1 

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;

		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC5(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;


		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC6(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){
	
	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1 

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;

		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC7(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){
	
	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1 

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;


		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}

void read_matrix_RnC8(const v_dt *A, hls::stream<v_dt>& AStreamRows, hls::stream<v_dt>& AStreamCols, const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx){

	int row_blk16_idx_limit = ((N-start_row-1)>>4) + 1 + vds_blk_str_idx ; 
	int row_limit_incr = 0;
	int row = start_row;
	int blk16_idx = vds_blk_str_idx;
	int int_idx = 0;
	int zeroElemIdx = N%VDATA_SIZE; 
	unsigned char zeroFlag=1; 

	read_matrix_rows:
	while(row < end_row)
	{
		while(blk16_idx<row_blk16_idx_limit)
		{
			#pragma HLS pipeline II=1 

			v_dt AtempRows=A[int_idx];
			v_dt AtempCols=A[int_idx];


			if (zeroFlag){
				AtempCols.data[zeroElemIdx] = 0;
				zeroFlag = 0;
			}

			int_idx++;
			blk16_idx++;
			AStreamRows << AtempRows;
			AStreamCols << AtempCols;

		}
		row++;
		row_limit_incr = (( N - row - 1 ) >>4 ) + 1 ;
		row_blk16_idx_limit +=  row_limit_incr;
		zeroElemIdx = (zeroElemIdx + 1)%VDATA_SIZE;
		zeroFlag = 1;


	}
}


/*****************************************SCAL ACUM AND REDUCTION COLUMNS ***********************************************************************/

void scal_acum_n_red_cols1(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){
		

		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;


		int blk16_idx = vds_blk_str_idx;

		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1

				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}

						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;


		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;


		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 
				}
			}
			out_loop_idx++;

		}

	}

void scal_acum_n_red_cols2(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){

		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;

		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1

				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;


		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;

		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 

				}
			}
			out_loop_idx++;

		}

	}

void scal_acum_n_red_cols3(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){


		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;

		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1
				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}

						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;

		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;
		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 

				}
			}
			out_loop_idx++;

		}

	}


void scal_acum_n_red_cols4(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){
	
		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;

		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1
				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}

						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;

		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;
		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 
				}
			}
			out_loop_idx++;

		}

	}


void scal_acum_n_red_cols5(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){

		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;
		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1
				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;

		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;

		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 
				}
			}
			out_loop_idx++;

		}

	}

void scal_acum_n_red_cols6(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){
		

		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;

		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1

				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}

						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;
					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;

		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;

		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 

				}
			}
			out_loop_idx++;

		}

	}

void scal_acum_n_red_cols7(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){
		

		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;
		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1

				v_dt A_temp = A.read();
				v_dt X_temp = X.read();


				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}

						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;

		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;




		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 

				}
			}
			out_loop_idx++;

		}

	}


void scal_acum_n_red_cols8(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<float>& AmulX, float bram_ping[VDATA_SIZE][BRAM_SIZE],float bram_pong[VDATA_SIZE][BRAM_SIZE], const float alpha, const int N, const int start_col, const int end_col, const int vds_blk_str_idx, const int vds_blk_end_idx){
		
		for (int i = 0; i < VDATA_SIZE; i++) {
			#pragma HLS unroll
			for (int j = 0; j < BRAM_SIZE; j++) {
			#pragma hls pipeline II=1
				bram_ping[i][j] = 0.0f;
				bram_pong[i][j] = 0.0f;
			}
		}

		int col_blk16_idx_limit = ((N-start_col-1)>>4) + 1 + vds_blk_str_idx ;
		int col = start_col;
		int blk16_idx = vds_blk_str_idx;
		int curColBlk16Idx = 0; 
		int start_idx = 0;
		int curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
		int prevCurColBlk16sNum = curColBlks16Num;

		non_pipelined:
		while(col < end_col)
		{

			//Read the ready partial results from the proper memory  and drive them to ouput

			pipelined:
			while(blk16_idx < col_blk16_idx_limit )
			{
				#pragma hls pipeline II=1
				v_dt A_temp = A.read();
				v_dt X_temp = X.read();

				for(int k=0; k<VDATA_SIZE;k++)
				{
					#pragma hls unroll

					if(col%2==0)
					{
						float temp_read;
						if(col==0 && k==0)
						{
							 temp_read = 0;
						}
						else
						{
							 temp_read = bram_ping[k][curColBlk16Idx];
						}

						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_pong[k][curColBlk16Idx] = temp_res; 

					}
					else
					{
						float temp_read = bram_pong[k][curColBlk16Idx];
						float temp_res = temp_read + alpha*A_temp.data[k]*X_temp.data[k];
						bram_ping[k][curColBlk16Idx] = temp_res;

					}
				}

				blk16_idx++;
				curColBlk16Idx++;
	
			}

			col++;
			curColBlks16Num = (( N - col - 1 ) >>4 ) + 1 ; // decreases per vdata_size columns
			col_blk16_idx_limit +=  curColBlks16Num;
			if(prevCurColBlk16sNum != curColBlks16Num)
			{
				start_idx++;
			}
			curColBlk16Idx = start_idx;
			prevCurColBlk16sNum = curColBlks16Num;

		}

		int out_loop_idx = 0;
		int out_loop_zerolim = ((N-start_col-1)>>4) + 1;
		int total_blk16_num = ((N-1)>>4) + 1;
		int zeroElemvlk16  = total_blk16_num - out_loop_zerolim;

		while(out_loop_idx < total_blk16_num ) 
		{
			#pragma hls pipeline II=1
			if(out_loop_idx < zeroElemvlk16)
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << 0;
				}
			}
			else
			{
				for(int k=0; k<VDATA_SIZE;k++)
				{
					AmulX << bram_ping[k][out_loop_idx-zeroElemvlk16]; 

				}
			}
			out_loop_idx++;

		}

	}



/********************************************SUM PARTIAL RESULT FOR THE UPPER SYMMETRIC PART********************************************************/
void sum_partial_res_upper(	hls::stream<float>& PR1, hls::stream<float>& PR2, hls::stream<float>& PR3, hls::stream<float>& PR4,
							hls::stream<float>& PR5, hls::stream<float>& PR6, hls::stream<float>& PR7, hls::stream<float>& PR8,
							hls::stream<float>& ScalY1, hls::stream<float>& ScalY2, hls::stream<float>& ScalY3, hls::stream<float>& ScalY4,
							hls::stream<float>& ScalY5, hls::stream<float>& ScalY6, hls::stream<float>& ScalY7, hls::stream<float>& ScalY8,
							hls::stream<float>& PR_RES, const int N, const int break_row1, const int break_row2, const int break_row3,
							const int break_row4, const int break_row5, const int break_row6, const int break_row7)

{
	write_out:
		for(int i=0;i<break_row1;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR1.read();
			float tmp2 = ScalY1.read();
			PR_RES <<tmp + tmp2;
		}

		for(int i=break_row1;i<break_row2;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR2.read();
			float tmp2 = ScalY2.read();
			PR_RES <<tmp + tmp2;

		}

		for(int i=break_row2;i<break_row3;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR3.read();
			float tmp2 = ScalY3.read();
			PR_RES <<tmp + tmp2;

		}

		for(int i=break_row3;i<break_row4;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR4.read();
			float tmp2 = ScalY4.read();
			PR_RES <<tmp + tmp2;
		}

		for(int i=break_row4;i<break_row5;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR5.read();
			float tmp2 = ScalY5.read();
			PR_RES <<tmp + tmp2;
		}

		for(int i=break_row5;i<break_row6;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR6.read();
			float tmp2 = ScalY6.read();
			PR_RES <<tmp + tmp2;
		}

		for(int i=break_row6;i<break_row7;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR7.read();
			float tmp2 = ScalY7.read();
			PR_RES <<tmp + tmp2;

		}

		for(int i=break_row7;i<N;i++)
		{
			#pragma HLS pipeline II=1
			float tmp = PR8.read();
			float tmp2 = ScalY8.read();
			PR_RES <<tmp + tmp2;
		}
}

void sum_partial_res_brams1(hls::stream<float>& PR1, hls::stream<float>& PR2, hls::stream<float>& PR_RES, const int N)
{
	
	write_out:
		for(int i=0;i<N;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp4 = tmp1+tmp2;
			PR_RES << tmp4;

		}
}

void sum_partial_res_brams2(hls::stream<float>& PR1, hls::stream<float>& PR2, hls::stream<float>& PR_RES, const int N)
{
	write_out:
		for(int i=0;i<N;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp4 = tmp1+tmp2;
			PR_RES << tmp4;
		}
}

void sum_partial_res_brams3(hls::stream<float>& PR1, hls::stream<float>& PR2, hls::stream<float>& PR_RES, const int N)
{
	write_out:
		for(int i=0;i<N;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp4 = tmp1+tmp2;
			PR_RES << tmp4;
		}
}

void sum_partial_res_brams4(hls::stream<float>& PR1, hls::stream<float>& PR2, hls::stream<float>& PR_RES, const int N)
{
	
	write_out:
		for(int i=0;i<N;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp4 = tmp1+tmp2;
			PR_RES << tmp4;
		}
}

void sum_final(	hls::stream<float>& PR1, hls::stream<float>& PR2, hls::stream<float>& PR3,
				hls::stream<float>& PR4, hls::stream<float>& PR5,
			 	float *Y1FinRes, float *Y2FinRes, float *Y3FinRes, float *Y4FinRes,
				float *Y5FinRes, float *Y6FinRes, float *Y7FinRes, float *Y8FinRes,
				const int break_row1, const int break_row2, const int break_row3, const int break_row4,
				const int break_row5, const int break_row6, const int break_row7, const int N)
{

	write_out:

		for(int i=0;i<break_row1;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y1FinRes[i] = tmp1+tmp2+tmp3+tmp4+tmp5;
		}

		for(int i=break_row1;i<break_row2;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y2FinRes[i-break_row1] = tmp1+tmp2+tmp3+tmp4+tmp5;

		}

		for(int i=break_row2;i<break_row3;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y3FinRes[i-break_row2] = tmp1+tmp2+tmp3+tmp4+tmp5;

		}

		for(int i=break_row3;i<break_row4;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y4FinRes[i-break_row3]  = tmp1+tmp2+tmp3+tmp4+tmp5;

		}

		for(int i=break_row4;i<break_row5;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y5FinRes[i-break_row4]  = tmp1+tmp2+tmp3+tmp4+tmp5;

		}

		for(int i=break_row5;i<break_row6;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y6FinRes[i-break_row5]  = tmp1+tmp2+tmp3+tmp4+tmp5;

		}

		for(int i=break_row6;i<break_row7;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y7FinRes[i-break_row6]  = tmp1+tmp2+tmp3+tmp4+tmp5;
		}

		for(int i=break_row7;i<N;i++)
		{
			#pragma HLS pipeline II=1
			float tmp1 = PR1.read();
			float tmp2 = PR2.read();
			float tmp3 = PR3.read();
			float tmp4 = PR4.read();
			float tmp5 = PR5.read();
			Y8FinRes[i-break_row7]  = tmp1+tmp2+tmp3+tmp4+tmp5;

		}

}










extern "C" {

	void krnl_spmv(const int N, const int n16blocks,const float alpha , const float beta,
					const v_dt  *A1,  v_dt  *X_in1, v_dt  *X_in11, const int break_row1, const int break_vds_blk_idx1,
					const v_dt  *A2,  v_dt  *X_in2, v_dt  *X_in22, const int break_row2, const int break_vds_blk_idx2,
					const v_dt  *A3,  v_dt  *X_in3, v_dt  *X_in33, const int break_row3, const int break_vds_blk_idx3,
					const v_dt  *A4,  v_dt  *X_in4, v_dt  *X_in44, const int break_row4, const int break_vds_blk_idx4,
					const v_dt  *A5,  v_dt  *X_in5, v_dt  *X_in55, const int break_row5, const int break_vds_blk_idx5,
					const v_dt  *A6,  v_dt  *X_in6, v_dt  *X_in66, const int break_row6, const int break_vds_blk_idx6,
					const v_dt  *A7,  v_dt  *X_in7, v_dt  *X_in77, const int break_row7, const int break_vds_blk_idx7,
					const v_dt  *A8,  v_dt  *X_in8, v_dt  *X_in88,
					float *Y_in1, float  *Y_in2,float *Y_in3, float  *Y_in4,
					float *Y_in5, float  *Y_in6,float *Y_in7, float  *Y_in8 ){
	//CU1
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = hbm0
	#pragma HLS INTERFACE m_axi port = X_in1 offset = slave bundle = hbm1
	#pragma HLS INTERFACE m_axi port = X_in11 offset = slave bundle = hbm2

	//CU2
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = hbm3
	#pragma HLS INTERFACE m_axi port = X_in2 offset = slave bundle = hbm4
	#pragma HLS INTERFACE m_axi port = X_in22 offset = slave bundle = hbm5

	//CU3
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = hbm6
	#pragma HLS INTERFACE m_axi port = X_in3 offset = slave bundle = hbm7
	#pragma HLS INTERFACE m_axi port = X_in33 offset = slave bundle = hbm8

	//CU4
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = hbm9
	#pragma HLS INTERFACE m_axi port = X_in4 offset = slave bundle = hbm10
	#pragma HLS INTERFACE m_axi port = X_in44 offset = slave bundle = hbm11

//CU5
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = hbm12
	#pragma HLS INTERFACE m_axi port = X_in5 offset = slave bundle = hbm13
	#pragma HLS INTERFACE m_axi port = X_in55 offset = slave bundle = hbm14
//CU6
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = hbm15
	#pragma HLS INTERFACE m_axi port = X_in6 offset = slave bundle = hbm16
	#pragma HLS INTERFACE m_axi port = X_in66 offset = slave bundle = hbm17
//CU7
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = hbm18
	#pragma HLS INTERFACE m_axi port = X_in7 offset = slave bundle = hbm19
	#pragma HLS INTERFACE m_axi port = X_in77 offset = slave bundle = hbm20
//CU8
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = hbm21
	#pragma HLS INTERFACE m_axi port = X_in8 offset = slave bundle = hbm22
	#pragma HLS INTERFACE m_axi port = X_in88 offset = slave bundle = hbm23





	#pragma HLS INTERFACE m_axi port = Y_in1 offset = slave bundle = hbm24
	#pragma HLS INTERFACE m_axi port = Y_in2 offset = slave bundle = hbm25
	#pragma HLS INTERFACE m_axi port = Y_in3 offset = slave bundle = hbm26
	#pragma HLS INTERFACE m_axi port = Y_in4 offset = slave bundle = hbm27
	#pragma HLS INTERFACE m_axi port = Y_in5 offset = slave bundle = hbm28
	#pragma HLS INTERFACE m_axi port = Y_in6 offset = slave bundle = hbm29
	#pragma HLS INTERFACE m_axi port = Y_in7 offset = slave bundle = hbm30
	#pragma HLS INTERFACE m_axi port = Y_in8 offset = slave bundle = hbm31


	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = n16blocks
	#pragma HLS INTERFACE s_axilite port = alpha
	#pragma HLS INTERFACE s_axilite port = beta
	#pragma HLS INTERFACE s_axilite port = break_row1
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx1
	#pragma HLS INTERFACE s_axilite port = break_row2
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx2
	#pragma HLS INTERFACE s_axilite port = break_row3
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx3
	#pragma HLS INTERFACE s_axilite port = break_row4
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx4
	#pragma HLS INTERFACE s_axilite port = break_row5
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx5
	#pragma HLS INTERFACE s_axilite port = break_row6
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx6
	#pragma HLS INTERFACE s_axilite port = break_row7
	#pragma HLS INTERFACE s_axilite port = break_vds_blk_idx7


/// CU1 Streams
	static hls::stream<v_dt> A1_rdRows("A1_rdRows");
	static hls::stream<v_dt> A1_rdCols("A1_rdCols");

	static hls::stream<v_dt> X1_rdRows("X1_rdRows");
	static hls::stream<v_dt> AX1_rows_scal("AX1_rows_scal");
	static hls::stream<v_dt> AX1_rows_accum("AX_rows_accum");
	static hls::stream<float> AX1_rows_red("AX_rows_red");

	static hls::stream<v_dt> X1_rdCols("X1_rdCols");
	static hls::stream<float> AX1_cols_red("AX1_cols_red");


	#pragma HLS STREAM variable = A1_rdRows depth = 2048
	#pragma HLS STREAM variable = A1_rdCols depth = 2048

	#pragma HLS STREAM variable = X1_rdRows depth = 2048
	#pragma HLS STREAM variable = AX1_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX1_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX1_rows_red depth = 2048

	#pragma HLS STREAM variable = X1_rdCols depth = 2048
	#pragma HLS STREAM variable = AX1_cols_red depth = 2048
/// CU2 Streams
	static hls::stream<v_dt> A2_rdRows("A2_rdRows");
	static hls::stream<v_dt> A2_rdCols("A2_rdCols");

	static hls::stream<v_dt> X2_rdRows("X2_rdRows");
	static hls::stream<v_dt> AX2_rows_scal("AX2_rows_scal");
	static hls::stream<v_dt> AX2_rows_accum("AX2_rows_accum");
	static hls::stream<float> AX2_rows_red("A2X_rows_red");

	static hls::stream<v_dt> X2_rdCols("X2_rdCols");
	static hls::stream<float> AX2_cols_red("AX2_cols_red");

	#pragma HLS STREAM variable = A2_rdRows depth = 2048
	#pragma HLS STREAM variable = A2_rdCols depth = 2048

	#pragma HLS STREAM variable = X2_rdRows depth = 2048
	#pragma HLS STREAM variable = AX2_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX2_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX2_rows_red depth = 2048

	#pragma HLS STREAM variable = X2_rdCols depth = 2048
	#pragma HLS STREAM variable = AX2_cols_red depth = 2048
/// CU3 Streams
	static hls::stream<v_dt> A3_rdRows("A3_rdRows");
	static hls::stream<v_dt> A3_rdCols("A3_rdCols");

	static hls::stream<v_dt> X3_rdRows("X3_rdRows");
	static hls::stream<v_dt> AX3_rows_scal("AX3_rows_scal");
	static hls::stream<v_dt> AX3_rows_accum("AX3_rows_accum");
	static hls::stream<float> AX3_rows_red("A3X_rows_red");

	static hls::stream<v_dt> X3_rdCols("X3_rdCols");
	static hls::stream<float> AX3_cols_red("AX3_cols_red");

	#pragma HLS STREAM variable = A3_rdRows depth = 2048
	#pragma HLS STREAM variable = A3_rdCols depth = 2048

	#pragma HLS STREAM variable = X3_rdRows depth = 2048
	#pragma HLS STREAM variable = AX3_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX3_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX3_rows_red depth = 2048

	#pragma HLS STREAM variable = X3_rdCols depth = 2048
	#pragma HLS STREAM variable = AX3_cols_red depth = 2048
/// CU4 Streams
	static hls::stream<v_dt> A4_rdRows("A4_rdRows");
	static hls::stream<v_dt> A4_rdCols("A4_rdCols");

	static hls::stream<v_dt> X4_rdRows("X4_rdRows");
	static hls::stream<v_dt> AX4_rows_scal("AX4_rows_scal");
	static hls::stream<v_dt> AX4_rows_accum("AX4_rows_accum");
	static hls::stream<float> AX4_rows_red("A4X_rows_red");

	static hls::stream<v_dt> X4_rdCols("X4_rdCols");
	static hls::stream<float> AX4_cols_red("AX4_cols_red");

	#pragma HLS STREAM variable = A4_rdRows depth = 2048
	#pragma HLS STREAM variable = A4_rdCols depth = 2048

	#pragma HLS STREAM variable = X4_rdRows depth = 2048
	#pragma HLS STREAM variable = AX4_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX4_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX4_rows_red depth = 2048

	#pragma HLS STREAM variable = X4_rdCols depth = 2048
	#pragma HLS STREAM variable = AX4_cols_red depth = 2048
	/// CU5 Streams
	static hls::stream<v_dt> A5_rdRows("A5_rdRows");
	static hls::stream<v_dt> A5_rdCols("A5_rdCols");

	static hls::stream<v_dt> X5_rdRows("X5_rdRows");
	static hls::stream<v_dt> AX5_rows_scal("AX5_rows_scal");
	static hls::stream<v_dt> AX5_rows_accum("AX5_rows_accum");
	static hls::stream<float> AX5_rows_red("A5X_rows_red");

	static hls::stream<v_dt> X5_rdCols("X5_rdCols");
	static hls::stream<float> AX5_cols_red("AX5_cols_red");

	#pragma HLS STREAM variable = A5_rdRows depth = 2048
	#pragma HLS STREAM variable = A5_rdCols depth = 2048

	#pragma HLS STREAM variable = X5_rdRows depth = 2048
	#pragma HLS STREAM variable = AX5_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX5_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX5_rows_red depth = 2048

	#pragma HLS STREAM variable = X5_rdCols depth = 2048
	#pragma HLS STREAM variable = AX5_cols_red depth = 2048

	/// CU6 Streams
	static hls::stream<v_dt> A6_rdRows("A6_rdRows");
	static hls::stream<v_dt> A6_rdCols("A6_rdCols");

	static hls::stream<v_dt> X6_rdRows("X6_rdRows");
	static hls::stream<v_dt> AX6_rows_scal("AX6_rows_scal");
	static hls::stream<v_dt> AX6_rows_accum("AX6_rows_accum");
	static hls::stream<float> AX6_rows_red("A6X_rows_red");

	static hls::stream<v_dt> X6_rdCols("X6_rdCols");
	static hls::stream<float> AX6_cols_red("AX6_cols_red");

	#pragma HLS STREAM variable = A6_rdRows depth = 2048
	#pragma HLS STREAM variable = A6_rdCols depth = 2048

	#pragma HLS STREAM variable = X6_rdRows depth = 2048
	#pragma HLS STREAM variable = AX6_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX6_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX6_rows_red depth = 2048

	#pragma HLS STREAM variable = X6_rdCols depth = 2048
	#pragma HLS STREAM variable = AX6_cols_red depth = 2048

	/// CU7 Streams
	static hls::stream<v_dt> A7_rdRows("A7_rdRows");
	static hls::stream<v_dt> A7_rdCols("A7_rdCols");

	static hls::stream<v_dt> X7_rdRows("X7_rdRows");
	static hls::stream<v_dt> AX7_rows_scal("AX7_rows_scal");
	static hls::stream<v_dt> AX7_rows_accum("AX7_rows_accum");
	static hls::stream<float> AX7_rows_red("A7X_rows_red");

	static hls::stream<v_dt> X7_rdCols("X7_rdCols");
	static hls::stream<float> AX7_cols_red("AX7_cols_red");

	#pragma HLS STREAM variable = A7_rdRows depth = 2048
	#pragma HLS STREAM variable = A7_rdCols depth = 2048

	#pragma HLS STREAM variable = X7_rdRows depth = 2048
	#pragma HLS STREAM variable = AX7_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX7_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX7_rows_red depth = 2048

	#pragma HLS STREAM variable = X7_rdCols depth = 2048
	#pragma HLS STREAM variable = AX7_cols_red depth = 2048

	/// CU8 Streams
	static hls::stream<v_dt> A8_rdRows("A8_rdRows");
	static hls::stream<v_dt> A8_rdCols("A8_rdCols");

	static hls::stream<v_dt> X8_rdRows("X8_rdRows");
	static hls::stream<v_dt> AX8_rows_scal("AX8_rows_scal");
	static hls::stream<v_dt> AX8_rows_accum("AX8_rows_accum");
	static hls::stream<float> AX8_rows_red("A8X_rows_red");

	static hls::stream<v_dt> X8_rdCols("X8_rdCols");
	static hls::stream<float> AX8_cols_red("AX8_cols_red");

	#pragma HLS STREAM variable = A8_rdRows depth = 2048
	#pragma HLS STREAM variable = A8_rdCols depth = 2048

	#pragma HLS STREAM variable = X8_rdRows depth = 2048
	#pragma HLS STREAM variable = AX8_rows_scal depth = 2048
	#pragma HLS STREAM variable = AX8_rows_accum depth = 2048
	#pragma HLS STREAM variable = AX8_rows_red depth = 2048

	#pragma HLS STREAM variable = X8_rdCols depth = 2048
	#pragma HLS STREAM variable = AX8_cols_red depth = 2048



	static hls::stream<float> ScalYBeta1("ScalYBeta1");
	static hls::stream<float> ScalYBeta2("ScalYBeta2");
	static hls::stream<float> ScalYBeta3("ScalYBeta3");
	static hls::stream<float> ScalYBeta4("ScalYBeta4");
	static hls::stream<float> ScalYBeta5("ScalYBeta5");
	static hls::stream<float> ScalYBeta6("ScalYBeta6");
	static hls::stream<float> ScalYBeta7("ScalYBeta7");
	static hls::stream<float> ScalYBeta8("ScalYBeta8");

	static hls::stream<float> UpSymPRes("UpSymPRes");
	static hls::stream<float> LowSymPRes1("LowSymPRes1");
	static hls::stream<float> LowSymPRes2("LowSymPRes2");
	static hls::stream<float> LowSymPRes3("LowSymPRes3");
	static hls::stream<float> LowSymPRes4("LowSymPRes4");



	#pragma HLS STREAM variable = ScalYBeta1 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta2 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta3 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta4 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta5 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta6 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta7 depth = 2048
	#pragma HLS STREAM variable = ScalYBeta8 depth = 2048

	#pragma HLS STREAM variable = UpSymPRes depth = 2048
	#pragma HLS STREAM variable = LowSymPRes1 depth = 2048
	#pragma HLS STREAM variable = LowSymPRes2 depth = 2048
	#pragma HLS STREAM variable = LowSymPRes3 depth = 2048
	#pragma HLS STREAM variable = LowSymPRes4 depth = 2048

	static float bram_ping[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping complete dim=1

	static float bram_pong[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong complete dim=1

	static float bram_ping2[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping2 complete dim=1

	static float bram_pong2[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong2 complete dim=1

	static float bram_ping3[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping3 complete dim=1

	static float bram_pong3[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong3 complete dim=1

	static float bram_ping4[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping4 complete dim=1

	static float bram_pong4[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong4 complete dim=1

	static float bram_ping5[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping5 complete dim=1

	static float bram_pong5[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong5 complete dim=1

	static float bram_ping6[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping6 complete dim=1

	static float bram_pong6[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong6 complete dim=1

	static float bram_ping7[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping7 complete dim=1

	static float bram_pong7[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong7 complete dim=1

	static float bram_ping8[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_ping8 complete dim=1

	static float bram_pong8[VDATA_SIZE][BRAM_SIZE];
	#pragma HLS array_partition variable=bram_pong8 complete dim=1

	#pragma HLS dataflow


	// Read the supplied packed matrix and fill up 2 streams one for rows and one for columns for later processing
	read_matrix_RnC1(A1, A1_rdRows, A1_rdCols, N, 0			, break_row1, 0					, break_vds_blk_idx1);
	read_matrix_RnC2(A2, A2_rdRows, A2_rdCols, N, break_row1, break_row2, break_vds_blk_idx1, break_vds_blk_idx2);
	read_matrix_RnC3(A3, A3_rdRows, A3_rdCols, N, break_row2, break_row3, break_vds_blk_idx2, break_vds_blk_idx3);
	read_matrix_RnC4(A4, A4_rdRows, A4_rdCols, N, break_row3, break_row4, break_vds_blk_idx3, break_vds_blk_idx4);
	read_matrix_RnC5(A5, A5_rdRows, A5_rdCols, N, break_row4, break_row5, break_vds_blk_idx4, break_vds_blk_idx5);
	read_matrix_RnC6(A6, A6_rdRows, A6_rdCols, N, break_row5, break_row6, break_vds_blk_idx5, break_vds_blk_idx6);
	read_matrix_RnC7(A7, A7_rdRows, A7_rdCols, N, break_row6, break_row7, break_vds_blk_idx6, break_vds_blk_idx7);
	read_matrix_RnC8(A8, A8_rdRows, A8_rdCols, N, break_row7, N			, break_vds_blk_idx7, n16blocks);

/****************************************************************************************/
	read_x_Rows1(X_in1,X1_rdRows, N, 0, 			break_row1);
	read_x_Rows2(X_in2,X2_rdRows, N, break_row1, 	break_row2-break_row1);
	read_x_Rows3(X_in3,X3_rdRows, N, break_row2, 	break_row3-break_row2);
	read_x_Rows4(X_in4,X4_rdRows, N, break_row3, 	break_row4-break_row3);
	read_x_Rows5(X_in5,X5_rdRows, N, break_row4, 	break_row5-break_row4);
	read_x_Rows6(X_in6,X6_rdRows, N, break_row5, 	break_row6-break_row5);
	read_x_Rows7(X_in7,X7_rdRows, N, break_row6, 	break_row7-break_row6);
	read_x_Rows8(X_in8,X8_rdRows, N, break_row7, 	N-break_row7);

/****************************************************************************************/

		read_x_Cols1(X_in11, X1_rdCols, N, 0			, break_row1);
		read_x_Cols2(X_in22, X2_rdCols, N, break_row1	, break_row2);
		read_x_Cols3(X_in33, X3_rdCols, N, break_row2	, break_row3);
		read_x_Cols4(X_in44, X4_rdCols, N, break_row3	, break_row4);
		read_x_Cols5(X_in55, X5_rdCols, N, break_row4	, break_row5);
		read_x_Cols6(X_in66, X6_rdCols, N, break_row5	, break_row6);
		read_x_Cols7(X_in77, X7_rdCols, N, break_row6	, break_row7);
		read_x_Cols8(X_in88, X8_rdCols, N, break_row7	, N);

/****************************************************************************************/
		read_y_scal_beta1(Y_in1, beta, ScalYBeta1, break_row1			);
		read_y_scal_beta2(Y_in2, beta, ScalYBeta2, break_row2-break_row1);
		read_y_scal_beta3(Y_in3, beta, ScalYBeta3, break_row3-break_row2);
		read_y_scal_beta4(Y_in4, beta, ScalYBeta4, break_row4-break_row3);
		read_y_scal_beta5(Y_in5, beta, ScalYBeta5, break_row5-break_row4);
		read_y_scal_beta6(Y_in6, beta, ScalYBeta6, break_row6-break_row5);
		read_y_scal_beta7(Y_in7, beta, ScalYBeta7, break_row7-break_row6);
		read_y_scal_beta8(Y_in8, beta, ScalYBeta8, N-break_row7			);

		// Functions for rows
/****************************************************************************************/
		scal_ax_rows1(X1_rdRows, A1_rdRows, AX1_rows_scal, alpha, break_vds_blk_idx1);
		scal_ax_rows2(X2_rdRows, A2_rdRows, AX2_rows_scal, alpha, break_vds_blk_idx2-break_vds_blk_idx1);
		scal_ax_rows3(X3_rdRows, A3_rdRows, AX3_rows_scal, alpha, break_vds_blk_idx3-break_vds_blk_idx2);
		scal_ax_rows4(X4_rdRows, A4_rdRows, AX4_rows_scal, alpha, break_vds_blk_idx4-break_vds_blk_idx3);
		scal_ax_rows5(X5_rdRows, A5_rdRows, AX5_rows_scal, alpha, break_vds_blk_idx5-break_vds_blk_idx4);
		scal_ax_rows6(X6_rdRows, A6_rdRows, AX6_rows_scal, alpha, break_vds_blk_idx6-break_vds_blk_idx5);
		scal_ax_rows7(X7_rdRows, A7_rdRows, AX7_rows_scal, alpha, break_vds_blk_idx7-break_vds_blk_idx6);
		scal_ax_rows8(X8_rdRows, A8_rdRows, AX8_rows_scal, alpha, n16blocks-break_vds_blk_idx7);
/****************************************************************************************/
		accum_rows1(AX1_rows_scal, AX1_rows_accum, N, 0				, break_row1, 0					, break_vds_blk_idx1  );
		accum_rows2(AX2_rows_scal, AX2_rows_accum, N, break_row1	, break_row2, break_vds_blk_idx1, break_vds_blk_idx2  );
		accum_rows3(AX3_rows_scal, AX3_rows_accum, N, break_row2	, break_row3, break_vds_blk_idx2, break_vds_blk_idx3  );
		accum_rows4(AX4_rows_scal, AX4_rows_accum, N, break_row3	, break_row4, break_vds_blk_idx3, break_vds_blk_idx4  );
		accum_rows5(AX5_rows_scal, AX5_rows_accum, N, break_row4	, break_row5, break_vds_blk_idx4, break_vds_blk_idx5  );
		accum_rows6(AX6_rows_scal, AX6_rows_accum, N, break_row5	, break_row6, break_vds_blk_idx5, break_vds_blk_idx6  );
		accum_rows7(AX7_rows_scal, AX7_rows_accum, N, break_row6	, break_row7, break_vds_blk_idx6, break_vds_blk_idx7  );
		accum_rows8(AX8_rows_scal, AX8_rows_accum, N, break_row7	, N			, break_vds_blk_idx7, n16blocks			  );
/****************************************************************************************/
		reduction_rows1(AX1_rows_accum, AX1_rows_red, break_row1			);
		reduction_rows2(AX2_rows_accum, AX2_rows_red, break_row2-break_row1	);
		reduction_rows3(AX3_rows_accum, AX3_rows_red, break_row3-break_row2	);
		reduction_rows4(AX4_rows_accum, AX4_rows_red, break_row4-break_row3	);
		reduction_rows5(AX5_rows_accum, AX5_rows_red, break_row5-break_row4	);
		reduction_rows6(AX6_rows_accum, AX6_rows_red, break_row6-break_row5	);
		reduction_rows7(AX7_rows_accum, AX7_rows_red, break_row7-break_row6	);
		reduction_rows8(AX8_rows_accum, AX8_rows_red, N-break_row7			);

		// Functions for columns
/****************************************************************************************/
		scal_acum_n_red_cols1(X1_rdCols ,A1_rdCols, AX1_cols_red, bram_ping , bram_pong , alpha,  N, 0			, break_row1, 0					, break_vds_blk_idx1);
		scal_acum_n_red_cols2(X2_rdCols ,A2_rdCols, AX2_cols_red, bram_ping2, bram_pong2, alpha,  N, break_row1	, break_row2, break_vds_blk_idx1, break_vds_blk_idx2);
		scal_acum_n_red_cols3(X3_rdCols ,A3_rdCols, AX3_cols_red, bram_ping3, bram_pong3, alpha,  N, break_row2	, break_row3, break_vds_blk_idx2, break_vds_blk_idx3);
		scal_acum_n_red_cols4(X4_rdCols ,A4_rdCols, AX4_cols_red, bram_ping4, bram_pong4, alpha,  N, break_row3	, break_row4, break_vds_blk_idx3, break_vds_blk_idx4);
		scal_acum_n_red_cols5(X5_rdCols ,A5_rdCols, AX5_cols_red, bram_ping5 ,bram_pong5, alpha,  N, break_row4	, break_row5, break_vds_blk_idx4, break_vds_blk_idx5);
		scal_acum_n_red_cols6(X6_rdCols ,A6_rdCols, AX6_cols_red, bram_ping6, bram_pong6, alpha,  N, break_row5	, break_row6, break_vds_blk_idx5, break_vds_blk_idx6);
		scal_acum_n_red_cols7(X7_rdCols ,A7_rdCols, AX7_cols_red, bram_ping7, bram_pong7, alpha,  N, break_row6	, break_row7, break_vds_blk_idx6, break_vds_blk_idx7);
		scal_acum_n_red_cols8(X8_rdCols ,A8_rdCols, AX8_cols_red, bram_ping8, bram_pong8, alpha,  N, break_row7	, N			, break_vds_blk_idx7, n16blocks			);



		//Add the partial results for Rows and Cols
/****************************************************************************************/
		sum_partial_res_upper(	AX1_rows_red, AX2_rows_red, AX3_rows_red, AX4_rows_red,
								AX5_rows_red, AX6_rows_red, AX7_rows_red, AX8_rows_red,
								ScalYBeta1, ScalYBeta2, ScalYBeta3, ScalYBeta4,
								ScalYBeta5, ScalYBeta6, ScalYBeta7, ScalYBeta8,
								UpSymPRes, N, break_row1, break_row2, break_row3,
								break_row4, break_row5, break_row6, break_row7);



/****************************************************************************************/
		sum_partial_res_brams1(AX1_cols_red, AX2_cols_red, LowSymPRes1, N);
		sum_partial_res_brams2(AX3_cols_red, AX4_cols_red, LowSymPRes2, N);
		sum_partial_res_brams1(AX5_cols_red, AX6_cols_red, LowSymPRes3, N);
		sum_partial_res_brams2(AX7_cols_red, AX8_cols_red, LowSymPRes4, N);

/****************************************************************************************/


		sum_final(UpSymPRes, LowSymPRes1, LowSymPRes2,
				  LowSymPRes3, LowSymPRes4,
				  Y_in1, Y_in2, Y_in3, Y_in4,
				  Y_in5, Y_in6, Y_in7, Y_in8,
				  break_row1, break_row2, break_row3, break_row4,
				  break_row5, break_row6, break_row7, N);


	}
}
