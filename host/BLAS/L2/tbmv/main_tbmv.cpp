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

        char Uplo = 'U'; // ='U',='u', or ='L','l' for lower
        char TransA = 'N'; // ='C', = 'T' :options
        char Diag='N'; // ='n', or ='U','u', for unit triangular matrix

        int N=7168, incX=1;
        int K=281;

        int lda = K+1;
        float *X= (float *)OOPS_malloc(sizeof(float)*N*incX);
        float *A= (float *)OOPS_malloc(sizeof(float)*N*(K+1));

        vector_N(X,N,incX);

        int N_cols = (K/16 + 1)*16;
        float *Apadded= (float *)OOPS_malloc(sizeof(float)*N*N_cols);
        memset(Apadded, 0, (sizeof(float)*N*N_cols));

        trband_matrix(Uplo,A,Apadded,N,K);

        //verify
        float *sw_results= (float *)malloc(sizeof(float)*N*incX);

        if(TransA=='N' && Uplo =='U'){
                for(int i=0;i<N;i++){
                sw_results[i]=0;
                        for (int j=i;j<std::min(N,i+K+1);j++){
                                sw_results[i]+=X[j]*A[-i+j+i*(K+1)];
                        }
                }
        }

        OOPS_tbmv(Uplo, TransA, Diag, N, K, Apadded, lda, X, incX);

        //verify
        int match = 0;
        for (int i = 0; i < N; i++) {
                if (abs(X[i]- sw_results[i])/sw_results[i]>0.001) {
                        std::cout << "Error: Result mismatch" << std::endl;
                        std::cout << "i = " << i << " CPU result = " << sw_results[i]<< " Device result = " << X[i] << std::endl;
                        match = 1;

                }
        }

        std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

        free(X);
        free(A);
        free(sw_results);
        free(Apadded);


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

