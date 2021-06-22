
//             LEVEL 1 BLAS 



//N (int)~> number of elements in input vector 
//X (float) ~> is a float array 
//incX (int) ~> storage spacing between elements of X
//finds the index of the first element having maximum absolute value.
int OOPS_iamax(const int N, const float  *X, const int incX);

//N (int)~> number of elements in input vector 
//X (float) ~> is a float array
//incX (integer) ~> storage spacing between elements of X
//takes the sum of the absolute values.
float OOPS_asum(const int N, const float *X, const int incX);


//N (int)~> number of elements in input vector 
//X (float) ~> a float array (input) , Y (float) ~> a float array (output) 
//alpha (float)~> specifies the scalar alpha 
//incX (int) ~> storage spacing between elements of X, incY(int) ~> storage spacing between elements of Y
void OOPS_axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY);


//N (int)~> number of elements in input vector 
//X (float) ~> a float array (input) , Y (float) ~> a float array (output) 
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_copy( const int N, const float *X, const int incX, float *Y, const int incY);


//N (int)~> number of elements in input vector 
//X (float) ~> a float array (input) , Y (float) ~> a float array (input) 
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
//forms the dot product of two vectors.
float OOPS_dot( const int N, const float *X, const int incX, const float *Y, const int incY);

//N (int)~> number of elements in input vector 
//X (float) ~> a float array (input) , Y (float) ~> a float array (input) 
//alpha (float)~> specifies the scalar alpha 
//incX (int) ~> storage spacing between elements of X, incY(int) ~> storage spacing between elements of Y
//computes the inner product of two vectors with extended precision accumulation
float OOPS_ddot( const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY);


//N (int)~> number of elements in input vector 
//X (float) ~> is a float array
//incX (int) ~> storage spacing between elements of X
//returns the euclidean norm of a vector via the function
float OOPs_nrm2( const int N, const float *X, const int incX)


//N (int)~> number of elements in input vector 
//X (float) ~> a float array (input/output) , Y (float) ~> a float array (input/output) 
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
// c (float) ~> specifies the scalar c , s (float) ~> specifies the scalar s 
void OOPS_rot( const int N, float *X, const int incX, float *Y, const int incY, const float  c, const float  s)

//N (int)~> number of elements in input vector 
//X (float) ~> a float array (output) , Y (float) ~> a float array (output) 
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
// P (float) ~> a float parameter array with dimension 5, param(1) contains a switch, flag. param(2-5) contain h11, h21, h12, and h22, respectively 
void OOPS_rotm( const int N, float *X, const int incX, float *Y, const int incY, const float *P)

//N (int)~> number of elements in input vector 
//X (float) ~> is a float array (input/output)
//alpha (float)~> specifies the scalar alpha 
//incX (int) ~> storage spacing between elements of X
void OOPS_scal( const int N, const float alpha, float *X, const int incX)

//N (int)~> number of elements in input vector 
//X (float) ~> a float array (input/output) , Y (float) ~> a float array (input/output) 
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_swap( const int N, float *X, const int incX, float *Y, const int incY)




//             LEVEL 2 BLAS 



//layout ~> enum {ColMajor, RowMajor}
//Trans (char) ~> specifies the operation to be performed 
//M (int) ~> specifies the number of rows of matrix A, N (int) ~> specifies the number of columns of matrix A
//KL (int) ~> specifies the number of sub-diagonals of matrix A, KU (int) ~> specifies the number of super-diagonals of matrix A
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta, when beta is zero then Y need not be set on input
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array , Y (float) ~> a float array
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_gbmv(const enum layout, const char TransA, const int M, const int N, const int KL, const int KU, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY)


//layout ~> enum {ColMajor, RowMajor}
//Trans (char) ~> specifies the operation to be performed 
//M (int) ~> specifies the number of rows of matrix A, N (int) ~> specifies the number of columns of matrix A
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta, when beta is zero then Y need not be set on input
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array , Y (float) ~> a float array
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_gemv(const enum layout, const char TransA, const int M, const int N, const float alpha, const float  *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY)

