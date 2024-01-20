#include <hls_stream.h>
#include <ap_int.h>
#include <hls_math.h>
#include <stdio.h>

#define VDATA_SIZE 16
typedef struct v_datatype { float data[VDATA_SIZE]; } v_dt;



/************************ READ_X START*************************************************************************/
void read_x_1(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;
	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}

	}
}

void read_x_2(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;
	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
void read_x_3(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;
	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
void read_x_4(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;
	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
void read_x_5(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
void read_x_6(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;

	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
void read_x_7(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;
	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
void read_x_8(const v_dt *X, hls::stream<v_dt>& Xstream, const int N, const int start_row, const int end_row){

	int i=0,j=start_row>>4;
	read_X:
	while(i<end_row){
		#pragma HLS pipeline II=1
			v_dt Xtemp;
			Xtemp = X[j]; 
			Xstream << Xtemp;

			if (j==(N>>4)-1){
				i++;
				j=(i+start_row)>>4;
			}
			else{
				j++;
			}
	}
}
/************************ READ_X END *****************************************************************************/

/************************ WRITE X START **************************************************************************/
void write_x_1(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_2(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_3(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_4(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_5(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_6(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_7(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

void write_x_8(hls::stream<float>& Ystream,float *Y, const int end_row){

	write_x:
	for(int i=0;i<end_row;i++){
		#pragma HLS pipeline II=1
		Y[i] = Ystream.read();

	}
}

/************************ WRITE X END **************************************************************************/

/************************ READ_MATRIX START**********************************************************************/

void read_matrix_1(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){

	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_2(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){

	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_3(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){
	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_4(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){
	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_5(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){
	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_6(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){
	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_7(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){

	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}

void read_matrix_8(const v_dt *A, hls::stream<v_dt>& AStream, const int vds_blk_end_idx){
	int i=0;

	wide_read:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt Atemp=A[i];
		AStream << Atemp;

		i++;
	}
}
/************************ READ_MATRIX END **************************************************************************/

/************************ SCAL_AX START*************************************************************************/
void scal_ax_1(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];
			}

		Y << temp;
		i++;
		}
}
void scal_ax_2(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}

void scal_ax_3(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}
void scal_ax_4(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}

void scal_ax_5(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}
void scal_ax_6(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}

void scal_ax_7(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}
void scal_ax_8(hls::stream<v_dt>& X,hls::stream<v_dt>& A, hls::stream<v_dt>& Y, const int vds_blk_end_idx){

	int i=0;

	gemv_trig:
	while(i<vds_blk_end_idx){
		#pragma HLS pipeline II=1

		v_dt A_temp = A.read();
		v_dt X_temp = X.read();
		v_dt temp;
	

		for(int k=0;k<VDATA_SIZE;k++){
			#pragma HLS unroll
				temp.data[k]=X_temp.data[k]*A_temp.data[k];

			}

		Y << temp;
		i++;
		}
}
/************************ SCAL_AX END *************************************************************************/

/************************ REDUCTION START *************************************************************************/
void reduction_1(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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

void reduction_2(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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

void reduction_3(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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

void reduction_4(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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

void reduction_5(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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
void reduction_6(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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
void reduction_7(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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
void reduction_8(hls::stream<v_dt>& In,hls::stream<float>& Out,const int end_row){

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
/************************ REDUCTION END *************************************************************************/

/************************ ACUM START *************************************************************************/
void accum_1(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){
//	printf("accum_1 \n");

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

void accum_2(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){

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

void accum_3(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


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

void accum_4(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


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

void accum_5(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


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

void accum_6(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


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

void accum_7(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


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

void accum_8(hls::stream<v_dt>& In,hls::stream<v_dt>& Out,const int N, const int start_row, const int end_row, const int vds_blk_str_idx, const int vds_blk_end_idx  ){


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


/************************ ACUM END *************************************************************************/




extern "C" {

void krnl_tpmv(const int N, const int n16blocks,
				const v_dt  *A1, v_dt  *X_in1, float  *X_out1, const int break_row1, const int break_vds_blk_idx1,
				const v_dt  *A2, v_dt  *X_in2, float  *X_out2, const int break_row2, const int break_vds_blk_idx2,
				const v_dt  *A3, v_dt  *X_in3, float  *X_out3, const int break_row3, const int break_vds_blk_idx3,
				const v_dt  *A4, v_dt  *X_in4, float  *X_out4, const int break_row4, const int break_vds_blk_idx4,
				const v_dt  *A5, v_dt  *X_in5, float  *X_out5, const int break_row5, const int break_vds_blk_idx5,
				const v_dt  *A6, v_dt  *X_in6, float  *X_out6, const int break_row6, const int break_vds_blk_idx6,
				const v_dt  *A7, v_dt  *X_in7, float  *X_out7, const int break_row7, const int break_vds_blk_idx7,
				const v_dt  *A8, v_dt  *X_in8, float  *X_out8
				 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 ){
	//CU1
	#pragma HLS INTERFACE m_axi port = A1 offset = slave bundle = hbm0
	#pragma HLS INTERFACE m_axi port = X_in1 offset = slave bundle = hbm1
	#pragma HLS INTERFACE m_axi port = X_out1 offset = slave bundle = hbm2

	//CU2
	#pragma HLS INTERFACE m_axi port = A2 offset = slave bundle = hbm3
	#pragma HLS INTERFACE m_axi port = X_in2 offset = slave bundle = hbm4
	#pragma HLS INTERFACE m_axi port = X_out2 offset = slave bundle = hbm5

	//CU3
	#pragma HLS INTERFACE m_axi port = A3 offset = slave bundle = hbm6
	#pragma HLS INTERFACE m_axi port = X_in3 offset = slave bundle = hbm7
	#pragma HLS INTERFACE m_axi port = X_out3 offset = slave bundle = hbm8

	//CU4
	#pragma HLS INTERFACE m_axi port = A4 offset = slave bundle = hbm9
	#pragma HLS INTERFACE m_axi port = X_in4 offset = slave bundle = hbm10
	#pragma HLS INTERFACE m_axi port = X_out4 offset = slave bundle = hbm11

	//CU5
	#pragma HLS INTERFACE m_axi port = A5 offset = slave bundle = hbm12
	#pragma HLS INTERFACE m_axi port = X_in5 offset = slave bundle = hbm13
	#pragma HLS INTERFACE m_axi port = X_out5 offset = slave bundle = hbm14

	//CU6
	#pragma HLS INTERFACE m_axi port = A6 offset = slave bundle = hbm15
	#pragma HLS INTERFACE m_axi port = X_in6 offset = slave bundle = hbm16
	#pragma HLS INTERFACE m_axi port = X_out6 offset = slave bundle = hbm17

	//CU7
	#pragma HLS INTERFACE m_axi port = A7 offset = slave bundle = hbm18
	#pragma HLS INTERFACE m_axi port = X_in7 offset = slave bundle = hbm19
	#pragma HLS INTERFACE m_axi port = X_out7 offset = slave bundle = hbm20

	//CU8
	#pragma HLS INTERFACE m_axi port = A8 offset = slave bundle = hbm21
	#pragma HLS INTERFACE m_axi port = X_in8 offset = slave bundle = hbm22
	#pragma HLS INTERFACE m_axi port = X_out8 offset = slave bundle = hbm23



	#pragma HLS INTERFACE s_axilite port = N
	#pragma HLS INTERFACE s_axilite port = n16blocks

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





	static hls::stream<v_dt> A1_rd("A1_rd");
	static hls::stream<v_dt> X1_rd("X1_rd");
	static hls::stream<v_dt> X1_scal("X1_scal");
	static hls::stream<v_dt> X1_accum("X1_accum");
	static hls::stream<float> X1_red("X1_red");

	#pragma HLS STREAM variable = A1_rd depth = 2048
	#pragma HLS STREAM variable = X1_rd depth = 2048
	#pragma HLS STREAM variable = X1_scal depth = 2048
	#pragma HLS STREAM variable = X1_accum depth = 2048
	#pragma HLS STREAM variable = X1_red depth = 2048

	static hls::stream<v_dt> A2_rd("A2_rd");
	static hls::stream<v_dt> X2_rd("X2_rd");
	static hls::stream<v_dt> X2_scal("X2_scal");
	static hls::stream<v_dt> X2_accum("X2_accum");
	static hls::stream<float> X2_red("X2_red");

	#pragma HLS STREAM variable = A2_rd depth = 2048
	#pragma HLS STREAM variable = X2_rd depth = 2048
	#pragma HLS STREAM variable = X2_scal depth = 2048
	#pragma HLS STREAM variable = X2_accum depth = 2048
	#pragma HLS STREAM variable = X2_red depth = 2048

	static hls::stream<v_dt> A3_rd("A3_rd");
	static hls::stream<v_dt> X3_rd("X3_rd");
	static hls::stream<v_dt> X3_scal("X3_scal");
	static hls::stream<v_dt> X3_accum("X3_accum");
	static hls::stream<float> X3_red("X3_red");

	#pragma HLS STREAM variable = A3_rd depth = 2048
	#pragma HLS STREAM variable = X3_rd depth = 2048
	#pragma HLS STREAM variable = X3_scal depth = 2048
	#pragma HLS STREAM variable = X3_accum depth = 2048
	#pragma HLS STREAM variable = X3_red depth = 2048

	static hls::stream<v_dt> A4_rd("A4_rd");
	static hls::stream<v_dt> X4_rd("X4_rd");
	static hls::stream<v_dt> X4_scal("X4_scal");
	static hls::stream<v_dt> X4_accum("X4_accum");
	static hls::stream<float> X4_red("X4_red");

	#pragma HLS STREAM variable = A4_rd depth = 2048
	#pragma HLS STREAM variable = X4_rd depth = 2048
	#pragma HLS STREAM variable = X4_scal depth = 2048
	#pragma HLS STREAM variable = X4_accum depth = 2048
	#pragma HLS STREAM variable = X4_red depth = 2048

	static hls::stream<v_dt> A5_rd("A5_rd");
	static hls::stream<v_dt> X5_rd("X5_rd");
	static hls::stream<v_dt> X5_scal("X5_scal");
	static hls::stream<v_dt> X5_accum("X5_accum");
	static hls::stream<float> X5_red("X5_red");

	#pragma HLS STREAM variable = A5_rd depth = 2048
	#pragma HLS STREAM variable = X5_rd depth = 2048
	#pragma HLS STREAM variable = X5_scal depth = 2048
	#pragma HLS STREAM variable = X5_accum depth = 2048
	#pragma HLS STREAM variable = X5_red depth = 2048

	static hls::stream<v_dt> A6_rd("A6_rd");
	static hls::stream<v_dt> X6_rd("X6_rd");
	static hls::stream<v_dt> X6_scal("X6_scal");
	static hls::stream<v_dt> X6_accum("X6_accum");
	static hls::stream<float> X6_red("X6_red");

	#pragma HLS STREAM variable = A6_rd depth = 2048
	#pragma HLS STREAM variable = X6_rd depth = 2048
	#pragma HLS STREAM variable = X6_scal depth = 2048
	#pragma HLS STREAM variable = X6_accum depth = 2048
	#pragma HLS STREAM variable = X6_red depth = 2048

	static hls::stream<v_dt> A7_rd("A7_rd");
	static hls::stream<v_dt> X7_rd("X7_rd");
	static hls::stream<v_dt> X7_scal("X7_scal");
	static hls::stream<v_dt> X7_accum("X7_accum");
	static hls::stream<float> X7_red("X7_red");

	#pragma HLS STREAM variable = A7_rd depth = 2048
	#pragma HLS STREAM variable = X7_rd depth = 2048
	#pragma HLS STREAM variable = X7_scal depth = 2048
	#pragma HLS STREAM variable = X7_accum depth = 2048
	#pragma HLS STREAM variable = X7_red depth = 2048

	static hls::stream<v_dt> A8_rd("A8_rd");
	static hls::stream<v_dt> X8_rd("X8_rd");
	static hls::stream<v_dt> X8_scal("X8_scal");
	static hls::stream<v_dt> X8_accum("X8_accum");
	static hls::stream<float> X8_red("X8_red");

	#pragma HLS STREAM variable = A8_rd depth = 2048
	#pragma HLS STREAM variable = X8_rd depth = 2048
	#pragma HLS STREAM variable = X8_scal depth = 2048
	#pragma HLS STREAM variable = X8_accum depth = 2048
	#pragma HLS STREAM variable = X8_red depth = 2048

	int lim_row2 = break_row2-break_row1;
	int lim_row3 = break_row3-break_row2;
	int lim_row4 = break_row4-break_row3;
	int lim_row5 = break_row5-break_row4;
	int lim_row6 = break_row6-break_row5;
	int lim_row7 = break_row7-break_row6;
	int lim_row8 = N-break_row7;

	int lim_vds2 = break_vds_blk_idx2-break_vds_blk_idx1;
	int lim_vds3 = break_vds_blk_idx3-break_vds_blk_idx2;
	int lim_vds4 = break_vds_blk_idx4-break_vds_blk_idx3;
	int lim_vds5 = break_vds_blk_idx5-break_vds_blk_idx4;
	int lim_vds6 = break_vds_blk_idx6-break_vds_blk_idx5;
	int lim_vds7 = break_vds_blk_idx7-break_vds_blk_idx6;
	int lim_vds8 = n16blocks-break_vds_blk_idx7;




	#pragma HLS dataflow

	read_x_1(X_in1,X1_rd,N,0,break_row1);
	read_x_2(X_in2,X2_rd,N,break_row1,lim_row2);
	read_x_3(X_in3,X3_rd,N,break_row2,lim_row3);
	read_x_4(X_in4,X4_rd,N,break_row3,lim_row4);
	read_x_5(X_in5,X5_rd,N,break_row4,lim_row5);
	read_x_6(X_in6,X6_rd,N,break_row5,lim_row6);
	read_x_7(X_in7,X7_rd,N,break_row6,lim_row7);
	read_x_8(X_in8,X8_rd,N,break_row7,lim_row8);


	read_matrix_1(A1, A1_rd, break_vds_blk_idx1);
	read_matrix_2(A2, A2_rd, lim_vds2);
	read_matrix_3(A3, A3_rd, lim_vds3);
	read_matrix_4(A4, A4_rd, lim_vds4);
	read_matrix_5(A5, A5_rd, lim_vds5);
	read_matrix_6(A6, A6_rd, lim_vds6);
	read_matrix_7(A7, A7_rd, lim_vds7);
	read_matrix_8(A8, A8_rd, lim_vds8);


	scal_ax_1(X1_rd, A1_rd, X1_scal, break_vds_blk_idx1);
	scal_ax_2(X2_rd, A2_rd, X2_scal, lim_vds2);
	scal_ax_3(X3_rd, A3_rd, X3_scal, lim_vds3);
	scal_ax_4(X4_rd, A4_rd, X4_scal, lim_vds4);
	scal_ax_5(X5_rd, A5_rd, X5_scal, lim_vds5);
	scal_ax_6(X6_rd, A6_rd, X6_scal, lim_vds6);
	scal_ax_7(X7_rd, A7_rd, X7_scal, lim_vds7);
	scal_ax_8(X8_rd, A8_rd, X8_scal, lim_vds8);


	accum_1(X1_scal, X1_accum, N, 0			, break_row1, 0					, break_vds_blk_idx1);
	accum_2(X2_scal, X2_accum, N, break_row1, break_row2, break_vds_blk_idx1, break_vds_blk_idx2);
	accum_2(X3_scal, X3_accum, N, break_row2, break_row3, break_vds_blk_idx2, break_vds_blk_idx3);
	accum_2(X4_scal, X4_accum, N, break_row3, break_row4, break_vds_blk_idx3, break_vds_blk_idx4);
	accum_2(X5_scal, X5_accum, N, break_row4, break_row5, break_vds_blk_idx4, break_vds_blk_idx5);
	accum_2(X6_scal, X6_accum, N, break_row5, break_row6, break_vds_blk_idx5, break_vds_blk_idx6);
	accum_2(X7_scal, X7_accum, N, break_row6, break_row7, break_vds_blk_idx6, break_vds_blk_idx7);
	accum_2(X8_scal, X8_accum, N, break_row7, N			, break_vds_blk_idx7, n16blocks			);

	reduction_1(X1_accum, X1_red, break_row1);
	reduction_2(X2_accum, X2_red, lim_row2);
	reduction_3(X3_accum, X3_red, lim_row3);
	reduction_4(X4_accum, X4_red, lim_row4);
	reduction_5(X5_accum, X5_red, lim_row5);
	reduction_6(X6_accum, X6_red, lim_row6);
	reduction_7(X7_accum, X7_red, lim_row7);
	reduction_8(X8_accum, X8_red, lim_row8);


	write_x_1(X1_red, X_out1, break_row1);
	write_x_2(X2_red, X_out2, lim_row2);
	write_x_3(X3_red, X_out3, lim_row3);
	write_x_4(X4_red, X_out4, lim_row4);
	write_x_5(X5_red, X_out5, lim_row5);
	write_x_6(X6_red, X_out6, lim_row6);
	write_x_7(X7_red, X_out7, lim_row7);
	write_x_8(X8_red, X_out8, lim_row8);



}}



