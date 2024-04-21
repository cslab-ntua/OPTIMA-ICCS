#include "test_functions_set.h"
// #include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

//----------------------------------------------------------------------------------------


void OOPS_tbmv( const char Uplo, const char TransA, const char Diag,
                 const int N, const int K, const float  *A, const int lda, float  *X, const int incX){

        cl_int err;

        int N_cols = (K/16 + 1)*16;
        int CUs = 14;

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
        size_t vector_size_bytes = sizeof(float) * N/CUs*incX;
        size_t matrix_size_bytes = sizeof(float) *N_cols*N/CUs;

        cl::Kernel krnl;
        OCL_CHECK(err, krnl= cl::Kernel (program_interface.program,"krnl_tbmv", &err));
	
	// Allocate Buffer in Global Memory
        OCL_CHECK(err, cl::Buffer Sx_in1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) Xpad2, &err));
        OCL_CHECK(err, cl::Buffer Sx_in2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(2*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(3*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(4*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(5*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(6*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(7*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(8*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(9*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(10*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in12(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(11*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in13(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(12*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx_in14(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, pad_vector_size_bytes, (void *) &Xpad2[(13*N/CUs)*N_cols], &err));
        
	OCL_CHECK(err, cl::Buffer Sa1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(2*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(3*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(4*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(5*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(6*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(7*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(8*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(9*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(10*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa12(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(11*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa13(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(12*N/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa14(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(13*N/CUs)*N_cols], &err));
	 
 	OCL_CHECK(err, cl::Buffer Sx_out1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) X, &err));
        OCL_CHECK(err, cl::Buffer Sx_out2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[2*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[3*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[4*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[5*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[6*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[7*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out9(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[8*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out10(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[9*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[10*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out12(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[11*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out13(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[12*N/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sx_out14(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) &X[13*N/CUs], &err));
        
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
        OCL_CHECK(err, err = krnl.setArg(narg++, lda));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in1));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in2));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in3));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in4));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in5));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in6));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in7));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in8));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in9));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in10));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in11));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in12));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in13));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in14));
        OCL_CHECK(err, err = krnl.setArg(narg++, incX));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out1));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out2));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out3));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out4));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out5));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out6));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out7));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out8));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out9));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out10));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out11));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out12));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out13));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out14));

        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in1}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in2}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in3}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in4}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in5}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in6}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in7}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in8}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in9}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in10}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in11}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in12}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in13}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_in14}, 0 /* 0 means from host*/));
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
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa11}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa12}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa13}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa14}, 0 /* 0 means from host*/));

        program_interface.q.finish();

	// Launch the Kernel
	OCL_CHECK(err, err = program_interface.q.enqueueTask(krnl));
	program_interface.q.finish();


	
        // Copy Result from Device Global Memory to Host Local Memory
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out1}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out2}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out3}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out4}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out5}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out6}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out7}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out8}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out9}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out10}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out11}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out12}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out13}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx_out14}, CL_MIGRATE_MEM_OBJECT_HOST));

        program_interface.q.finish();
        clReleaseKernel(krnl.get());
        free(Xpad);
        free(Xpad2);
}