//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the upper or lower triangular part of the band matrix A is being supplied
//N (int) ~> specifies the order of matrix A , K (int) ~> specifies the number of super-diagonals of matrix A
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array , Y (float) ~> a float array
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_sbmv(const enum layout, const char Uplo, const int N, const int K, const float alpha, const float *A, const int lda, const float *X, const int incX,
           const float beta, float *Y, const int incY)


//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the upper or lower triangular part of array A is to be referenced
//N (int) ~> specifies the order of matrix A
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta, when beta is zero then Y need not be set on input
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array , Y (float) ~> a float array
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_symv(const enum layout, const char Uplo, const int N, const float alpha, const float  *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY)


//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the upper or lower triangular part of matrix A is supplied in the packed array AP
//N (int) ~> specifies the order of matrix A
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta, when beta is zero then Y need not be set on input
//AP (float) ~> a float array of dimension at least " ( ( n*( n + 1 ) )/2 ) "
//X (float) ~> a float array , Y (float) ~> a float array
//incX (int) ~> storage spacing between elements of X, incY (int) ~> storage spacing between elements of Y
void OOPS_spmv(const enum layout, const char Uplo, const int N, const float alpha, const float  *AP,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY)



//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular matrix
//Trans (char) ~> specifies the equation to be solved 
//Diag (char) ~> specifies whether or not A is unit triangular
//N (int) ~> specifies the order of matrix A
//K (int) ~> specifies the number of super-diagonals if Uplo is 'U', if Uplo is 'L' specifies the number of sub-diagonals
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array 
//incX (int) ~> storage spacing between elements of X
void OOPS_tbsv(const enum layout, const char Uplo, const char TransA, const char Diag,
                 const int N, const int K, const float  *A, const int lda, float  *X, const int incX)


//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular matrix
//Trans (char) ~> specifies the equation to be performed 
//Diag (char) ~> specifies whether or not A is unit triangular
//N (int) ~> specifies the order of matrix A
//K (int) ~> specifies the number of super-diagonals if Uplo is 'U', if Uplo is 'L' specifies the number of sub-diagonals
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array 
//incX (int) ~> storage spacing between elements of X
void OOPS_tbmv(const enum layout, const char Uplo, const char TransA, const char Diag,
                 const int N, const int K, const float  *A, const int lda, float  *X, const int incX)

//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular matrix
//Trans (char) ~> specifies the equation to be solved 
//Diag (char) ~> specifies whether or not Ap is unit triangular
//N (int) ~> specifies the order of matrix A
//AP (float) ~> a float array of dimension at least " ( ( n*( n + 1 ) )/2 ) "
//X (float) ~> a float array 
//incX (int) ~> storage spacing between elements of X
void OOPS_tpmv(const enum layout, const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *Ap, float  *X, const int incX)

//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular matrix
//Trans (char) ~> specifies the equation to be solved 
//Diag (char) ~> specifies whether or not Ap is unit triangular
//N (int) ~> specifies the order of matrix Ap
//AP (float) ~> a float array of dimension at least " ( ( n*( n + 1 ) )/2 ) "
//X (float) ~> a float array 
//incX (int) ~> storage spacing between elements of X
void OOPS_tpsv(const enum layout, const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *Ap, float  *X, const int incX)

//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular matrix
//Trans (char) ~> specifies the equation to be solved 
//Diag (char) ~> specifies whether or not A is unit triangular
//N (int) ~> specifies the order of matrix Ap
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array 
//incX (int) ~> storage spacing between elements of X
void OOPS_trmv(const enum layout, const char Uplo, const char TransA, const CBLAS_DIAG Diag,
                 const int N, const float  *A, const int lda, float  *X, const int incX)

//layout ~> enum {ColMajor, RowMajor}
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular matrix
//Trans (char) ~> specifies the equation to be solved 
//Diag (char) ~> specifies whether or not A is unit triangular
//N (int) ~> specifies the order of matrix Ap
//A (float) ~> a float array of dimension LDA, N
//lda (int) ~> specifies the first dimension of A 
//X (float) ~> a float array 
//incX (int) ~> storage spacing between elements of X
void OOPS_trsv(const enum layout, const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *A, const int lda, float  *X, const int incX)




