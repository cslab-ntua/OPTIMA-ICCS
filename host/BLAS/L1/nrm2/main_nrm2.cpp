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
	int NCU = 2;
	int MAX_CUS = 10;
    float *X;

    float sw_sum_of_mults = 0.0;
    float sw_final_res = 0.0;
    float hw_final_res;

    X=(float*)OOPS_malloc(sizeof(float)*N*incX);


    vector_N(X,N,incX);

//verify
    for(int i=0;i<N;i++){

		sw_sum_of_mults += X[i] * X[i];
	}
	sw_final_res += sqrtf(sw_sum_of_mults);
   
    hw_final_res = OOPS_nrm2( N, NCU, MAX_CUS, X, incX);
	
	//verify
	int match = 0;

	if ( (abs(sw_final_res-hw_final_res)>0.01)) {
		std::cout << "Error: Result mismatch" << std::endl;
		std::cout << "sw_final_res : " << sw_final_res << "  hw_final_res: " << hw_final_res << "diff: " << (abs(sw_final_res-hw_final_res)) << std::endl;
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
