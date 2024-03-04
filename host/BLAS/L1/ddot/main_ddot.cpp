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


	int incX=1, incY=1;
	int N = 2048;
	int NCU = 2;
	int MAX_CUS = 10;
    float *X;
    float *Y;

    float sw_sum_of_mults = 0.0;
    float alpha = ((float) rand())/ (float) RAND_MAX;
    float hw_sum_of_mults;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);
    Y=(float*)OOPS_malloc(sizeof(float)*N*incY);

    vector_N(X,N,incX);
    vector_N(Y,N,incY);

    //verify
	for(int i=0;i<N;i++){

		sw_sum_of_mults += X[i] * Y[i];
	}
	sw_sum_of_mults += alpha;
   	hw_sum_of_mults = OOPS_ddot( N, NCU, MAX_CUS, alpha, X, incX, Y, incY);
	
	//verify
	int match = 0;

	if ( (abs(sw_sum_of_mults-hw_sum_of_mults)>0.01)) {
		std::cout << "Error: Result mismatch" << std::endl;
		std::cout << "sw_sum_of_mults : " << sw_sum_of_mults << "  hw_sum_of_mults: " << hw_sum_of_mults << "diff: " << (abs(sw_sum_of_mults-hw_sum_of_mults)) << std::endl;
		match = 1;

	}
   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	
   	free(X);
   	free(Y);

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