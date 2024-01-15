#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include <omp.h>
#include <ctime>
#include <chrono>

using namespace std::chrono;

void main_tbsv(){
    int N=11264, incX=1, K=28;

    int lda = N;
    //enum layout {ColMajor=N, RowMajor=M};
    //layout layout_item;
    char Uplo = 'L';
    char TransA = 'N';
    char Diag='N';

    float *X= (float *)OOPS_malloc(sizeof(float)*N*incX);
    float *A= (float *)OOPS_malloc(sizeof(float)*N*(K+1));

    vector_N(X,N,incX);

    int N_cols = (K/16 +1)*16;
    float *Apadded = (float *)OOPS_malloc(sizeof(float)*N*N_cols);
    memset(A, 0, sizeof(float)*N*(K+1));
    memset(Apadded, 0, (sizeof(float)*N*N_cols));
    trband_matrix(Uplo,A,Apadded,N,K);

    //verify
    float * sw_results= (float *)OOPS_malloc(sizeof(float)*N*incX);
    memcpy(sw_results, X, sizeof(float)*N);

    //verify
    if (Uplo == 'L' or Uplo == 'l'){
        for (int i=0; i<N; i++) {
            float a = sw_results[i];
            for (int j=1; j<K+1; j++){
                sw_results[i+j] -= a*Apadded[i*N_cols+j];
            }
        }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }

    //call kernel
    OOPS_tbsv(Uplo, TransA, Diag, N, K, Apadded, lda, X, incX);

    //verify
    int match = 0;
    for (int i = 0; i < N; i++) {
        if (abs(X[i]- sw_results[i])/sw_results[i]>0.001) {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << sw_results[i]<< " Device result = " << X[i] << std::endl;
            match = 1;
        }
    }
    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
    free(X);
    free(A);
    free(sw_results);

}
