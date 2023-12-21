void cholesky(float ** A, int n){
	int i,j,k;
	for (j = 0; j < n; j++){
		
		for (k = 0; k < j; k++){
			// Inner sum
			for (i = j; i < n; i++){
				A[i][j] = A[i][j] - A[i][k] * A[j][k];
			}
		}
		
		A[j][j] = sqrt(A[j][j]);
		
		for (i = j+1; i < n; i++){
			A[i][j] = A[i][j]/ A[j][j];
		}
	}
}

// Check again 
// https://github.com/mdwarfgeek/lib/blob/master/cholesky.c
