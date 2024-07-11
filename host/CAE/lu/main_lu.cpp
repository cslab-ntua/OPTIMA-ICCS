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

	int N=256;
	int incX=1;
    float* A, *L, *L_sw, *U, *U_sw;
    
    int matrixSize = N*N;

    A= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (A == NULL) {
    	std::cout << "LU: A = NULL abort.." << std::endl;
    	return false;
    }
    L= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (L == NULL) {
		std::cout << "LU: L = NULL abort.." << std::endl;
		return false;
	}
    else {
    	memset(&L[0],0,matrixSize*sizeof(float));
    }
	L_sw = (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
	if (L_sw == NULL) {
		std::cout << "LU: L_sw = NULL abort.." << std::endl;
		return false;
	}
    U= (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (U == NULL) {
		std::cout << "LU: U = NULL abort.." << std::endl;
		return false;
	}
    else {
		memset(&U[0],0,matrixSize*sizeof(float));
	}
    U_sw = (float *)OOPS_malloc(sizeof(float)*matrixSize*incX);
    if (U_sw == NULL) {
		std::cout << "LU: U_sw = NULL abort.." << std::endl;
		return false;
	}


	vector_N(A,matrixSize,incX);





    memcpy(&U_sw[0],&A[0],sizeof(float)*matrixSize*incX);
    for (int i=0;i<N;i++) {
		for (int j=0;j<N;j++) {
			L_sw[i*N+j] = (i==j) ? 1.0f : 0.0f;
		}
	}
    memcpy(&U[0],&A[0],sizeof(float)*matrixSize*incX);
	memcpy(&L[0],&L_sw[0],sizeof(float)*matrixSize*incX);


	std::cout << "LU: software started (N = " << N << ").." << endl;


    float pivot = 0.0f, x = 0.0f;
    int pivotRow = 0;

    int ops = 0;
    for (int c=0;c<N-1;c++) {
    	pivotRow = c;
    	pivot = U_sw[c*N+c];
		for (int r=c+1;r<N;r++) {
			x = U_sw[r*N+c] / pivot;
			L_sw[r*N+c] = x;
			ops++;
			for (int c1=c;c1<N;c1++) {
				U_sw[r*N+c1] = U_sw[r*N+c1] - x*U_sw[pivotRow*N+c1];
				ops+=2;
			}
		}
	}



    OOPS_lu(L, U, N); //OOPS_lu3d15df_b



	int match = 1;
	for(int i=0;i<matrixSize;i++){
		if ((abs(L_sw[i]-L[i])>0.1) || (isnan(L[i])) || (!isfinite(L[i]))){
			 std::cout << "Error: Result mismatch in L" << std::endl;
			 std::cout << "i = " << i << " L_sw[" << i << "] = " << L_sw[i] << ", L[" << i << "] = " << L[i] << std::endl;
			 match = 0;
			 break;
		}
	}

	for(int i=0;i<matrixSize;i++){
		if ((abs(U_sw[i]-U[i])>0.1) || (isnan(U[i])) || (!isfinite(U[i]))){
			 std::cout << "Error: Result mismatch in U" << std::endl;
			 std::cout << "i = " << i << " U_sw[" << i << "] = " << U_sw[i] << ", U[" << i << "] = " << U[i] << std::endl;
			 match = 0;
			 break;
		}
	}

	std::cout << "LU: TEST " << (match ? "PASSED" : "FAILED") << std::endl;



    free(A);
    free(L);
    free(L_sw);
    free(U);
    free(U_sw);
  	
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

