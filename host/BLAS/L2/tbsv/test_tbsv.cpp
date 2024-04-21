#include "test_functions_set.h"
// #include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

//----------------------------------------------------------------------------------------


void OOPS_tbsv(const char Uplo, const char TransA, const char Diag, const int N,
                const int K, const float  *A, const int lda, float  *X, const int incX){

        cl_int err;
        cl::Kernel krnl;

        int N_cols = (K/16 + 1)*16;

        size_t vector_size_bytes = sizeof(float)*N*incX;
        size_t matrix_size_bytes = sizeof(float)*N_cols*N;

        OCL_CHECK(err, krnl= cl::Kernel (program_interface.program,"krnl_tbsv", &err));

        // Allocate Buffer in Global Memory
        OCL_CHECK(err, cl::Buffer Sx_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, (void *) X, &err));
        OCL_CHECK(err, cl::Buffer Sx_out(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) X, &err));

        OCL_CHECK(err, cl::Buffer Sa1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa12(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa13(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa14(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa15(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa16(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));

        // Set the Kernel Arguments
        int narg = 0;
        OCL_CHECK(err, err = krnl.setArg(narg++, Uplo));
        OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
        OCL_CHECK(err, err = krnl.setArg(narg++, Diag));
        OCL_CHECK(err, err = krnl.setArg(narg++, N));
        OCL_CHECK(err, err = krnl.setArg(narg++, K));
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
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa11));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa12));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa13));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa14));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa15));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa16));
        OCL_CHECK(err, err = krnl.setArg(narg++, lda));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in));
        OCL_CHECK(err, err = krnl.setArg(narg++, incX));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out));


        // Copy input data to device global memory
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa1}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa2}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa3}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa4}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa5}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa6}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa7}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa8}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa9}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa10}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa11}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa12}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa13}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa14}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa15}, 0));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa16}, 0));

        program_interface.q.finish();

	
        // Launch the Kernel
	OCL_CHECK(err, err = program_interface.q.enqueueTask(krnl));
	program_interface.q.finish();
	

        // Copy Result from Device Global Memory to Host Local Memory
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out}, CL_MIGRATE_MEM_OBJECT_HOST));

        program_interface.q.finish();
        clReleaseKernel(krnl.get());

}