//             LEVEL 3 BLAS 



//layout ~> enum {ColMajor, RowMajor}
//TransA (char) ~> specifies the form of operation A to be used in the matrix multiplication
//TransB (char) ~> specifies the form of operation B to be used in the matrix multiplication
//M (int) ~> specifies  the number  of rows  of the  matrix operation A and of the matrix C
//N (int) ~> specifies  the number  of columns  of the  matrix operation B and of the matrix C
//K (int) ~> specifies  the number of columns of the matrix operation A and the number of rows of the matrix operation B
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta, when beta is zero then C need not be set on input
//A (float) ~> a float array of dimension LDA, K or M
//lda (int) ~> specifies the first dimension of A 
//B (float) ~> a float array of dimension LDB, N or K
//ldB (int) ~> specifies the first dimension of B 
//C (float) ~> a float array of dimension LDC, N
//ldC (int) ~> specifies the first dimension of C 
void OOPS_gemm(const enum layout, const char TransA, const char TransB, const int M, const int N,
                 const int K, const float alpha, const float  *A, const int lda, const float  *B, const int ldb,
                 const float beta, float  *C, const int ldc)



//layout ~> enum {ColMajor, RowMajor}
//Side (char) ~> specifies whether the symmetric matrix A appears on the left or right in the operation
//Uplo (char) ~> specifies whether the symmetric matrix A is an upper or lower triangular
//M (int) ~> specifies  the number  of rows of the  matrix  C
//N (int) ~> specifies  the number  of columns  of the  matrix  C
//alpha (float) ~> specifies the scalar alpha , beta (float) ~> specifies the scalar beta, when beta is zero then C need not be set on input
//A (float) ~> a float array of dimension LDA, M or N
//lda (int) ~> specifies the first dimension of A 
//B (float) ~> a float array of dimension LDB, N
//ldB (int) ~> specifies the first dimension of B 
//C (float) ~> a float array of dimension LDC, N
//ldC (int) ~> specifies the first dimension of C
void OOPS_symm(const enum layout, const char Side, const char Uplo, const int M, const int N,
                 const float alpha, const float  *A, const int lda, const float  *B, const int ldb, const float beta,
                 float  *C, const int ldc)


//layout ~> enum {ColMajor, RowMajor}
//Side (char) ~> specifies whether the operation A multiplies B from the left or right
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular
//TransA (char) ~> specifies the form of operation A to be used in the matrix multiplication
//Diag (char) ~> specifies whether or not A is unit triangular
//M (int) ~> specifies  the number  of rows of the  matrix  B
//N (int) ~> specifies  the number  of columns  of the  matrix  B
//alpha (float) ~> specifies the scalar alpha 
//A (float) ~> a float array of dimension LDA, M or N
//lda (int) ~> specifies the first dimension of A 
//B (float) ~> a float array of dimension LDB, N
//ldB (int) ~> specifies the first dimension of B 
void OOPS_trmm(const enum layout, const char Side, const char Uplo, const  char TransA,
                 const char Diag, const int M, const int N, const float alpha, const float *A, const int lda,
                 float *B, const int ldb)


//layout ~> enum {ColMajor, RowMajor}
//Side (char) ~> specifies whether the operation A multiplies B from the left or right
//Uplo (char) ~> specifies whether the matrix A is an upper or lower triangular
//TransA (char) ~> specifies the form of operation A to be used in the matrix multiplication
//Diag (char) ~> specifies whether or not A is unit triangular
//M (int) ~> specifies  the number  of rows of the  matrix  B
//N (int) ~> specifies  the number  of columns  of the  matrix  B
//alpha (float) ~> specifies the scalar alpha 
//A (float) ~> a float array of dimension LDA, M or N
//lda (int) ~> specifies the first dimension of A 
//B (float) ~> a float array of dimension LDB, N
//ldB (int) ~> specifies the first dimension of B 
void OOPS_trsm(const enum layout, const char Side, const char Uplo, const char TransA,
                 const char Diag, const int M, const int N, const float alpha, const float  *A, const CBLAS_INDEX lda,
                 float  *B, const CBLAS_INDEX ldb)
