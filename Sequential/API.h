
void spmv_csc(const int *col_ptr, const int *row_ind, const float *values, const int num_cols, const float *x, float *y);

void spmv_csr(const int *row_ptr, const int *col_ind, const float *values, const int num_rows, const float *x, float *y);

void cholesky(float ** A, int n, float **L);

void lu(float *matrix, int n, float *L, float *U);

void jacobi_matrix(double *M, double **Adata, int n);

void jakobi_precond(double *Minvx, double *Mdata, double *x, int n);

void precond_cg(double **Adata, double *b, double *x, double rtol, int n, int maxiter, int *niters);
