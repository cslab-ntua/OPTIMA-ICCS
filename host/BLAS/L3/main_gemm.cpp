void main_gemm(){

	const int K=2048,Mnew=2048,Nnew=2048;
    int lda = Mnew;
    int ldb = K;
    int ldc = Mnew;
    char TransA = 'N'; // ='C', = 'T' :options
    char TransB = 'N'; // ='C', = 'T' :options

    float* A= (float *)OOPS_malloc(sizeof(float)*K*Mnew);
    float* B= (float *)OOPS_malloc(sizeof(float)*K*Nnew);
    float* C= (float *)OOPS_malloc(sizeof(float)*Nnew*Mnew);

    if(TransA=='T' && TransB=='T'){
        MxN_matrix(A,K,Mnew);
        MxN_matrix(B,Nnew,K);
    }
    else if(TransA=='N' && TransB=='T'){
        MxN_matrix(A,Mnew,K);
        MxN_matrix(B,Nnew,K);
    }
    else if(TransA=='T' && TransB=='N'){
        MxN_matrix(A,K,Mnew);
        MxN_matrix(B,K,Nnew);
    }
    else{
        MxN_matrix(A,Mnew,K);
        MxN_matrix(B,K,Nnew);
    }

    MxN_matrix(C,Mnew,Nnew);

    float alpha=((float) rand()) / (float) RAND_MAX;
    float beta=((float) rand()) / (float) RAND_MAX;

    //verify
	float* C_sw= (float *)OOPS_malloc(sizeof(float)*Nnew*Mnew);


    high_resolution_clock::time_point t1 = high_resolution_clock::now();


	for (int i = 0; i < Mnew; i++) {
		for (int j = 0; j < Nnew; j++){
			C_sw[i*Nnew+j]=beta*C[i*Nnew+j];
			for (int k=0; k<K;k++){
				if(TransA=='T' && TransB=='T'){
					C_sw[i*Nnew+j]+=alpha*A[k*Mnew+i]*B[j*K+k];
				}
				else if(TransA=='N' && TransB=='T'){
					C_sw[i*Nnew+j]+=alpha*A[i*K+k]*B[j*K+k];
				}
				else if(TransA=='T' && TransB=='N'){
					C_sw[i*Nnew+j]+=alpha*A[k*Mnew+i]*B[k*Nnew+j];
				}
				else{
					C_sw[i*Nnew+j]+=alpha*A[i*K+k]*B[k*Nnew+j];
				}
			}
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;

    OOPS_gemm(TransA, TransB, Mnew, Nnew, K, alpha, A, lda, B, ldb, beta, C, ldc);

   	//verify
   	int match = 0;
   	for (int i = 0; i < Mnew*Nnew; i++) {
   		if (abs(C_sw[i]-C[i])>0.001) {
   			std::cout << "Error: Result mismatch" << std::endl;
   	        std::cout << "i = " << i << " CPU result = " << C_sw[i]
   	                      << " Device result = " << C[i] << std::endl;
   	        match = 1;
   	        break;
   	    }
   	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(A);
    free(B);
    free(C);
    free(C_sw);
}
