
// col_ptr (int*) --> vector containing the index of the first non-zero entry of each column in the value vector
// row_ind (int*) --> vector containing row index of each non zero entry
// values (float*) --> vector containing all non-zero entries
// num_cols (int) --> number of columns of the matrix
// nnz (int) --> number of nnz values
// x (float*) --> input vector 
// y (float*) --> output vector 
void spmv_csc(const int *col_ptr, const int *row_ind, const float *values, 
                                const int num_cols, int nnz, const float *x, float *y);



// row_ptr (int*) --> vector containing the index of the first non-zero entry of each row in the value vector
// col_ind (int*) --> vector containing column index of each non zero entry
// values (float*) --> vector containing all non-zero entries
// num_rows (int) --> number of rows of the matrix
// nnz (int) --> number of nnz values
// x (float*) --> input vector 
// y (float*) --> output vector 
void spmv_csr(const int *row_ptr, const int *col_ind, const float *values, 
                                const int num_rows, int nnz, const float *x, float *y);



// A (float**) --> input natrix (n x n) to decompose
// n (int) --> matrix dimension
// The function stores inplace (A) the decomposed matrix
void cholesky(float ** A, int n);



// A (float**) --> input natrix (n x n) to decompose
// n (int) --> matrix dimension
// L (float**) --> output lower triangular
// U (float**) --> output upper triangular
void lu(float **A, int n, float **L, float **U);



// Adata (float**) --> the input matrix to calculate the pre-conditioner (n x n)
// n (int) --> matrix dimension
// Mdata (float*) --> the ouptut vector (pre-conditioner) 
void jacobi_matrix(float **Adata, int n, float *Mdata);



// Mdata (float*) --> the pre-conditioner vector
// x (float*) --> the input vector to apply the pre-conditioner
// n (int) --> the size of the vectors
// Minvx (float*) --> the output (pre-conditioned) vector 
void jakobi_precond(float *Mdata, float *x, int n, float *Minvx);


// Preconditioned CG to solve symmetric linear system Adata*x=b
// Adata (float**) --> the input matrix (n x n)
// b (float*) --> the input vector
// x (float*) --> the output vector
// n (int) --> the matrix and vector dimension
// rtol (double) --> relative tolerance 
// maxiter (int) --> a maximum number of iterations to apply the method
// niters (int *) --> the number of iters it took until convergenece (output)
void precond_cg(float **Adata, float *b, float *x, double rtol, int n, int maxiter, int *niters);
