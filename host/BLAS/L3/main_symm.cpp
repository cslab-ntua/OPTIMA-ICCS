void main_symm(){

	const int Mnew=2048,Nnew=2048;
    int lda = Mnew;
    int ldb = Mnew;
    int ldc = Mnew;

    float* A= (float *)OOPS_malloc(sizeof(float)*Mnew*Mnew);
    float* B= (float *)OOPS_malloc(sizeof(float)*Mnew*Nnew);
    float* C= (float *)OOPS_malloc(sizeof(float)*Mnew*Nnew);



    MxN_matrix(B,Mnew,Nnew);
    triangular_MxN_matrix('U',A,Nnew);
    MxN_matrix(C,Mnew,Nnew);


    float alpha=((float) rand()) / (float) RAND_MAX;
    float beta=((float) rand()) / (float) RAND_MAX;

    //verify
	float* C_sw= (float *)OOPS_malloc(sizeof(float)*Nnew*Mnew);

	for (int i = 0; i < Mnew; i++) {
		for (int j = 0; j < Nnew; j++){
			C_sw[i*Nnew+j]=C[i*Nnew+j];

		}
	}

    high_resolution_clock::time_point t1 = high_resolution_clock::now();


	for (int i = 0; i < Mnew; i++) {
		for (int j = 0; j < Nnew; j++){
			C_sw[i*Nnew+j]=beta*C_sw[i*Nnew+j];
			for (int k=0; k<Mnew;k++){
				if(i<=k){
					C_sw[i*Nnew+j]+=alpha*A[i*Mnew+k]*B[k*Nnew+j];
				}
				else{
					C_sw[i*Nnew+j]+=alpha*A[k*Mnew+i]*B[k*Nnew+j];
				}
			}
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;

    OOPS_symm('L','U', Mnew, Nnew, alpha, A, lda, B, ldb, beta, C, ldc);

   	//verify
   	int match = 0;
   	for (int i = 0; i < Mnew; i++) {
   		for(int j=0;j<Nnew;j++){
   			if (abs(C_sw[i*Nnew+j]-C[i*Nnew+j])>0.001) {
   				std::cout << "Error: Result mismatch" << std::endl;
   			   	std::cout << "i = " << i*Nnew+j << " CPU result = " << C_sw[i*Nnew+j]
   			   	                      << " Device result = " << C[i*Nnew+j] << std::endl;
   			   	match = 1;
   			   	//break;
   			}
   		}
   	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(A);
    free(B);
    free(C);
    free(C_sw);
}

