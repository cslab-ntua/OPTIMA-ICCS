#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <fstream> // to use i/ofstream
#include <iomanip> // to use setw and setprecision

#include "oops.hpp"
#include "test_function.h"

using namespace std;

int main(int argc, const char* argv[])
{
	printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n(0) Program the device\n");
	program_device(argv[1]);

	//-------------------------------------------------------------------------------------
	printf("\n(1) Read parameters\n");
	int	 M  = atoi(argv[2]);
	int	 K  = atoi(argv[3]);
	int	 N  = atoi(argv[4]);
	ValueType alpha = atof(argv[5]);
	ValueType beta = atof(argv[6]);

	// read parameters
	printf("- M = %10d\n",M);
	printf("- K = %10d\n",K);
	printf("- N = %10d\n",N);
	printf("- alpha = %lf\n",alpha);
	printf("- beta = %lf\n",beta);

	//-------------------------------------------------------------------------------------
	printf("\n(2) Generate matrices\n");
	auto time_gen_start = std::chrono::high_resolution_clock::now();

	ValueType* A = (ValueType*) OOPS_malloc( M * K * sizeof(ValueType) );
	ValueType* B = (ValueType*) OOPS_malloc( K * N * sizeof(ValueType) );
	ValueType* C_in = (ValueType*) OOPS_malloc( M * N * sizeof(ValueType) );
	ValueType* C_out = (ValueType*) OOPS_malloc( M * N * sizeof(ValueType) );

	srand(14); // seed the random number generator
	for (int i = 0; i < M; i++)
		for (int j = 0; j < K; j++)
			A[i * K + j] = (ValueType)rand() / RAND_MAX; // assign a random value between 0 and 1
	for (int i = 0; i < K; i++)
		for (int j = 0; j < N; j++)
			B[i * N + j] = (ValueType)rand() / RAND_MAX; // assign a random value between 0 and 1 // WRONG
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			C_in[i * N + j] = (ValueType)rand() / RAND_MAX; // assign a random value between 0 and 1

	auto time_gen_end = std::chrono::high_resolution_clock::now();
	auto time_gen = std::chrono::duration<double>(time_gen_end-time_gen_start);
	printf("time gen [s]  = %15.6lf\n",time_gen.count());

	int Layout; // 0: CblasRowMajor, 1: CblasColMajor
	Layout = 0; // Row-major
	int transa, transb; // 0: CblasNoTrans, 1: CblasTrans, 2: CblasConjTrans
	transa = 0; // CblasNoTrans
	transb = 0; // CblasNoTrans

	int lda, ldb, ldc; // leading dimensions of each array
	if((Layout == 0) && (transa == 0))
		lda = K;
	else if((Layout == 1) && (transa == 0))
		lda = M;
	else if((Layout == 0) && ((transa == 1) || (transa == 2)))
		lda = M;
	else if((Layout == 1) && ((transa == 1) || (transa == 2)))
		lda = K;

	if((Layout == 0) && (transb == 0))
		ldb = N;
	else if((Layout == 1) && (transb == 0))
		ldb = K;
	else if((Layout == 0) && ((transb == 1) || (transb == 2)))
		ldb = K;
	else if((Layout == 1) && ((transb == 1) || (transb == 2)))
		ldb = N;

	if(Layout == 0)
		ldc = N;
	else if (Layout == 1)
		ldc = M;

	//-------------------------------------------------------------------------------------
	printf("\n(3) Running test\n");
	test_function(argv[1], Layout, transa, transb, M, N, K, alpha, A, lda, B, ldb, beta, C_in, C_out, ldc);
	//-------------------------------------------------------------------------------------
	printf("\n(4) Free memory\n");
	free(A);
	free(B);
	free(C_in);
	free(C_out);

	//-------------------------------------------------------------------------------------
	printf("\n(5) Close OpenCL objects\n");
	clReleaseProgram(program_interface.program.get());
	clReleaseContext(program_interface.context.get());
	clReleaseCommandQueue(program_interface.q.get());

	//-------------------------------------------------------------------------------------

	// End
	printf("\n");
	return 0;  
}
