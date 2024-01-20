timeMeasurements main_tpmv(){
	int incX = 1;
	int N = 2048;
	double elapsed_krnl_time;

	float *X;
	float *A;
	float *Apacked;

	char Uplo = 'U';
	char Diag =  't';


	uint32_t packedMatrixSize = calc_packed_matrix_usefull_data(N);

	X = (float *)OOPS_malloc(sizeof(float)*N*incX);
	float *X_out = (float *)OOPS_malloc(sizeof(float)*N*incX);

	memset(X_out, 0, sizeof(float)*N*incX);

	A = (float *)OOPS_malloc(sizeof(float)*N*N);
	memset(A, 0, sizeof(float)*N*N);

	Apacked = (float *)OOPS_malloc(sizeof(float)*packedMatrixSize);
	memset(Apacked, 0, sizeof(float)*packedMatrixSize);

	float *sw_results = (float *)OOPS_malloc(sizeof(float)*N*incX);
	memset(sw_results, 0, sizeof(float)*N*incX);

	pls_std_vector_N(X,N,incX);
	triangular_NxN_matrix_pls1('U', A, N);
	convert_triangular_matrix_to_packed('U', A, Apacked, N, packedMatrixSize);


	if(Uplo == 'U' || Uplo == 'u'){
		for (int i = 0; i < N; i++){
			sw_results[i] = 0;
			for (int j = 0; j < N; j++) {
				sw_results[i]+=X[j]*A[i*N+j];
			}
		}
	}
	else if (Uplo == 'L' || Uplo == 'l'){
		for (int i = 0; i < N; i++){
			sw_results[i] = 0;
			for (int j = 0; j < N; j++) {
				sw_results[i]+=X[j]*A[i*N+j];
			}
		}
	}
	else{
		printf("Define Uplo\n");
	}

	OOPS_tpmv(Uplo, Diag, N, Apacked,packedMatrixSize, X, incX);

	uint8_t mismatch = 0;
		for (int i = 0; i < N; i++) {
			if( abs(X[i]- sw_results[i])/sw_results[i]>00.1 ){
				printf("Error: Result mismatch\n %f - %f ",sw_results[i],X[i]);
				mismatch = 1;
			}
		}


	std::cout << "TEST " <<(mismatch ? "FAILED" : "PASSED") << std::endl;

	free(X);
	free(A);
	free(X_out);
	free(Apacked);
	free(sw_results);

}