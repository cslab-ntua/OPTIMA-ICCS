void main_spmv(){
	int incX = 1;
	int N = 2048;

	float *X;
	float *Yin;

	float *A;
	float *Apacked;

	float alpha = ((float) rand()) / (float) RAND_MAX;
	float beta = ((float) rand()) / (float) RAND_MAX;

	char Uplo = 'U';
	char Diag =  't';

	int Opcode = 3;


	uint32_t packedMatrixSize = calc_packed_matrix_usefull_data(N);



	printf("packedMatrixSize %d \n",packedMatrixSize);

	X = (float *)OOPS_malloc(sizeof(float)*N*incX);
	Yin = (float *)OOPS_malloc(sizeof(float)*N*incX);

	memset(X, 0, sizeof(float)*N*incX);
	memset(Yin, 0, sizeof(float)*N*incX);


	A = (float *)OOPS_malloc(sizeof(float)*N*N);
	memset(A, 0, sizeof(float)*N*N);

	Apacked = (float *)OOPS_malloc(sizeof(float)*packedMatrixSize);
	memset(Apacked, 0, sizeof(float)*packedMatrixSize);

	float *sw_results = (float *)OOPS_malloc(sizeof(float)*N*incX);
	memset(sw_results, 0, sizeof(float)*N*incX);

	//fill vecs and matrix with pls1 or rand vals
	if(Opcode == 0)
	{
		// Plus 1 values in vecs and in table;
		//	//Set X
		pls_std_vector_N(X,N,incX);
		//	//Set Y
		pls_std_vector_N(Yin,N,incX);
		triangular_NxN_matrix_pls1('U', A, N);
	}
	else if(Opcode == 1)
	{
		// Plus 1 values in vecs and random table;
		//	//Set X
		pls_std_vector_N(X,N,incX);
		//	//Set Y
		pls_std_vector_N(Yin,N,incX);
		triangular_NxN_matrix('U', A, N);
	}
	else if(Opcode == 2)
	{
		// random values in vecs and plus 1 values in table;
		//Set X
		vector_N(X,N,incX);
		//Set Y
		vector_N(Yin,N,incX);
		triangular_NxN_matrix_pls1('U', A, N);
	}
	else
	{
		// random values in vecs and plus 1 values in table;
		//Set X
		vector_N(X,N,incX);
		//Set Y
		vector_N(Yin,N,incX);
		triangular_NxN_matrix('U', A, N);
	}



	printf("\n\n");

	convert_triangular_matrix_to_packed('U', A, Apacked, N, packedMatrixSize);

	// Unoptimized software implementation of the spmv algorithm for verification
	high_resolution_clock::time_point swStart = high_resolution_clock::now();
	if(Uplo == 'U' || Uplo == 'u'){
		for (int i = 0; i < N; i++){
			sw_results[i] = Yin[i]*beta;
			for (int j = 0; j <=i; j++) {
				sw_results[i]+=alpha*X[j]*A[j*N+i];
			}
			for (int j = i+1; j <N; j++) {
				sw_results[i]+=alpha*X[j]*A[i*N+j];
			}
		}
	}
	else if (Uplo == 'L' || Uplo == 'l'){
		for (int i = 0; i < N; i++){
			sw_results[i] = Yin[i]*beta;
			for (int j = 0; j <=i; j++) {
				sw_results[i]+=X[j]*A[i*N+j];
			}
			for (int j = i+1; j <N; j++) {
				sw_results[i]+=X[j]*A[j*N+i];
			}
		}
	}
	else{
		printf("Define Uplo\n");
	}
	
	OOPS_spmv(Uplo, Diag, N, Apacked,packedMatrixSize, alpha, X, beta, Yin,  incX);

	uint8_t mismatch = 0;
	float dif = 0.0;
		for (int i = 0; i < N; i++) {
			if( (abs(Yin[i]-sw_results[i]))/sw_results[i] > 0.01 ){
				dif = (abs(Yin[i]-sw_results[i]))/sw_results[i];
				printf("Error: Result mismatch sw_results[%d]: %f - Yin[%d]: %f dif: %f\n",i, sw_results[i],i, Yin[i], dif);
				mismatch = 1;
			}
		}


	std::cout << "TEST " <<(mismatch ? "FAILED" : "PASSED") << std::endl;


	free(X);
	free(A);
	free(Yin);
	free(Apacked);
	free(sw_results);

}