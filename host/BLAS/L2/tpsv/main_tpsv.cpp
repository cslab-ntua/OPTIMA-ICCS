#include <chrono>
#include <stdio.h>
#include <stdlib.h>

#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include "test_functions_set.h"

using namespace std;

int main(int argc, const char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n(0) Program the device\n");
	program_device(argv[1]);
	
	int incX = 1;
    int N = 2048;

	float *X;

	float *A, *ATrans;
	float *ATransPacked;


	char Uplo = 'L';
	char Diag =  't';


	uint32_t packedMatrixSize = calc_packed_matrix_usefull_data(N);


	printf("packedMatrixSize %d \n",packedMatrixSize);

	X = (float *)OOPS_malloc(sizeof(float)*N*incX);
	memset(X, 0, sizeof(float)*N*incX);


	A = (float *)OOPS_malloc(sizeof(float)*N*N);
	memset(A, 0, sizeof(float)*N*N);

	ATrans = (float *)OOPS_malloc(sizeof(float)*N*N);
	memset(ATrans, 0, sizeof(float)*N*N);

	ATransPacked = (float *)OOPS_malloc(sizeof(float)*packedMatrixSize);
	memset(ATransPacked, 0, sizeof(float)*packedMatrixSize);

	float *sw_results = (float *)OOPS_malloc(sizeof(float)*N*incX);
	memset(sw_results, 0, sizeof(float)*N*incX);



	vector_N(X,N,incX);
	memcpy(sw_results, X, sizeof(float)*N);

	triangular_NxN_matrix('L', A, N);

	printf("\n\n");

	//Transpose A from lower triangular to upper triangular
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			ATrans[j*N+i] = A[i*N+j];
		}
	}

	//verify for lower case only
	for(int i=0; i<N; i++){
		float a = sw_results[i];
		for(int j=0; j<i; j++){
			a -= sw_results[j] * ATrans[j*N+i];
		}
		sw_results[i] = a;
	}

	convert_triangular_matrix_to_packed('U', ATrans, ATransPacked, N, packedMatrixSize);

	OOPS_tpsv('U', Diag, N, ATransPacked, packedMatrixSize, X, incX);
	
	uint8_t mismatch = 0;
	float dif = 0.0;
		for (int i = 0; i < N; i++) {
			if( (abs(X[i]-sw_results[i]))/sw_results[i] > 0.01 ){
				dif = (abs(X[i]-sw_results[i]))/sw_results[i];
				printf("Error: Result mismatch sw_results[%d]: %f - X[%d]: %f dif: %f\n",i, sw_results[i],i, X[i], dif);
				mismatch = 1;
			}
		}


	std::cout << "TEST " <<(mismatch ? "FAILED" : "PASSED") << std::endl;


	free(X);
	free(A);
	free(ATrans);
	free(ATransPacked);
	free(sw_results);

	//-------------------------------------S------------------------------------------------
	printf("\n(5) Close OpenCL objects\n");
	clReleaseProgram(program_interface.program.get());
	clReleaseContext(program_interface.context.get());
	clReleaseCommandQueue(program_interface.q.get());

	//-------------------------------------------------------------------------------------

	// End
	printf("\n");

    return 0;

}