void main_scal(){
	
	int incX=1;
	int N = 2048;
	int NCU = 2; // 1 2 4 8 16 32
	int MAX_CUS = 32;
    float* X;
    float* X_sw;
    X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    X_sw= (float *)OOPS_malloc(sizeof(float)*N*incX);
    vector_N(X,N,incX);
    float alpha=((float) rand()) / (float) RAND_MAX;
	
	//verify
	for(int i=0;i<N;i++){
		X_sw[i] = alpha*X[i];
	}
 	OOPS_scal( N, NCU, MAX_CUS, alpha, X, incX);
	
	//verify
	int match = 0;
	for (int i = 0; i < N; i++) {
		if (abs(X_sw[i]-X[i])>0.001) {
			std::cout << "Error: Result mismatch" << std::endl;
			std::cout << "i = " << i << " CPU result = " << X_sw[i]
						  << " Device result = " << X[i] << std::endl;
			match = 1;
			break;
		}
	}

	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	free(X);
	free(X_sw);

}