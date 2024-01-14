void main_asum(){

	int incX=1;
	int N=2048;
	int NCU = 2;
	int MAX_CUS = 16;
    float* X;

    double sum_sw = 0.0;
    double sum_hw = 0.0;
    double elapsed_krnl_time;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);

    std_vector_N(X, N, incX, 1.0);

//verify
	for(int i=0;i<N;i++){

		sum_sw += X[i];
	}

   sum_hw = OOPS_asum( N, NCU, MAX_CUS, X, incX);

//verify
	int match = 0;

	if ( (abs(sum_hw-sum_sw)>0.10) ) {
		std::cout << "Error: Result mismatch" << std::endl;
		match = 1;

	}
   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
   
   	free(X);

}