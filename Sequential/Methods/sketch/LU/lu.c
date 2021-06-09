LU lu(Matrix matrix){
    int n = matrix.n;
    float **A = matrix.matrix;

    for (int i = 0; i < n; i++) { // Iterates over the columns to remove
        for(int j = i + 1; j < n; j++){
            A[j][i] = A[j][i] / A[i][i]; // Computes the multipliers and updates L in A
        }
        for(int j = i + 1; j < n; j++){ // Iterates over the remaining rows
            for(int k = i+1; k < n; k++){ // iterates over the remaining columns
                A[j][k] -= A[j][i] * A[i][k]; // Updates U in A
            }
        }
    }

    LU decomposition = split_lu(A, n);
    return decomposition;
}
