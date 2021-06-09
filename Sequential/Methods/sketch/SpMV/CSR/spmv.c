void spmv_csr(const int *row_ptr, const int *col_ind, const float *values, const int num_rows, const float *x, float *y) {
    for (int i = 0; i < num_rows; i++) {
        float dotProduct = 0;
        const int row_start = row_ptr[i];
        const int row_end = row_ptr[i + 1];
        
        for (int j = row_start; j < row_end; j++) {
            dotProduct += values[j] * x[col_ind[j]];
        }
        
        y[i] = dotProduct;
    }
}

