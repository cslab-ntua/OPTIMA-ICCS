#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include <ctime>
#include <chrono>

using namespace std::chrono;

size_t OOPS_align_bytes(size_t bytesToAlign) {
	int rem = bytesToAlign % 64;
	return (rem == 0) ? bytesToAlign : bytesToAlign + 64 - rem;
}

int roundUpToMultiple(int number, int multiple){
	int rem = number % multiple;
	return (rem == 0) ? number : number + multiple - rem;
}

void printMatrix(float *m, string name, int N) {
	std::cout << "LU: " << name << std::endl;
	for (int i=0;i<N;i++) {
		for (int j=0;j<N;j++) {
			printf("%08f\t",m[i*N+j]);
		}
		std::cout << std::endl;
	}
}

void printMatrix1d(float *m, string name, int N) {
	std::cout << "LU: " << name << std::endl;
	for (int i=0;i<N;i++) {
		printf("%08f\t",m[i]);
	}
	std::cout << std::endl;
}

void printPaddedMatrix1d(float *m, string name, int N, int paddedWidth) {
	std::cout << "LU: " << name << std::endl;
	for (int i=0;i<N;i++) {
		if (i%paddedWidth == 0) {
			printf("\n");
		}
		printf("%08f\t",m[i]);
	}
	std::cout << std::endl;
}

int luRowDistribution(int N, int plEngines, int *rowsPerPlEngine) {
	int peRows = 0, lastPeRows = 0, rowsDiff = 0, maxRows = 0;

	memset(&rowsPerPlEngine[0],0,plEngines*sizeof(int));

	peRows = (int)(floor((float)N / (float)plEngines));
	lastPeRows = N - (plEngines-1)*peRows;

	for (int i=0;i<plEngines-1;i++) {
		rowsPerPlEngine[i] = peRows;
	}
	rowsPerPlEngine[plEngines-1] = lastPeRows;

	rowsDiff = lastPeRows - peRows;

	if (rowsDiff > 0) {
		lastPeRows = lastPeRows - rowsDiff;
		rowsPerPlEngine[plEngines-1] = lastPeRows;

		for (int i=0;i<rowsDiff;i++) {
			rowsPerPlEngine[i]++;
		}
	}

	for (int i=0;i<plEngines;i++) {
		if (maxRows < rowsPerPlEngine[i]) {
			maxRows = rowsPerPlEngine[i];
		}
	}

	return maxRows;

}

void printMatrix2(float *m, string name, int r, int c) {
	std::cout << "LU: " << name << "(" << r << "x" << c << ")" << std::endl;
	for (int i=0;i<r;i++) {
		for (int j=0;j<c;j++) {
			printf("%08f\t",m[i*c+j]);
		}
		std::cout << std::endl;
	}
}


