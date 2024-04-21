#include <omp.h>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>

#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include "test_functions_set.h"


using namespace std;
using namespace std::chrono;


int main(int argc, const char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n(0) Program the device\n");
	program_device(argv[1]);

    int N=1280, incX=1, incY=1;

    float *X = (float *)OOPS_malloc(sizeof(float)*N*incX);
    float *Y = (float *)OOPS_malloc(sizeof(float)*N*incY);
    float *A = (float *)OOPS_malloc(sizeof(float)*N*N);

    int lda = N;
    char Uplo = 'U'; // ='u', or ='L','l' for lower
    vector_N(X,N,incX);
    vector_N(Y,N,incY);
    symmetric_matrix(A, N);
    float alpha = ((float) rand()) / (float) RAND_MAX;
    float beta = ((float) rand()) / (float) RAND_MAX;

    float sw_results[N];

    for (int i = 0; i < N; i++) {
        sw_results[i]=beta*Y[i];
        for (int j = 0; j < N; j++){
            sw_results[i]=sw_results[i]+X[j]*A[i*N+j]*alpha;
        }
    }

    OOPS_symv(Uplo, N,alpha,A,lda, X, incX,beta, Y, incY);

    int match = 0;
    for (int i = 0; i < N; i++) {
        if (abs(Y[i]- sw_results[i])/sw_results[i]>0.001) {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << sw_results[i] << " Device result = " << Y[i] << std::endl;
            match = 1;
        }
    }

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(X);
    free(Y);
    free(A);


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
