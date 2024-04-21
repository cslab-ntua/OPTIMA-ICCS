#include "test_functions_set.h"
// #include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

//----------------------------------------------------------------------------------------
#include <pthread.h>
#include <signal.h>


void OOPS_gbmv(const char TransA, const int M, const int N, const int KL, const int KU, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY){

        cl_int err;

        int CUs=10;
        int row_size= M;
        int col_size =N;
        int N_cols = ((KU+KL)/16 + 1)*16;

        size_t input_vector_size_bytes = sizeof(float) *N_cols*(row_size/CUs);
        size_t vector_size_bytes = sizeof(float) * (row_size/CUs)*incY;
        size_t matrix_size_bytes = sizeof(float) *N_cols*(row_size/CUs);


        cl::Kernel krnl;
        OCL_CHECK(err, krnl= cl::Kernel (program_interface.program,"krnl_gbmv", &err));

        // Allocate Buffer in Global Memory
        OCL_CHECK(err, cl::Buffer Sx1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) X, &err));
        OCL_CHECK(err, cl::Buffer Sx2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(2*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(3*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(4*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(5*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(6*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(7*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(8*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(9*M/CUs)*N_cols], &err));

        OCL_CHECK(err, cl::Buffer Sy1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) Y, &err));
        OCL_CHECK(err, cl::Buffer Sy2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[2*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[3*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[4*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[5*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[6*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[7*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[8*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[9*M/CUs], &err));
        
	OCL_CHECK(err, cl::Buffer Sa1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(2*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(3*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(4*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(5*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(6*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(7*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(8*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(9*M/CUs)*N_cols], &err));


        // Set the Kernel Arguments
        int narg = 0;
        OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
        OCL_CHECK(err, err = krnl.setArg(narg++, M));
        OCL_CHECK(err, err = krnl.setArg(narg++, N));
        OCL_CHECK(err, err = krnl.setArg(narg++, KL));
        OCL_CHECK(err, err = krnl.setArg(narg++, KU));
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

        program_interface.q.finish();
        clReleaseKernel(krnl.get());

}
