#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include <omp.h>
#include <ctime>
#include <chrono>

using namespace std::chrono;

void main_trmv(){

    int N=11264, incX=1;
    int lda=N;
    char Uplo='U'; // ='U',='u', or ='L','l' for lower
    char TransA='N'; // ='C', = 'T' :options
    char Diag='N'; // ='n', or ='U','u', for unit triangular matrix
    
    float *X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    float *A= (float *)OOPS_malloc(sizeof(float)*N*N);
    float *sw_results= (float *)OOPS_malloc(sizeof(float)*N*incX);

    vector_N(X,N,incX);
    triangular_NxN_matrix( Uplo,A, N);

    //verify
    if (Uplo == 'U' or Uplo == 'u'){
        for (int i = 0; i < N; i++) {
                sw_results[i]=0;
                for (int j = i ;j < N; j++){
                        sw_results[i]+=X[j]*A[i*N+j];
            }
        }
    }
    else if (Uplo == 'L' or Uplo == 'l'){
        for (int i = 0; i < N; i++) {
                sw_results[i]=0;
            for (int j = 0 ;j < i+1; j++){
                        sw_results[i]+=X[j]*A[i*N+j];
            }
        }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }

    //call kernel
    OOPS_trmv(Uplo,TransA,Diag, N,A,lda, X, incX);

    //verify
        int match = 0;
        for (int i = 0; i < N; i++) {
                if (abs(X[i]- sw_results[i])/sw_results[i]>0.001) {
                        std::cout << "Error: Result mismatch" << std::endl;
                match = 1;
            }
                //std::cout << "i = " << i << " CPU result = " << sw_results[i]<< " Device result = " << X[i] << std::endl;
            }

            std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(X);
    free(A);
    free(sw_results);

}
