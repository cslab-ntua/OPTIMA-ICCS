#ifndef MATRIX_VECTOR_GENERATION_HPP
#define MATRIX_VECTOR_GENERATION_HPP
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <unistd.h>

const float FLAG[4] = {-1.0, 0.0, 1.0, -2.0};

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

inline void vector_N(float *X,int N,int incX){
      int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
}

inline void std_vector_N(float *X,int N,int incX, float num){
      int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = num;
        x_index=x_index + incX;
    }
}

inline void pls_std_vector_N(float *X,int N,int incX){
      int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = float(i);
        x_index=x_index + incX;
    }
}

inline void gen_rotm_param(float *fH){
//	float flag[4] = {-1.0, 0.0, 1.0, -2.0};
	srand(time(NULL));
	float chosen_flag = FLAG[rand()%4]; //Choose randomly between the 4 values of the flag array
	fH[0] = chosen_flag;
	#ifdef DEBUG_MATR
	std::cout << "Chosen flag is :" << chosen_flag << std::endl;
	#endif
	if(chosen_flag == 0.0)
	{
		fH[1] = 1.0;
		fH[2] = ((float) rand()) / (float) RAND_MAX;
		fH[3] = ((float) rand()) / (float) RAND_MAX;
		fH[4] = 1.0;
	}
	else if(chosen_flag == 1.0)
	{
		fH[1] = ((float) rand()) / (float) RAND_MAX;
		fH[2] = 1.0;
		fH[3] = -1.0;
		fH[4] = ((float) rand()) / (float) RAND_MAX;
	}
	else if(chosen_flag == -2.0)
	{
		fH[1] = 1.0;
		fH[2] = 0.0;
		fH[3] = 0.0;
		fH[4] = 1.0;
	}
	else
	{
		fH[1] = ((float) rand()) / (float) RAND_MAX;
		fH[2] = ((float) rand()) / (float) RAND_MAX;
		fH[3] = ((float) rand()) / (float) RAND_MAX;
		fH[4] = ((float) rand()) / (float) RAND_MAX;
	}

}

inline void banded_MxN_matrix(float *A,int rows,int columns,int KU, int KL){
    float *Atemp;
    Atemp= (float *)OOPS_malloc(sizeof(float)*rows*columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0 ;j < columns; j++){
            //Atemp[i*columns+j] = ((float) rand()) / (float) RAND_MAX;
            Atemp [i*columns+j]=i*10+j;
            std::cout <<"Atemp["<<i<<"]["<<j<<"]="<<Atemp[i*columns+j]<<std::endl;
        }
    }
    for (int j=0;j<columns;j++){
        for (int i=std::max(0,j-KU);i<std::min(rows,j+KL+1);i++){
            A[(KU+i-j)*columns+j]=Atemp[i*columns+j];
        }
    }
    free (Atemp);

}
inline void symmetric_matrix(float *A,int N){
    for (int i = 0; i < N; i++) {
        for (int j = i ;j < N; j++){
            A[i*N+j] = ((float) rand()) / (float) RAND_MAX;
            A[j*N+i] = A[i*N+j];
        }
    }
}
inline void symmetric_banded_matrix();
inline void symmetric_packed_matrix();
inline void triangular_MxN_matrix(const char Uplo,float *A, int N){
    if (Uplo == 'U' or Uplo == 'u'){
        for (int i = 0; i < N; i++) {
            for (int j = i ;j < N; j++){
                A[i*N+j] = ((float) rand()) / (float) RAND_MAX;
            }
        }
    }
    else if (Uplo == 'L' or Uplo == 'l'){
        for (int i = 0; i < N; i++) {
            for (int j = 0 ;j < i+1; j++){
                A[i*N+j] = ((float) rand()) / (float) RAND_MAX;
            }
        }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }
}
inline void triangular_banded_MxN_matrix();
inline void MxN_matrix(float *A,int rows,int columns){
    for (int i = 0; i < rows; i++) {
        for (int j = 0 ;j < columns; j++){
            A[i*columns+j] = ((float) rand()) / (float) RAND_MAX;
        }
    }
}

#endif
