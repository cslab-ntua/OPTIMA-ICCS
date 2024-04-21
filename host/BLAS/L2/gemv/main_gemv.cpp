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

	int M=1792,N=1792, incX=1, incY=1;

    	float* X;
    	float* Y;
    	float *A;
    	int lda = M;
    	//enum layout {ColMajor=N, RowMajor=M};
    	//layout layout_item;
    	char TransA = 'N'; // ='C', = 'T' :options
    	X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    	Y= (float *)OOPS_malloc(sizeof(float)*M*incY);
    	float* sw_results= (float *)OOPS_malloc(sizeof(float)*M*incY);

    	A= (float *)OOPS_malloc(sizeof(float)*N*M);

    	vector_N(X,N,incX);
    	vector_N(Y,M,incY);
    	MxN_matrix(A,N,M);
    	float alpha= ((float) rand()) / (float) RAND_MAX;
    	float beta=((float) rand()) / (float) RAND_MAX;

        for (int i = 0; i < M; i++) {
                sw_results[i]= beta*Y[i];
                for (int j = 0; j < N; j++){
                        sw_results[i]+=alpha*X[j]*A[i*N+j];
                }
        }

    	OOPS_gemv(TransA,M, N,alpha,A,lda, X, incX,beta, Y, incY);

        //verify
        int match = 0;
        for (int i = 0; i < M; i++) {
                if (abs(Y[i]- sw_results[i])/sw_results[i]>0.001) {
                        std::cout << "Error: Result mismatch" << std::endl;
                match = 1;
                //break;
                std::cout << "i = " << i << " CPU result = " << sw_results[i] << " Device result = " << Y[i] << std::endl;
                }
                //std::cout << "i = " << i << " CPU result = " << sw_results[i] << " Device result = " << Y[i] << std::endl;

        }

        std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    	free(X);
    	free(Y);
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
