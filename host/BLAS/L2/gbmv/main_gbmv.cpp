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

        int N=1280, M=1280, incX=1, incY=1;

        int KU=25, KL=26;

        int lda = KU+KL+1;
        char TransA = 'N'; // ='C', = 'T' :options
        float *X= (float *)OOPS_malloc(sizeof(float)*N*incX);
        float *Y= (float *)OOPS_malloc(sizeof(float)*M*incY);
        float *A= (float *)OOPS_malloc(sizeof(float)*M*(KU+KL+1));

        vector_N(X,N,incX);
        vector_N(Y,M,incY);
        float alpha= ((float) rand()) / (float) RAND_MAX;
        float beta= ((float) rand()) / (float) RAND_MAX;

        int N_cols = ((KU+KL)/16 + 1)*16;
        float *Apadded= (float *)OOPS_malloc(sizeof(float)*M*N_cols);
        float *Xpadded= (float *)OOPS_malloc(sizeof(float)*(N+KL+KU)*incX);
        float *Xpadded2= (float *)OOPS_malloc(sizeof(float)*M*N_cols);


        memset(Apadded, 0, (sizeof(float)*M*N_cols));
        memset(Xpadded, 0, (sizeof(float)*(N+KL+KU)*incX));

        for(int i=0;i<N;i++){
                Xpadded[i+KL]=X[i];
        }

        #pragma omp parallel for num_threads(2)
        for(int i=0;i<M;i++){
                #pragma omp simd
                for(int j=0;j<KU+KL+1;j++){
                        Xpadded2[i*N_cols+j]=Xpadded[i+j];
                }
        }

        banded_MxN_matrix(A,Apadded,N,M,KU,KL);

	//verify
        float *sw_results= (float *)malloc(sizeof(float)*M*incY);

        if(TransA=='N'){
                for(int i=0;i<M;i++){
                        sw_results[i]=beta*Y[i];
                        for (int j=std::max(0,i-KL);j<std::min(N,i+KU+1);j++){
                                sw_results[i]+=X[j]*A[KL-i+j+i*(KU+KL+1)]*alpha;
                        }
                }
        }

        OOPS_gbmv(TransA, M,  N, KL, KU, alpha,  Apadded, lda, Xpadded2,  incX, beta,Y, incY);

        //verify
        int match = 0;
        for (int i = 0; i < M; i++) {
                if (abs(Y[i]- sw_results[i])/sw_results[i]>0.001) {
                        std::cout << "Error: Result mismatch" << std::endl;
                        std::cout << "i = " << i << " CPU result = " << sw_results[i]<< " Device result = " << Y[i] << std::endl;
                        match = 1;

                }
        }

        std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

        free(X);
        free(Y);
        free(A);
        free(sw_results);
        free(Apadded);
        free(Xpadded2);
        free(Xpadded);


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

