main_iamin(int N, int NCU, int MAX_CUS){

	int incX=1;
	int N=2048;
	int NCU = 2; // 1, 2, 4, 8, 12, 16
	int MAX_CUS = 16;
    float* X;

    int min_sw_idx = 0;
    int min_hw_idx = 0;

    double elapsed_krnl_time;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);

    pls_std_vector_N(X,N,incX);
	
	//verify
    float min_sw_val = X[0];
	for(int i=0;i<N;i++){
		if(min_sw_val>X[i])
		{
			min_sw_val = X[i];
			min_sw_idx = i;
		}
	}

    min_hw_idx = OOPS_iamin( N, NCU, MAX_CUS, X, incX);

	//verify
	int match = 0;
	if ( min_sw_idx!=min_hw_idx) {
		std::cout << "Error: Result mismatch" << std::endl;
		match = 1;

	}
   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
   
   	free(X);

}

