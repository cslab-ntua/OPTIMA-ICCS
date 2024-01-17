void OOPS_tpsv_16cus(const char Uplo,const char Diag,const int N, const float  *A, const uint32_t packedMatrixSize, float  *X,  const int incX){
	 	cl_int err;
	 	// Functions expects a transposed matrix for the computation
	 	int total_vds_blks = OOPS_cmpt_hyperpacked_triang_mtrx_blks(N, V_DATA_SIZE);
	 	int fpgaPckMtrxElems = total_vds_blks * V_DATA_SIZE;
		size_t matrix_prt_sBytes = fpgaPckMtrxElems * sizeof(float);

	 	float *AhyperPacked = (float *)OOPS_malloc(matrix_prt_sBytes);
		memset(AhyperPacked, 0, matrix_prt_sBytes);

		convert_packed_matrix_to_Hyperpacked(Uplo, A, packedMatrixSize, AhyperPacked, fpgaPckMtrxElems, N, V_DATA_SIZE);

		size_t input_vector_size_bytes = sizeof(float) * N*incX;
		size_t out_vec_prt_sBytes = sizeof(float) * N * incX;

		int row_part = N/16;
		int break_row[16] ;
		break_row[0] = 0;
		for (int i = 1; i < 16; i++) {
			break_row[i] = break_row[i-1] + row_part;
		}


        cl::Kernel krnl;
		OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_tpsv", &err));

	    // Allocate Buffer in Global Memorυ
		OCL_CHECK(err, cl::Buffer Sb1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
					                                           (void *) X, &err));



		/*-----------------------------------------------------------------------------------------------------*/
	 

	    OCL_CHECK(err, cl::Buffer Sa1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	                                             (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	    	                                             (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	                                             	 	 (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	    	                                             (void *) AhyperPacked, &err));
	    OCL_CHECK(err, cl::Buffer Sa5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	    	                                             (void *) AhyperPacked, &err));

		OCL_CHECK(err, cl::Buffer Sa6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
																	 	 	 (void *) AhyperPacked, &err));

		OCL_CHECK(err, cl::Buffer Sa7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
																			(void *) AhyperPacked, &err));

		OCL_CHECK(err, cl::Buffer Sa8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
																		 	 (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	                                             (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	    	                                             (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa11(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	                                             	 	 (void *) AhyperPacked, &err));

	    OCL_CHECK(err, cl::Buffer Sa12(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	    	                                             (void *) AhyperPacked, &err));
	    OCL_CHECK(err, cl::Buffer Sa13(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
	    	    	    	                                             (void *) AhyperPacked, &err));

		OCL_CHECK(err, cl::Buffer Sa14(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
																	 	 	 (void *) AhyperPacked, &err));

		OCL_CHECK(err, cl::Buffer Sa15(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
																			(void *) AhyperPacked, &err));

		OCL_CHECK(err, cl::Buffer Sa16(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes,
																		 	 (void *) AhyperPacked, &err));


		/*-----------------------------------------------------------------------------------------------------*/
	    
		OCL_CHECK(err, cl::Buffer Sx_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes,
	    	                                             (void *) X, &err));

	    // Set the Kernel Arguments

	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));

	    /*-------------------------------------------*/
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sa1));
	    OCL_CHECK(err, err = krnl.setArg(narg++, break_row[1]));
	    /*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa2));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[2]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa3));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[3]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa4));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[4]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa5));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[5]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa6));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[6]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa7));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[7]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa8));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[8]));
	    /*-------------------------------------------*/
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sa9));
	    OCL_CHECK(err, err = krnl.setArg(narg++, break_row[9]));
	    /*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa10));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[10]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa11));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[11]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa12));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[12]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa13));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[13]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa14));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[14]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa15));
		OCL_CHECK(err, err = krnl.setArg(narg++, break_row[15]));
		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sa16));


		/*-------------------------------------------*/
		OCL_CHECK(err, err = krnl.setArg(narg++, Sb1));
		OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out));


	    // Copy input data to device global memory
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sb1}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa1}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa2}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa3}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa4}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa5}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa6}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa7}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa8}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa9}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa10}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa11}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa12}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa13}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa14}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa15}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa16}, 0 /* 0 means from host*/));

		OCL_CHECK(err, err = q.finish());


	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    OCL_CHECK(err, err = q.finish());

	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx_out}, CL_MIGRATE_MEM_OBJECT_HOST));

	    OCL_CHECK(err, err = q.finish());

	    clReleaseKernel(krnl.get());
	    free(AhyperPacked);

}