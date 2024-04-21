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
	
	int incX=1;
	int N = 2048;
	int NCU = 2; // 1 2 4 8 16 32
	int MAX_CUS = 32;
    float* X;
    float* X_sw;
    X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    X_sw= (float *)OOPS_malloc(sizeof(float)*N*incX);
    vector_N(X,N,incX);
    float alpha=((float) rand()) / (float) RAND_MAX;
	
	//verify
	for(int i=0;i<N;i++){
		X_sw[i] = alpha*X[i];
	}
 	OOPS_scal( N, NCU, MAX_CUS, alpha, X, incX);
	
	//verify
	int match = 0;
	for (int i = 0; i < N; i++) {
		if (abs(X_sw[i]-X[i])>0.001) {
			std::cout << "Error: Result mismatch" << std::endl;
			std::cout << "i = " << i << " CPU result = " << X_sw[i]
						  << " Device result = " << X[i] << std::endl;
			match = 1;
			break;
		}
	}

	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	free(X);
	free(X_sw);

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