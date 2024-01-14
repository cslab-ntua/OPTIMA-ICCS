void main_dot(){

	int incX=1, incY=1;
	int N=2048;
	int NCU = 2; // 1, 2, 4, 8, 12, 16
	int MAX_CUS = 16;
    float *X;
    float *Y;

    float sw_sum_of_mults = 0.0;
   	float hw_sum_of_mults;
    double elapsed_krnl_time;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);
    Y=(float*)OOPS_malloc(sizeof(float)*N*incY);

    vector_N(X,N,incX);
    vector_N(Y,N,incY);

	//verify
    for(int i=0;i<N;i++){

		sw_sum_of_mults += X[i] * Y[i];

	}

    hw_sum_of_mults = OOPS_dot( N, NCU, MAX_CUS, X, incX, Y, incY);
	int match = 0;

	//verify
	if ( (abs(sw_sum_of_mults-hw_sum_of_mults)>0.01)) {
		std::cout << "Error: Result mismatch" << std::endl;
		std::cout << "sw_sum_of_mults : " << sw_sum_of_mults << "  hw_sum_of_mults: " << hw_sum_of_mults << "diff: " << (abs(sw_sum_of_mults-hw_sum_of_mults)) << std::endl;
		match = 1;

	}


   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;


   	free(X);
   	free(Y);


   	return tm;

}