#pragma once
#include "oops.hpp"

/*********** L1 functions **************/
float OOPS_asum(const int N, const int NCU, const int MAX_CUS, const float *X, const int incX);

void OOPS_axpy(const int N,const int NCU,const int MAX_CUS, const float alpha,const float *X, const int incX, float *Y, const int incY);

void OOPS_copy(const int N,const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY);

float OOPS_ddot(const int N, const int NCU, const int MAX_CUS, const float alpha, const float *X, const int incX, const float *Y, const int incY);

float OOPS_dot(const int N, const int NCU, const int MAX_CUS, const float *X, const int incX, const float *Y, const int incY);

int OOPS_iamax(const int N, const int NCU, const int MAX_CUS, const float  *X, const int incX);

int OOPS_iamin(const int N, const int NCU, const int MAX_CUS, const float  *X, const int incX);

float OOPS_nrm2(const int N, const int NCU, const int MAX_CUS, const float *X, const int incX);

void OOPS_rot( const int N, const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY, const float  c, const float s);

void OOPS_rotm( const int N, const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY, const float *P);

void OOPS_scal( const uint64_t N, const uint32_t NCU, const uint32_t MAX_CUS, const float alpha, float *X, const int incX);

void OOPS_swap( const int N, const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY);

/*********** L2 functions **************/
void OOPS_tpsv(const char Uplo,const char Diag,const int N, const float  *A, const uint32_t packedMatrixSize, float  *X,  const int incX);

void OOPS_spmv(const char Uplo,const char Diag,const int N, const float  *AP, const uint32_t packedMatrixSize, float alpha, float  *X, float beta, float *Yin,  const int incX);

void OOPS_tpmv(const char Uplo,const char Diag,const int N, const float  *AP, const uint32_t packedMatrixSize, float  *X, const int incX);

void OOPS_gbmv(const char TransA, const int M, const int N, const int KL, const int KU, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY);

void OOPS_gemv(const char TransA, const int M, const int N, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY);

void OOPS_sbmv(const char Uplo, const int N, const int K, const float alpha, const float  *A, const int lda, float  *X, const int incX,
                const float beta, float *Y, const int incY);

void OOPS_symv(const char Uplo, const int N, const float alpha, const float  *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY);


void OOPS_trmv(const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *A, const int lda, float  *X, const int incX);            

void OOPS_tbsv(const char Uplo, const char TransA, const char Diag, const int N,
                const int K, const float  *A, const int lda, float  *X, const int incX);

void OOPS_trsv(const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *A, const int lda, float  *X, const int incX);

void OOPS_tbmv( const char Uplo, const char TransA, const char Diag,
                 const int N, const int K, const float  *A, const int lda, float  *X, const int incX);


/*********** L3 functions **************/
void OOPS_gemm(const char TransA, const char TransB, const int M, const int N, const int K, const float alpha, const float  *A, const int lda,
				const float  *B, const int ldb, const float beta, float  *C, const int ldc);

void OOPS_symm(const char Side, const char Uplo, const int M, const int N, const float alpha, const float  *A, const int lda, const float  *B,
		        const int ldb, const float beta, float  *C, const int ldc);

void OOPS_trmm(const char Side, const char Uplo, const  char TransA, const char Diag, const int M, const int N, const float alpha,
                const float *A, const int lda, const float *B, const int ldb, float *C);

void OOPS_trsm(const char Side, const char Uplo, const  char TransA, const char Diag, const int M, const int N, const float alpha,
		        const float *A, const int lda, const float *B, const int ldb, float *C);

/********** SpMV ********************/

void OOPS_SpMV(const int nrows, const int nterm,
               const int* iat, const int* ja, const float* __restrict__ coef,
               const float* __restrict__ x, float* __restrict__ b);


/********** CAE ********************/

void OOPS_jacobi( const int N, float *X, float *Y);

bool OOPS_lu(float *L, float *U, int N);