#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <fstream> // to use i/ofstream

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

	
	int incX=1, incY=1;
	int N=2048;
	int NCU = 2; // 1, 2, 4, 8, 12, 16
	int MAX_CUS = 16;

    float* X;
    float* Y;
    float* Y_sw;
    X=(float*)OOPS_malloc(sizeof(float)*N*incX);
    Y=(float*)OOPS_malloc(sizeof(float)*N*incY);
    Y_sw=(float*)OOPS_malloc(sizeof(float)*N*incY);

    vector_N(X,N,incX);
	//verify
	for(int i=0;i<N;i++){
		Y_sw[i] = X[i];
	}

    OOPS_copy( N, NCU, MAX_CUS, X, incX, Y, incY);
	
	//verify
	int match = 0;
   	for (int i = 0; i < N; i++) {
   		if (abs(Y_sw[i]-Y[i])>0.001) {
   			std::cout << "Error: Result mismatch" << std::endl;
   	        std::cout << "i = " << i << " CPU result = " << Y_sw[i]
   	                      << " Device result = " << Y[i] << std::endl;
   	        match = 1;
   	        break;
   	    }
   	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

   	free(X);
   	free(Y);
   	free(Y_sw);

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