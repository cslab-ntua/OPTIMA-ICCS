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
	int N=2048;
	int NCU = 2;
	int MAX_CUS = 16;
	float* X;
	
	double sum_sw = 0.0;
	double sum_hw = 0.0;
	
	X=(float*)OOPS_malloc(sizeof(float)*N*incX);
	std_vector_N(X, N, incX, 1.0);
	
	//verify
	for(int i=0;i<N;i++){
		sum_sw += X[i];
	}
	
	sum_hw = OOPS_asum( N, NCU, MAX_CUS, X, incX);
	
	//verify
	int match = 0;
	if ( (abs(sum_hw-sum_sw)>0.10) ) {
		std::cout << "Error: Result mismatch" << std::endl;
		match = 1;
	}
	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	
	free(X);

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
