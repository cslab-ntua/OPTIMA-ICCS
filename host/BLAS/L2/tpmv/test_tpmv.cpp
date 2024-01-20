void OOPS_tpmv(const char Uplo,const char Diag,const int N, const float  *AP, const uint32_t packedMatrixSize, float  *X, const int incX, double *krnlTime_arg ){
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

	/* With Load Balance					*/
	uint32_t av = (N-1)/V_DATA_SIZE+1;
	uint32_t v = av;
	uint32_t a1 = 1;
	uint32_t Sv = v*(a1 + av)/2; // arithmetic progression
	uint32_t sumLimit = Sv >> 3; // Sv / 2 integer part Divide the sum with the number of CUs
	uint32_t sumLimitIncr = sumLimit;

	uint32_t curSum = 0;
	uint32_t elems = 1;
	uint32_t startElem = av;
	uint32_t endElem = av;


	for (int i = 1; i < 8; i++) {
		curSum = 0;
		while(curSum<=sumLimit)
		{
			curSum = elems*(startElem + endElem)/2;
			elems++;
			startElem--;
		}
		break_row[i] = (elems-1) * V_DATA_SIZE;
		break_vds_blk_idx[i] = OOPS_Hpacked_triMtx_start_idx(N, V_DATA_SIZE, break_row[i]);
		sumLimit += sumLimitIncr;


		matrix_prt_sBytes[i-1] = (break_vds_blk_idx[i] - break_vds_blk_idx[i-1]) * V_DATA_SIZE * sizeof(float);
		out_vec_prt_sBytes[i-1] = sizeof(float) * (break_row[i] - break_row[i-1]) * incX;

	}

		matrix_prt_sBytes[7] = (total_vds_blks - break_vds_blk_idx[7]) * V_DATA_SIZE * sizeof(float);
		out_vec_prt_sBytes[7] = sizeof(float) * (N - break_row[7]) * incX;


	cl::Kernel krnl;
	std::cout << "1." << std::endl;
	OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_tpmv", &err));

	uint32_t xidx1 = break_row[1];
	uint32_t xidx2 = break_row[2];
	uint32_t xidx3 = break_row[3];
	uint32_t xidx4 = break_row[4];
	uint32_t xidx5 = break_row[5];
	uint32_t xidx6 = break_row[6];
	uint32_t xidx7 = break_row[7];


	// Allocate Buffer in Global Memory
	std::cout << "1.1" << std::endl;
	OCL_CHECK(err, cl::Buffer Sx1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
															(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
													(void *) X, &err));

	std::cout << "1.2" << std::endl;
	OCL_CHECK(err, cl::Buffer Sa1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[0],
														(void *) AhyperPacked, &err));
	OCL_CHECK(err, cl::Buffer Sa2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[1],
														(void *) &AhyperPacked[break_vds_blk_idx[1] * V_DATA_SIZE], &err));
	OCL_CHECK(err, cl::Buffer Sa3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[2],
														(void *) &AhyperPacked[break_vds_blk_idx[2] * V_DATA_SIZE], &err));
	OCL_CHECK(err, cl::Buffer Sa4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[3],
														(void *) &AhyperPacked[break_vds_blk_idx[3] * V_DATA_SIZE], &err));
	OCL_CHECK(err, cl::Buffer Sa5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[4],
														(void *) &AhyperPacked[break_vds_blk_idx[4] * V_DATA_SIZE], &err));
	OCL_CHECK(err, cl::Buffer Sa6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[5],
														(void *) &AhyperPacked[break_vds_blk_idx[5] * V_DATA_SIZE], &err));
	OCL_CHECK(err, cl::Buffer Sa7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[6],
														(void *) &AhyperPacked[break_vds_blk_idx[6] * V_DATA_SIZE], &err));
	OCL_CHECK(err, cl::Buffer Sa8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_prt_sBytes[7],
														(void *) &AhyperPacked[break_vds_blk_idx[7] * V_DATA_SIZE], &err));


	std::cout << "1.3" << std::endl;
	OCL_CHECK(err, cl::Buffer Sx1_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[0],
														(void *) X, &err));
	OCL_CHECK(err, cl::Buffer Sx2_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[1],
														(void *) &X[xidx1], &err));
	OCL_CHECK(err, cl::Buffer Sx3_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[2],
														(void *) &X[xidx2], &err));
	OCL_CHECK(err, cl::Buffer Sx4_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[3],
														(void *) &X[xidx3], &err));
	OCL_CHECK(err, cl::Buffer Sx5_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[4],
														(void *) &X[xidx4], &err));
	OCL_CHECK(err, cl::Buffer Sx6_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[5],
														(void *) &X[xidx5], &err));
	OCL_CHECK(err, cl::Buffer Sx7_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[6],
														(void *) &X[xidx6], &err));
	OCL_CHECK(err, cl::Buffer Sx8_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, out_vec_prt_sBytes[7],
														(void *) &X[xidx7], &err));

	// Set the Kernel Arguments

	int narg = 0;


	OCL_CHECK(err, err = krnl.setArg(narg++, N));
	OCL_CHECK(err, err = krnl.setArg(narg++, total_vds_blks));

	OCL_CHECK(err, err = krnl.setArg(narg++, Sa1));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx1));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx1_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[1]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[1]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa2));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx2));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx2_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[2]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[2]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa3));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx3));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx3_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[3]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[3]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa4));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx4));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx4_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[4]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[4]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa5));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx5));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx5_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[5]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[5]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa6));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx6));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx6_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[6]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[6]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa7));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx7));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx7_out));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_row[7]));
	OCL_CHECK(err, err = krnl.setArg(narg++, break_vds_blk_idx[7]));


	OCL_CHECK(err, err = krnl.setArg(narg++, Sa8));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx8));
	OCL_CHECK(err, err = krnl.setArg(narg++, Sx8_out));


	// Copy input data to device global memory
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa1}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx1}, 0 /* 0 means from host*/));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa2}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx2}, 0 /* 0 means from host*/));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa3}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx3}, 0 /* 0 means from host*/));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa4}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx4}, 0 /* 0 means from host*/));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa5}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx5}, 0 /* 0 means from host*/));


	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa6}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx6}, 0 /* 0 means from host*/));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa7}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx7}, 0 /* 0 means from host*/));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa8}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx8}, 0 /* 0 means from host*/));
	OCL_CHECK(err, err = q.finish());


	// Launch the Kernel
	OCL_CHECK(err, err = q.enqueueTask(krnl));
	OCL_CHECK(err, err = q.finish());


	// Copy Result from Device Global Memory to Host Local Memory
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx1_out}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx2_out}, CL_MIGRATE_MEM_OBJECT_HOST));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx3_out}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx4_out}, CL_MIGRATE_MEM_OBJECT_HOST));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx5_out}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx6_out}, CL_MIGRATE_MEM_OBJECT_HOST));

	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx7_out}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx8_out}, CL_MIGRATE_MEM_OBJECT_HOST));

	OCL_CHECK(err, err = q.finish());
	clReleaseKernel(krnl.get());


}