
timeMeasurements main_swap(){
	timeMeasurements tm;
	int incX=1, incY=1;
	int N = 2048;
	int NCU = 2; // 1 2 4 8 16 32
	int MAX_CUS = 16;

    float* X;
    float* Y;

    float* X_sw;
    float* Y_sw;
    X=(float*)OOPS_malloc(sizeof(float)*N*incX);
    Y=(float*)OOPS_malloc(sizeof(float)*N*incY);
    X_sw=(float*)OOPS_malloc(sizeof(float)*N*incX);
    Y_sw=(float*)OOPS_malloc(sizeof(float)*N*incY);

    vector_N(X,N,incX);
    vector_N(Y,N,incY);

    //verify
	for(int i=0;i<N;i++){

		X_sw[i] = Y[i];
		Y_sw[i] = X[i];
	}

OOPS_swap( N, NCU, MAX_CUS, X, incX, Y, incY);

//verify
	int match = 0;
   	for (int i = 0; i < N; i++) {
   		if ( (abs(Y_sw[i]-Y[i])>0.001) || (abs(X_sw[i]-X[i])>0.001) ) {
  			mismatchNum++;
  			std::cout << "Error: Result mismatch" << " i = " << i << " CPU res Y = " << Y_sw[i]
  	                      << " Device result Y = " << Y[i] << " CPU res X = " << X_sw[i]
  	 						  << " Device result X = " << X[i] << "  mismatchNum : "<< mismatchNum <<  std::endl;
  	        std::cout << "Error: Result mismatch" << std::endl;
  	        	        std::cout << "i = " << i << " CPU res X = " << X_sw[i]
						  << " Device result X = " << X[i] << std::endl;
   	        match = 1;
   	        break;
   	    }
   	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") <<  std::endl;

   	free(X);
   	free(Y);
   	free(Y_sw);
   	free(X_sw);


}