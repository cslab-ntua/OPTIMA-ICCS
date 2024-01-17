void main_iamax(){

	int incX=1;
	int N=2048;
	int NCU = 2; // 1, 2, 4, 8, 12, 16
	int MAX_CUS = 16;
    float* X;

    int max_sw_idx = 0;
    int max_hw_idx = 0;

    double elapsed_krnl_time;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);
    pls_std_vector_N(X,N,incX);

	//vweify
    float max_sw_val = X[0];
	for(int i=0;i<N;i++){
		if(max_sw_val<X[i])
		{
			max_sw_val = X[i];
			max_sw_idx = i;
		}
	}

   max_hw_idx = OOPS_iamax( N, NCU, MAX_CUS, X, incX);

	//verify
	int match = 0;

	if ( max_sw_idx!=max_hw_idx) {
		std::cout << "Error: Result mismatch" << std::endl;
		match = 1;
	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

   	free(X);
}