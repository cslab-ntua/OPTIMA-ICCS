main_tpsv(){
	int incX = 1;
    int N = 2048
	timeMeasurements tm;
	double elapsed_krnl_time;

	float *X;

	float *A, *ATrans;
	float *ATransPacked;


	char Uplo = 'L';
	char Diag =  't';



	uint32_t packedMatrixSize = calc_packed_matrix_usefull_data(N);



	printf("packedMatrixSize %d \n",packedMatrixSize);

	X = (float *)OOPS_malloc(sizeof(float)*N*incX);
	memset(X, 0, sizeof(float)*N*incX);


	A = (float *)OOPS_malloc(sizeof(float)*N*N);
	memset(A, 0, sizeof(float)*N*N);

	ATrans = (float *)OOPS_malloc(sizeof(float)*N*N);
	memset(ATrans, 0, sizeof(float)*N*N);

	ATransPacked = (float *)OOPS_malloc(sizeof(float)*packedMatrixSize);
	memset(ATransPacked, 0, sizeof(float)*packedMatrixSize);

	float *sw_results = (float *)OOPS_malloc(sizeof(float)*N*incX);
	memset(sw_results, 0, sizeof(float)*N*incX);



	vector_N(X,N,incX);
	memcpy(sw_results, X, sizeof(float)*N);

	triangular_NxN_matrix('L', A, N);

	printf("\n\n");

	//Transpose A from lower triangular to upper triangular
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			ATrans[j*N+i] = A[i*N+j];
		}
	}

	//verify for lower case only
	// Unoptimized software implementation of the tpsv algorithm
	high_resolution_clock::time_point swStart = high_resolution_clock::now();
	for(int i=0; i<N; i++){
		float a = sw_results[i];
		for(int j=0; j<i; j++){
			a -= sw_results[j] * ATrans[j*N+i];
		}
		sw_results[i] = a;
	}

	high_resolution_clock::time_point swEnd = high_resolution_clock::now();
	duration<double> swDuration = duration_cast<duration<double>>(swEnd - swStart);




	convert_triangular_matrix_to_packed('U', ATrans, ATransPacked, N, packedMatrixSize);


	high_resolution_clock::time_point hwStart = high_resolution_clock::now();

	OOPS_tpsv_16cus('U', Diag, N, ATransPacked, packedMatrixSize, X, incX, &elapsed_krnl_time);
	high_resolution_clock::time_point hwEnd = high_resolution_clock::now();

	duration<double> hwDuration = duration_cast<duration<double>>(hwEnd - hwStart);


	uint8_t mismatch = 0;
	float dif = 0.0;
		for (int i = 0; i < N; i++) {
			if( (abs(X[i]-sw_results[i]))/sw_results[i] > 0.01 ){
				dif = (abs(X[i]-sw_results[i]))/sw_results[i];
				printf("Error: Result mismatch sw_results[%d]: %f - X[%d]: %f dif: %f\n",i, sw_results[i],i, X[i], dif);
				mismatch = 1;
			}
		}


	std::cout << "TEST " <<(mismatch ? "FAILED" : "PASSED") << std::endl;


	free(X);
	free(A);
	free(ATrans);
	free(ATransPacked);
	free(sw_results);

}