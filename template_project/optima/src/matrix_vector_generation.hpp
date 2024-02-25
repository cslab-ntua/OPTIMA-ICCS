#ifndef MATRIX_VECTOR_GENERATION_HPP
#define MATRIX_VECTOR_GENERATION_HPP
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <unistd.h>
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
