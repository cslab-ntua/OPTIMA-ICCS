
void main_jacobi(int N, int NCUs){
	int incX=1;
    float* A, *M, *M_sw;
    duration<double> *krnlTime;
    krnlTime = (duration<double> *)OOPS_malloc(sizeof(duration<double>));
    int matrixSize = N*N;

    A= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    M= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    M_sw= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);

    diagonal_N(A,N);

    //printMatrix(A,"A",N);

    for (int i=0;i<matrixSize;i++) {
    	M[i] = 0.0f;
    	M_sw[i] = 0.0f;
    }

    high_resolution_clock::time_point swStart = high_resolution_clock::now();

    for (int i=0;i<matrixSize;i=i+N+1) {
    	A[i] != 0.0f ? M_sw[i] = 1.0f / A[i] : M_sw[i] = 0.0f;
    }
    high_resolution_clock::time_point swEnd = high_resolution_clock::now();

    duration<double> swDuration = duration_cast<duration<double>>(swEnd - swStart);

    //printMatrix(M_sw,"M_sw",N);

    high_resolution_clock::time_point hwStart = high_resolution_clock::now();
    OOPS_jacobi( N, A, M,krnlTime);
    high_resolution_clock::time_point hwEnd = high_resolution_clock::now();

    duration<double> hwDuration = duration_cast<duration<double>>(hwEnd - hwStart);

    /*std::cout << "JACOBI: Matrix M:" << std::endl;
	for (int i=0;i<N;i++) {
		for (int j=0;j<N;j++) {
			std::cout << M[i*N+j] << " ";
		}
		std::cout << std::endl;
	}*/

    int match = 1;
	for(int i=0;i<matrixSize;i++){
		if(abs(M_sw[i]-M[i])>0.1){
			 std::cout << "Error: Result mismatch" << std::endl;
			 std::cout << "i = " << i << " CPU result = " << M_sw[i] << ", device result = " << M[i] << std::endl;
			 match = 0;
			 break;
		}
	}

	std::cout << "JACOBI: TEST " << (match ? "PASSED" : "FAILED") << std::endl;
	if (match == 1){
		std::cout << "JACOBI: N = " << N << " (" << (N*N*sizeof(float))/(1024*1024) << " MB), hardware speedup: " << (swDuration.count() / (*krnlTime).count()) << "x" << std::endl;
		std::cout << "JACOBI: sw = " << swDuration.count()*1000 << " msec, hw kernel = " << (*krnlTime).count()*1000 << " msec, mem transfer = " << (hwDuration.count() - (*krnlTime).count())*1000 << " msec" << std::endl;
	}

    free(A);
    free(M);
    free(M_sw);
}


