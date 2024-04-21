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
        int CUs=8;

        std::vector<cl::Kernel> krnls(CUs);
        std::string krnl_name = "krnl_gemv";
        for (int i = 0; i < CUs; i++) {
                std::string cu_id = std::to_string(i + 1);
                std::string krnl_name_full = krnl_name + ":{" + krnl_name +"_" + cu_id + "}";
                printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
                OCL_CHECK(err, krnls[i] = cl::Kernel(program_interface.program, krnl_name_full.c_str(), &err));
        }

        std::vector<cl::Buffer> Sx(CUs);
        std::vector<cl::Buffer> Sy(CUs);
        std::vector<cl::Buffer> Sa(CUs);

        for (int i = 0; i < CUs; i++){
                    OCL_CHECK(err, Sx[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(float)*N*incX, (void *) X, &err));
                    OCL_CHECK(err, Sy[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(float)*(M/CUs)*incY, (void *) &Y[i*M/CUs], &err));
                    OCL_CHECK(err, Sa[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(float)*N*(M/CUs), (void *) &A[(i*M/CUs)*N], &err));
        }

        for (int i = 0; i <CUs; i++){

                int narg = 0;
                OCL_CHECK(err, err = krnls[i].setArg(narg++, TransA));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, M/CUs));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, N));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, alpha));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, Sa[i]));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, lda));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, Sx[i]));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, incX));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, beta));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, Sy[i]));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, incY));

                OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa[i],Sx[i],Sy[i]}, 0 /* 0 means from host */));

	}

	program_interface.q.finish();

        // Launch the Kernel
        for(int i=0; i<CUs; i++){
        	OCL_CHECK(err, err = program_interface.q.enqueueTask(krnls[i]));
        }
        program_interface.q.finish();

        // Copy Result from Device Global Memory to Host Local Memory
        for(int i=0; i<CUs; i++){
        	OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
        }
        program_interface.q.finish();
        for(int i=0; i<CUs; i++){
                clReleaseKernel(krnls[i].get());
        }
}
