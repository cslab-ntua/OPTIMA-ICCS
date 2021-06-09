float ** cholSerial(float ** A, int n){
	double ** L = initMatrix(n);
	transCopy(A, L, n);
	
	int i,j,k;
	for (j = 0; j < n; j++){
		
		for (k = 0; k < j; k++){
			// Inner sum
			for (i = j; i < n; i++){
				L[i][j] = L[i][j] - L[i][k] * L[j][k];
			}
		}
		
		L[j][j] = sqrt(L[j][j]);
		
		for (i = j+1; i < n; i++){
			L[i][j] = L[i][j]/ L[j][j];
		}
	}
	
	return L;
}
