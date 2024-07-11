

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

    int N=128;
	int incX=1;
    float* A, *M, *M_sw;

    int matrixSize = N*N;

    A= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    M= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    M_sw= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);

    diagonal_N(A,N);



    for (int i=0;i<matrixSize;i++) {
    	M[i] = 0.0f;
    	M_sw[i] = 0.0f;
    }



    for (int i=0;i<matrixSize;i=i+N+1) {
    	A[i] != 0.0f ? M_sw[i] = 1.0f / A[i] : M_sw[i] = 0.0f;
    }
 

     OOPS_jacobi( N, A, M);
 
    int match = 1;
	for(int i=0;i<matrixSize;i++){
		if(abs(M_sw[i]-M[i])>0.1){
			 std::cout << "Error: Result mismatch" << std::endl;
			 std::cout << "i = " << i << " CPU result = " << M_sw[i] << ", device result = " << M[i] << std::endl;
			 match = 0;
			 break;
		}
	}

	std::cout << "JACOBI: TEST " << (match ? "PASSED" : "FAILED") << std::endl;
	if (match == 1){
		std::cout << "JACOBI: N = " << N << " (" << (N*N*sizeof(float))/(1024*1024) << " MB) "  << std::endl;
		}

    free(A);
    free(M);
    free(M_sw);

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