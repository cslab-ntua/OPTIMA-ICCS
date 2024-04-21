#include "test_functions_set.h"
// #include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

//----------------------------------------------------------------------------------------
#include <pthread.h>
#include <omp.h>


void OOPS_sbmv(const char Uplo, const int N, const int K, const float alpha, const float  *A, const int lda, float  *X, const int incX,
                const float beta, float *Y, const int incY){

		cl_int err;
    
	    int N_cols = (K/16 + 1)*16;
	    int CUs = 10;
    
	    float *Y1_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);
	    float *Y2_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y3_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y4_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y5_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y6_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y7_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y8_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y9_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);    
	    float *Y10_sym = (float *)OOPS_malloc(sizeof(float)*N*incY);

		memset(Y1_sym, 0, (sizeof(float)*N*incY));
		memset(Y2_sym, 0, (sizeof(float)*N*incY));
		memset(Y3_sym, 0, (sizeof(float)*N*incY));
		memset(Y4_sym, 0, (sizeof(float)*N*incY));
		memset(Y5_sym, 0, (sizeof(float)*N*incY));
		memset(Y6_sym, 0, (sizeof(float)*N*incY));    
		memset(Y7_sym, 0, (sizeof(float)*N*incY));
		memset(Y8_sym, 0, (sizeof(float)*N*incY));
		memset(Y9_sym, 0, (sizeof(float)*N*incY));
		memset(Y10_sym, 0, (sizeof(float)*N*incY));
    
	    /*************************************************************/

		float *Xpad= (float *)OOPS_malloc(sizeof(float)*(N+K)*incX);
	    float *Xpad2= (float *)OOPS_malloc(sizeof(float)*N*N_cols);

		memset(Xpad, 0, (sizeof(float)*(N+K)*incX));    
	    memset(Xpad2, 0, (sizeof(float)*N*N_cols));

	    for(int i=0;i<N;i++){            
		    Xpad[i]=X[i];
	    }
        
	   #pragma omp parallel for num_threads(2)
    	    for(int i=0;i<N;i++){
		    #pragma omp simd
    		    for(int j=0;j<K+1;j++){
			    Xpad2[i*N_cols+j]=Xpad[i+j];
		    }
	    }
            
	    /*************************************************************/

	    
		size_t pad_vector_size_bytes = sizeof(float) *N_cols*N/CUs;
    
	    size_t vector_size_bytes = sizeof(float) * N/CUs*incY;
        
	    size_t full_vector_size = sizeof(float) * N*incY;
        
	    size_t matrix_size_bytes = sizeof(float) *N_cols*N/CUs;

            cl::Kernel krnl;
            
	    OCL_CHECK(err, krnl= cl::Kernel (program_interface.program,"krnl_sbmv", &err));

            // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) Xpad2, &err));
	    OCL_CHECK(err, cl::Buffer Sx2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(1*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(2*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(3*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(4*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(5*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(6*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(7*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sx9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(8*N/CUs)*N_cols], &err));        
	    OCL_CHECK(err, cl::Buffer Sx10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, pad_vector_size_bytes, (void *) &Xpad2[(9*N/CUs)*N_cols], &err));

	    OCL_CHECK(err, cl::Buffer Sa1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) A, &err));
	    OCL_CHECK(err, cl::Buffer Sa2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(1*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(2*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(3*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(4*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(5*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(6*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(7*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(8*N/CUs)*N_cols], &err));
	    OCL_CHECK(err, cl::Buffer Sa10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes, (void *) &A[(9*N/CUs)*N_cols], &err));

	    OCL_CHECK(err, cl::Buffer Sy1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) Y, &err));
		OCL_CHECK(err, cl::Buffer Sy2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[1*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[2*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[3*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[4*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[5*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[6*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[7*N/CUs], &err));
	    OCL_CHECK(err, cl::Buffer Sy9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[8*N/CUs], &err));        
	    OCL_CHECK(err, cl::Buffer Sy10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[9*N/CUs], &err));

	    OCL_CHECK(err, cl::Buffer Sy1_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y1_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy2_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y2_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy3_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y3_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy4_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y4_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy5_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y5_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy6_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y6_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy7_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y7_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy8_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y8_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy9_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y9_sym, &err));
	    OCL_CHECK(err, cl::Buffer Sy10_sym(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, full_vector_size, (void *) Y10_sym, &err));

            // Set the Kernel Arguments
            int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, Uplo));
            OCL_CHECK(err, err = krnl.setArg(narg++, N));
            OCL_CHECK(err, err = krnl.setArg(narg++, K));
            OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa1));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa2));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa3));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa4));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa5));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa6));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sa7));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa8));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa9));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa10));
            OCL_CHECK(err, err = krnl.setArg(narg++, lda));
    	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx1));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx2));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx3));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx4));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx5));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx6));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx7));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx8));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx9));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx10));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
            OCL_CHECK(err, err = krnl.setArg(narg++, beta));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy1));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy2));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy3));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy4));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy5));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy6));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy7));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy8));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy9));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy10));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy1_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy2_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy3_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy4_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy5_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy6_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy7_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy8_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy9_sym));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy10_sym));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa1}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa2}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa3}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa4}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa5}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa6}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa7}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa8}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa9}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa10}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx1}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx2}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx3}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx4}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx5}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx6}, 0 /* 0 means from host*/));
    	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx7}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx8}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx9}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx10}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy1}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy2}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy3}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy4}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy5}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy6}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy7}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy8}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy9}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy10}, 0 /* 0 means from host*/));
            program_interface.q.finish();


	    // Launch the Kernel
	    OCL_CHECK(err, err = program_interface.q.enqueueTask(krnl));
	    program_interface.q.finish();



	    // Copy Result from Device Global Memory to Host Local Memory
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy1}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy2}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy3}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy4}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy5}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy6}, CL_MIGRATE_MEM_OBJECT_HOST));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy7}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy8}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy9}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy10}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy1_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy2_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy3_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy4_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy5_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy6_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy7_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy8_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy9_sym}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy10_sym}, CL_MIGRATE_MEM_OBJECT_HOST));

    	    program_interface.q.finish();
    
	    clReleaseKernel(krnl.get());
        
	    #pragma omp parallel for num_threads(8)
		for(int i=0;i<N;i++){
		    	// #pragma omp simd
				Y[i] += Y1_sym[i] + Y2_sym[i] + Y3_sym[i] + Y4_sym[i] + Y5_sym[i] + Y6_sym[i] + Y7_sym[i] + Y8_sym[i] + Y9_sym[i] + Y10_sym[i];
	    }

	    free(Y1_sym);
	    free(Y2_sym);
	    free(Y3_sym);
	    free(Y4_sym);
	    free(Y5_sym);
	    free(Y6_sym);
	    free(Y7_sym);
    	    free(Y8_sym);
	    free(Y9_sym);
	    free(Y10_sym);
	    free(Xpad);
	    free(Xpad2);
}
