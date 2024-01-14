void main_copy(){
	
	int incX=1, incY=1;
	int N=2048;
	int NCU = 2; // 1, 2, 4, 8, 12, 16
	int MAX_CUS = 16;

    float* X;
    float* Y;
    float* Y_sw;
    X=(float*)OOPS_malloc(sizeof(float)*N*incX);
    Y=(float*)OOPS_malloc(sizeof(float)*N*incY);
    Y_sw=(float*)OOPS_malloc(sizeof(float)*N*incY);

    vector_N(X,N,incX);
	//verify
	for(int i=0;i<N;i++){
		Y_sw[i] = X[i];
	}

   

    OOPS_copy( N, NCU, MAX_CUS, X, incX, Y, incY);
	
	//verify
	int match = 0;
   	for (int i = 0; i < N; i++) {
   		if (abs(Y_sw[i]-Y[i])>0.001) {
   			std::cout << "Error: Result mismatch" << std::endl;
   	        std::cout << "i = " << i << " CPU result = " << Y_sw[i]
   	                      << " Device result = " << Y[i] << std::endl;
   	        match = 1;
   	        break;
   	    }
   	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

   	free(X);
   	free(Y);
   	free(Y_sw);


}