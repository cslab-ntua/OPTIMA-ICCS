#include "test_functions_set.h"
#include <cmath>

void OOPS_spmv(const char Uplo,const char Diag,const int N, const float  *AP, const uint32_t packedMatrixSize, float alpha, float  *X, float beta, float *Yin,  const int incX){
	 	cl_int err;
	 	int fpgaPackedSize = OOPS_cmpt_hyperpacked_triang_mtrx_blks(N, V_DATA_SIZE) * V_DATA_SIZE;
	 	float *AhyperPacked = (float *)OOPS_malloc(sizeof(float)*fpgaPackedSize);
		memset(AhyperPacked, 0, sizeof(float)*fpgaPackedSize);

		convert_packed_matrix_to_Hyperpacked(Uplo, AP, packedMatrixSize, AhyperPacked, fpgaPackedSize, N, V_DATA_SIZE);

	 	size_t input_vector_size_bytes = sizeof(float) * N*incX;
	 	int total_vds_blks = OOPS_cmpt_hyperpacked_triang_mtrx_blks(N, V_DATA_SIZE);
	 	int row_part = N/8;
	 	//find break row and break_vds_blk_idx1
		uint32_t break_row[8] ;
		uint32_t break_vds_blk_idx[8];
		size_t  matrix_prt_sBytes[8];
		size_t out_vec_prt_sBytes[8];

		break_row[0] = 0;
		break_vds_blk_idx[0] = 0;
		for (int i = 1; i < 8; i++) {
			break_row[i] = break_row[i-1] + row_part;
			break_vds_blk_idx[i] = OOPS_Hpacked_triMtx_start_idx(N, V_DATA_SIZE, break_row[i]);

			matrix_prt_sBytes[i-1] = (break_vds_blk_idx[i] - break_vds_blk_idx[i-1]) * V_DATA_SIZE * sizeof(float);
			out_vec_prt_sBytes[i-1] = sizeof(float) * (break_row[i] - break_row[i-1]) * incX;
		}

		matrix_prt_sBytes[7] = (total_vds_blks - break_vds_blk_idx[7]) * V_DATA_SIZE * sizeof(float);
		out_vec_prt_sBytes[7] = sizeof(float) * (N - break_row[7]) * incX;



	 	cl::Kernel krnl;

		OCL_CHECK(err, krnl= cl::Kernel (program_interface.program,"krnl_spmv", &err));




	    // Allocate Buffer in Global Memory

		OCL_CHECK(err, cl::Buffer Sx1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
					                                           (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													   	   	   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
																	   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
																	   (void *) X, &err));
		/*-----------------------------------------------------------------------------------------------------*/
		OCL_CHECK(err, cl::Buffer Sx11(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
					                                           (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx22(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx33(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx44(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx55(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
					                                           (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx66(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx77(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));
		OCL_CHECK(err, cl::Buffer Sx88(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															   (void *) X, &err));


		/*-----------------------------------------------------------------------------------------------------*/
	 

	    OCL_CHECK(err, cl::Buffer Sa1(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[0],
	    	    	                                             (void *) AhyperPacked, &err));
		OCL_CHECK(err, cl::Buffer Sa2(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[1],
	    														 (void *) &AhyperPacked[break_vds_blk_idx[1] * V_DATA_SIZE], &err));
		OCL_CHECK(err, cl::Buffer Sa3(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[2],
																 (void *) &AhyperPacked[break_vds_blk_idx[2] * V_DATA_SIZE], &err));
		OCL_CHECK(err, cl::Buffer Sa4(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[3],
																 (void *) &AhyperPacked[break_vds_blk_idx[3] * V_DATA_SIZE], &err));
		OCL_CHECK(err, cl::Buffer Sa5(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[4],
																 (void *) &AhyperPacked[break_vds_blk_idx[4] * V_DATA_SIZE], &err));
		OCL_CHECK(err, cl::Buffer Sa6(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[5],
																 (void *) &AhyperPacked[break_vds_blk_idx[5] * V_DATA_SIZE], &err));
		OCL_CHECK(err, cl::Buffer Sa7(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[6],
																 (void *) &AhyperPacked[break_vds_blk_idx[6] * V_DATA_SIZE], &err));
		OCL_CHECK(err, cl::Buffer Sa8(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[7],
																 (void *) &AhyperPacked[break_vds_blk_idx[7] * V_DATA_SIZE], &err));

		/*-----------------------------------------------------------------------------------------------------*/
	   	OCL_CHECK(err, cl::Buffer Sy1_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[0],
	    	                                             (void *) Yin, &err));
		OCL_CHECK(err, cl::Buffer Sy2_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[1],
														 (void *) &Yin[break_row[1]], &err));
		OCL_CHECK(err, cl::Buffer Sy3_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[2],
														 (void *) &Yin[break_row[2]], &err));
		OCL_CHECK(err, cl::Buffer Sy4_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[3],
														 (void *) &Yin[break_row[3]], &err));
		OCL_CHECK(err, cl::Buffer Sy5_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[4],
														 (void *) &Yin[break_row[4]], &err));
		OCL_CHECK(err, cl::Buffer Sy6_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[5],
														 (void *) &Yin[break_row[5]], &err));
		OCL_CHECK(err, cl::Buffer Sy7_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[5],
														 (void *) &Yin[break_row[6]], &err));
		OCL_CHECK(err, cl::Buffer Sy8_in(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, out_vec_prt_sBytes[5],
														 (void *) &Yin[break_row[7]], &err));

		
	    // Set the Kernel Arguments

	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, total_vds_blks));
	    OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
	    OCL_CHECK(err, err = krnl.setArg(narg++, beta));
	    /*-------------------------------------------*/
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sa1));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx1));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx11));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[1]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[1]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa2));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx2));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx22));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[2]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[2]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa3));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx3));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx33));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[3]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[3]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa4));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx4));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx44));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[4]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[4]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa5));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx5));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx55));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[5]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[5]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa6));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx6));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx66));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[6]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[6]));

		OCL_CHECK(err, err = krnl.setArg(narg++, Sa7));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx7));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx77));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[7]));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[7]));

		OCL_CHECK(err, err = krnl.setArg(narg++, Sa8));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx8));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx88));


		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy1_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy2_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy3_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy4_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy5_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy6_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy7_in));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sy8_in));


	    // Copy input data to device global memory
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa1}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx1}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx11}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa2}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx2}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx22}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa3}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx3}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx33}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa4}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx4}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx44}, 0 /* 0 means from host*/));
		/********************************************************************************/
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa5}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx5}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx55}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa6}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx6}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx66}, 0 /* 0 means from host*/));

		/********************************************************************************/
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa7}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx7}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx77}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sa8}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx8}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx88}, 0 /* 0 means from host*/));

	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy1_in}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy2_in}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy3_in}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy4_in}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy5_in}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy6_in}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy7_in}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy8_in}, 0 /* 0 means from host*/));

		OCL_CHECK(err, err = program_interface.q.finish());

	    // Launch the Kernel
		OCL_CHECK(err, err = program_interface.q.enqueueTask(krnl));
	    OCL_CHECK(err, err = program_interface.q.finish());
	    
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy1_in}, CL_MIGRATE_MEM_OBJECT_HOST));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy2_in}, CL_MIGRATE_MEM_OBJECT_HOST));
	    OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy3_in}, CL_MIGRATE_MEM_OBJECT_HOST));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy4_in}, CL_MIGRATE_MEM_OBJECT_HOST));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy5_in}, CL_MIGRATE_MEM_OBJECT_HOST));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy6_in}, CL_MIGRATE_MEM_OBJECT_HOST));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy7_in}, CL_MIGRATE_MEM_OBJECT_HOST));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy8_in}, CL_MIGRATE_MEM_OBJECT_HOST));
	    OCL_CHECK(err, err = program_interface.q.finish());

	    clReleaseKernel(krnl.get());

	    free(AhyperPacked);

}
