#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include <omp.h>
#include <ctime>
#include <chrono>

using namespace std::chrono;

void main_sbmv(){

    char Uplo = 'U'; // ='U',='u', or ='L','l' for lower
    int N=10240, K=26, incX=1, incY=1;
    int lda = K+1;


    float *X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    float *Y= (float *)OOPS_malloc(sizeof(float)*N*incY);
    float *A= (float *)OOPS_malloc(sizeof(float)*N*(K+1));

    vector_N(X, N, incX);
    vector_N(Y, N, incY);
    float alpha = ((float) rand()) / (float) RAND_MAX;
    float beta = ((float) rand()) / (float) RAND_MAX;

    int N_cols = (K/16 + 1)*16;
    float *Apadded= (float *)OOPS_malloc(sizeof(float)*N*N_cols);
    memset(Apadded, 0, (sizeof(float)*N*N_cols));

    trband_matrix(Uplo,A,Apadded,N,K);


    //verify
    float *sw_results= (float *)malloc(sizeof(float)*N*incX);


    for(int i=0; i<N; i++){
        sw_results[i] = beta*Y[i];
        for(int j=0; j<std::min(N-i,K+1); j++){
            sw_results[i] += alpha*X[j+i]*Apadded[i*(N_cols)+j];
        }
    }

    for(int i=0;i<N;i++){
        for(int j=1; j<std::min(N-i,K+1); j++){
            sw_results[i+j] += alpha*X[i]*Apadded[i*(N_cols)+j];
        }
    }

    OOPS_sbmv(Uplo, N, K, alpha, Apadded, lda, X, incX, beta, Y, incY);

    int match = 0;
    for (int i = 0; i < N; i++) {
        if (abs(Y[i]- sw_results[i])/sw_results[i]>0.001) {
            //std::cout << "Error: Result mismatch" << std::endl;
            //std::cout << "i = " << i << " CPU result = " << sw_results[i]<< " Device result = " << Y[i] << std::endl;
            match = 1;
        }
    }

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

    free(X);
    free(A);
    free(Y);
    free(sw_results);
    free(Apadded);

}
