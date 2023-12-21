void spmv_csc(const int *col_ptr, const int *row_ind, const float *values, const int num_cols, const float *x, float *y) {
    int          i, j;
    for (i=0; i < ncols; ++i) {
        temp = 0.0;
        for (j = col_ptr[i]; j < col_ptr[i+1]; ++j) {
            r[row_ind[j]] += values[j] * x[i];
            temp += values[j] * x[i];
        }

        y[row_ind[j]] = temp;
    }
}


