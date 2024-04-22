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

	const int Mnew=2048,Nnew=2048;
    int lda = Mnew;
    int ldb = Mnew;
    char TransA = 'N'; // ='C', = 'T' :options

    float* A= (float *)OOPS_malloc(sizeof(float)*Mnew*Mnew);
    float* B= (float *)OOPS_malloc(sizeof(float)*Mnew*Nnew);
    float* C= (float *)OOPS_malloc(sizeof(float)*Mnew*Nnew);



    MxN_matrix(B,Mnew,Nnew);
    triangular_NxN_matrix('L',A,Nnew);


    float alpha=((float) rand()) / (float) RAND_MAX;

    //verify
	float* B_sw= (float *)OOPS_malloc(sizeof(float)*Nnew*Mnew);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();


	for (int i = 0; i < Mnew; i++) {
		for (int j = i; j < Nnew; j++){
			float sum = B[j*Nnew + i];
			for (int k=0; k<i;k++){
				sum -= A[k*Mnew+i] * B_sw[k*Nnew+j];
			}
			B_sw[i*Nnew+j] = sum;
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";
	std::cout << std::endl;

    OOPS_trsm('L','L',TransA, 'N', Mnew, Nnew, alpha, A, lda, B, ldb, C);

   	//verify
   	int match = 0;
   	for (int i = 0; i < Mnew; i++) {
   		for(int j=i;j<Nnew;j++){
   			if (abs(B_sw[i*Nnew+j]-C[i*Nnew+j])>0.001) {
   				std::cout << "Error: Result mismatch" << std::endl;
   			   	std::cout << "i = " << i*Nnew+j << " CPU result = " << B_sw[i*Nnew+j]
   			   	                      << " Device result = " << C[i*Nnew+j] << std::endl;
   			   	match = 1;
   			   	//break;
   			}
   		}
   	}

   	std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(A);
    free(B);
    free(C);
    free(B_sw);

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

