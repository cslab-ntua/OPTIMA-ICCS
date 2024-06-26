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
    const int N=2048;
    int incX=1;
    int lda = N;
    //enum layout {ColMajor=N, RowMajor=M};
    //layout layout_item;
    char Uplo = 'L'; // ='U',='u', or ='L','l' for lower
    char TransA = 'N'; // ='C', = 'T' :options
    char Diag='N'; // ='n', or ='U','u', for unit triangular matrix
    float* X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    float* A= (float *)OOPS_malloc(sizeof(float)*N*N);

    float* sw_results= (float *)OOPS_malloc(sizeof(float)*N*incX);

    vector_N(X,N,incX);
    memcpy(sw_results, X, sizeof(float)*N);

    triangular_NxN_matrix(Uplo,A,N);

    //verify
    if (Uplo == 'U' or Uplo == 'u'){
        for (int i = 0; i < N; i++) {
            //sw_results[i]=0;
            for (int j = i ;j < N; j++){
                //sw_results[i]+=X[j]*A[i*N+j];
            }
        }
    }
    else if (Uplo == 'L' or Uplo == 'l'){
        for (int i = 0; i < N; i++) {
                float a = sw_results[i];
                for (int j = 0; j < i; j++){
                    a-=sw_results[j]*A[i*N+j];
                }
                sw_results[i] = a;
            }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }

    OOPS_trsv(Uplo, TransA, Diag, N, A, lda, X, incX);

    //verify
    int match = 0;
    for (int i = 0; i < N; i++) {
        std::cout << "i = " << i << " CPU result = " << sw_results[i]<< " Device result = " << X[i];
        if (abs(X[i]- sw_results[i])/sw_results[i]>0.001) {
            std::cout << "  Error: Result mismatch";
            match = 1;
        }
        std::cout << std::endl;
    }
    
    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(X);
    free(A);
    free(sw_results);

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
