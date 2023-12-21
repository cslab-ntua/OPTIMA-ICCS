void main_trsm(){

	const int Mnew=2048,Nnew=2048;
    int lda = Mnew;
    int ldb = Mnew;
    char TransA = 'N'; // ='C', = 'T' :options

    float* A= (float *)OOPS_malloc(sizeof(float)*Mnew*Mnew);
    float* B= (float *)OOPS_malloc(sizeof(float)*Mnew*Nnew);
    float* C= (float *)OOPS_malloc(sizeof(float)*Mnew*Nnew);



    MxN_matrix(B,Mnew,Nnew);
    triangular_MxN_matrix('U',A,Nnew);


    float alpha=((float) rand()) / (float) RAND_MAX;

    //verify
	float* B_sw= (float *)OOPS_malloc(sizeof(float)*Nnew*Mnew);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();


	for (int i = 0; i < Mnew; i++) {
		for (int j = i; j < Nnew; j++){
			B_sw[i*Nnew+j]=0;
			for (int k=i; k<Mnew;k++){
				B_sw[i*Nnew+j]+=(alpha*B[k*Nnew+j])/A[i*Mnew+k];
			}
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;

    OOPS_trsm('L','U',TransA, 'N', Mnew, Nnew, alpha, A, lda, B, ldb, C);

   	//verify
   	int match = 0;
   	for (int i = 0; i < Mnew; i++) {
   		for(int j=i;j<Nnew;j++){
   			if (abs(B_sw[i*Nnew+j]-C[i*Nnew+j])>0.001) {
   				std::cout << "Error: Result mismatch" << std::endl;
   			   	std::cout << "i = " << i*Nnew+j << " CPU result = " << B_sw[i*Nnew+j]
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
    free(B_sw);
}