bool main_lu(const int N){
	int incX=1;
    float* A, *L, *L_sw, *U, *U_sw;
    duration<double> *krnlTime, *memCpyTime,*h2dTime, *mallocTime;
    krnlTime = (duration<double> *)OOPS_malloc(sizeof(duration<double>));
    memCpyTime = (duration<double> *)OOPS_malloc(sizeof(duration<double>));
    h2dTime = (duration<double> *)OOPS_malloc(sizeof(duration<double>));
    mallocTime = (duration<double> *)OOPS_malloc(sizeof(duration<double>));
    int matrixSize = N*N;

    A= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (A == NULL) {
    	std::cout << "LU: A = NULL abort.." << std::endl;
    	return false;
    }
    L= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (L == NULL) {
		std::cout << "LU: L = NULL abort.." << std::endl;
		return false;
	}
    else {
    	memset(&L[0],0,matrixSize*sizeof(float));
    }
	L_sw = (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
	if (L_sw == NULL) {
		std::cout << "LU: L_sw = NULL abort.." << std::endl;
		return false;
	}
    U= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (U == NULL) {
		std::cout << "LU: U = NULL abort.." << std::endl;
		return false;
	}
    else {
		memset(&U[0],0,matrixSize*sizeof(float));
	}
    U_sw = (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (U_sw == NULL) {
		std::cout << "LU: U_sw = NULL abort.." << std::endl;
		return false;
	}


    if (N == 4) {
    	A[0] = 2.0f; 	A[1] = 4.0f; 	A[2] = 3.0f; 	A[3] = 5.0f;
		A[4] = -4.0f; 	A[5] = -7.0f; 	A[6] = -5.0f; 	A[7] = -8.0f;
		A[8] = 6.0f; 	A[9] = 8.0f; 	A[10] = 2.0f; 	A[11] = 9.0f;
		A[12] = 4.0f; 	A[13] = 9.0f; 	A[14] = -2.0f; 	A[15] = 14.0f;
    }
    else {
    	vector_N(A,matrixSize,incX);
    }


    //printMatrix(A,"A",N);

    memcpy(&U_sw[0],&A[0],sizeof(float)*matrixSize*incX);
    for (int i=0;i<N;i++) {
		for (int j=0;j<N;j++) {
			L_sw[i*N+j] = (i==j) ? 1.0f : 0.0f;
		}
	}
    memcpy(&U[0],&A[0],sizeof(float)*matrixSize*incX);
	memcpy(&L[0],&L_sw[0],sizeof(float)*matrixSize*incX);

	//printMatrix(L_sw,"L_sw",N);
	//printMatrix(U_sw,"U_sw",N);
	//printMatrix(L,"L",N);
	//printMatrix(U,"U",N);

	std::cout << "LU: software started (N = " << N << ").." << endl;

    high_resolution_clock::time_point swStart = high_resolution_clock::now();
    float pivot = 0.0f, x = 0.0f;
    int pivotRow = 0;

    int ops = 0;
    for (int c=0;c<N-1;c++) {
    	pivotRow = c;
    	pivot = U_sw[c*N+c];
		for (int r=c+1;r<N;r++) {
			x = U_sw[r*N+c] / pivot;
			L_sw[r*N+c] = x;
			ops++;
			for (int c1=c;c1<N;c1++) {
				U_sw[r*N+c1] = U_sw[r*N+c1] - x*U_sw[pivotRow*N+c1];
				ops+=2;
			}
		}
	}

    high_resolution_clock::time_point swEnd = high_resolution_clock::now();

    duration<double> swDuration = duration_cast<duration<double>>(swEnd - swStart);

    std::cout << "N = " << N << endl << "LU: swDuration = " << swDuration.count()*1000 << " msec" << std::endl;
    std::cout << "ops = " << ops << endl;

    //printMatrix(U_sw,"U_sw",N);

    high_resolution_clock::time_point hwStart = high_resolution_clock::now();    
    OOPS_lu(L, U, N, mallocTime,krnlTime,memCpyTime,h2dTime);


    high_resolution_clock::time_point hwEnd = high_resolution_clock::now();

    duration<double> hwDuration = duration_cast<duration<double>>(hwEnd - hwStart);
    //duration<double> mallocDuration = *mallocTime;
    //hwDuration = hwDuration - mallocDuration;


	std::cout << "LU: hwDuration = " << hwDuration.count()*1000 << " msec" << std::endl;

	//printMatrix(L_sw,"L_sw",N);
	//printMatrix(L,"L_hw",N);
	//printMatrix(U_sw,"U_sw",N);
	//printMatrix(U,"U_hw",N);


	int match = 1;
	for(int i=0;i<matrixSize;i++){
		if ((abs(L_sw[i]-L[i])>0.1) || (isnan(L[i])) || (!isfinite(L[i]))){
			 std::cout << "Error: Result mismatch in L" << std::endl;
			 std::cout << "i = " << i << " L_sw[" << i << "] = " << L_sw[i] << ", L[" << i << "] = " << L[i] << std::endl;
			 match = 0;
			 break;
		}
	}

	for(int i=0;i<matrixSize;i++){
		if ((abs(U_sw[i]-U[i])>0.1) || (isnan(U[i])) || (!isfinite(U[i]))){
			 std::cout << "Error: Result mismatch in U" << std::endl;
			 std::cout << "i = " << i << " U_sw[" << i << "] = " << U_sw[i] << ", U[" << i << "] = " << U[i] << std::endl;
			 match = 0;
			 break;
		}
	}

	std::cout << "LU: TEST " << (match ? "PASSED" : "FAILED") << std::endl;
	if (match == 1){
		std::cout << "LU: N = " << N << " (" << (N*N*sizeof(float))/(1024*1024) << " MB), hardware speedup: " << (swDuration.count() / (*krnlTime).count()) << "x" << std::endl;
		std::cout << "LU: sw = " << swDuration.count()*1000 << " msec, hw kernel = " << (*krnlTime).count()*1000 << " msec, total hw = " << hwDuration.count()*1000 << " msec" << std::endl;
		std::cout << "LU: memCpyTime = " << (*memCpyTime).count()*1000 << " msec, h2dTime = " << (*h2dTime).count()*1000 << " msec" << std::endl;
	}


    free(A);
    free(L);
    free(L_sw);
    free(U);
    free(U_sw);

    return (match == 1);
}



