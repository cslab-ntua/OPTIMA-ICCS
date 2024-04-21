#include "test_functions_set.h"
// #include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

//----------------------------------------------------------------------------------------


void OOPS_gemv(const char TransA, const int M, const int N, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY){

            cl_int err;
            int row_size= M;
            int col_size =N;

            int CUs=14;

            if(TransA=='C'|| TransA=='T'){
                    row_size =N;
                    col_size =M;
            }

            size_t input_vector_size_bytes = sizeof(float) * col_size*incX;
            size_t output_vector_size_bytes = sizeof(float) * (row_size/CUs)*incY;
        size_t matrix_size_bytes = sizeof(float) *N*(M/CUs);

            cl::Kernel krnl;

            OCL_CHECK(err, krnl= cl::Kernel (program_interface.program,"krnl_gemv", &err));

	                // Allocate Buffer in Global Memory
            OCL_CHECK(err, cl::Buffer Sx(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) X, &err));

            OCL_CHECK(err, cl::Buffer Sy_1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) Y, &err));
            OCL_CHECK(err, cl::Buffer Sy_2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[2*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[3*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[4*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[5*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[6*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[7*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[8*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[9*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[10*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_12(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[11*M/CUs], &err));
                OCL_CHECK(err, cl::Buffer Sy_13(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[12*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_14(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[13*M/CUs], &err));

            OCL_CHECK(err, cl::Buffer Sa_1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
            OCL_CHECK(err, cl::Buffer Sa_2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(2*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(3*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(4*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(5*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(6*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(7*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(8*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(9*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(10*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_12(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(11*M/CUs)*N], &err));
                OCL_CHECK(err, cl::Buffer Sa_13(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(12*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_14(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(13*M/CUs)*N], &err));

	    // Set the Kernel Arguments
            int narg = 0;
            OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
            OCL_CHECK(err, err = krnl.setArg(narg++, M));
            OCL_CHECK(err, err = krnl.setArg(narg++, N));
            OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_1));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_2));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_3));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_4));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_5));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_6));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_7));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_8));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_9));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_10));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_11));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_12));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_13));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_14));
            OCL_CHECK(err, err = krnl.setArg(narg++, lda));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
            OCL_CHECK(err, err = krnl.setArg(narg++, incX));
            OCL_CHECK(err, err = krnl.setArg(narg++, beta));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_1));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_2));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_3));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_4));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_5));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_6));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_7));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_8));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_9));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_10));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_11));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_12));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_13));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_14));
            OCL_CHECK(err, err = krnl.setArg(narg++, incY));

	    // Copy input data to device global memory
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_1}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_2}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_3}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_4}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_5}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_6}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_7}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_8}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_9}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_10}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_11}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_12}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_13}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa_14}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_1}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_2}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_3}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_4}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_5}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_6}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_7}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_8}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_9}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_10}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_11}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_12}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_13}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_14}, 0 /* 0 means from host*/));

	        program_interface.q.finish();

            // // Launch the Kernel
            OCL_CHECK(err, err = program_interface.q.enqueueTask(krnl));
            program_interface.q.finish();
            
            // Copy Result from Device Global Memory to Host Local Memory
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_1}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_2}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_3}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_4}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_5}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_6}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_7}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_8}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_9}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_10}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_11}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_12}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_13}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy_14}, CL_MIGRATE_MEM_OBJECT_HOST));

            program_interface.q.finish();
            clReleaseKernel(krnl.get());

}
