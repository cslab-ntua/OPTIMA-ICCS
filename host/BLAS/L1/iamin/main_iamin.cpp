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
	int NCU = 2; // 1, 2, 4, 8, 12, 16
	int MAX_CUS = 16;
    float* X;

    int min_sw_idx = 0;
    int min_hw_idx = 0;

    double elapsed_krnl_time;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);

    pls_std_vector_N(X,N,incX);
	
	//verify
    float min_sw_val = X[0];
	for(int i=0;i<N;i++){
		if(min_sw_val>X[i])
		{
			min_sw_val = X[i];
			min_sw_idx = i;
		}
	}

    min_hw_idx = OOPS_iamin( N, NCU, MAX_CUS, X, incX);

	//verify
	int match = 0;
	if ( min_sw_idx!=min_hw_idx) {
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


