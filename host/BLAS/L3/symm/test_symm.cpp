void OOPS_symm(const char Side, const char Uplo, const int M, const int N, const float alpha, const float  *A, const int lda, const float  *B,
		const int ldb, const float beta, float  *C, const int ldc)
{
	float* A_triang= (float *)OOPS_malloc(sizeof(float)*M*(M+1)/2);
	int cnt=0;
	for (int i = 0; i < M; i++){
		for (int j = 0; j < M; j++){
			if(i<=j)
				A_triang[cnt++] = A[i * M + j];
		}
	}

	cl_int err;

	// number of Compute Units to spawn
	int nstripe = 2;
	std::vector<cl::Kernel> krnls(nstripe);

	// This call will get the kernel object from program. A kernel is an OpenCL function that is executed on the FPGA.
	std::string krnl_name = "krnl_symm";
	for (int i = 0; i < nstripe; i++) {
		std::string cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + krnl_name +"_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}

	std::vector<int> stripe_ncols(nstripe);
	std::vector<int> stripe_col_offset(nstripe);
	std::vector<int> stripe_nterms_A(nstripe);
	std::vector<int> stripe_nterms_B(nstripe);
	std::vector<int> stripe_nterms_C_in(nstripe);
	std::vector<int> stripe_nterms_C_out(nstripe);

	int stripesize = N/nstripe; // now B will be split in stripes, so divide by columns (B has N columns)
	int remainder  = N%nstripe;
	for (int i = 0; i < nstripe; i++) {
		// set-up stripe info
		stripe_nterms_A[i] = M*(M+1)/2; // since A is an upper triangular matrix
		if ( i <= remainder ) {
			stripe_ncols[i]   = stripesize + 1;
			if ( i == remainder )
				stripe_ncols[i]--;
		}
		else {
			stripe_ncols[i]		= stripesize;
			if(i>0)
				stripe_col_offset[i] = stripe_col_offset[i-1] + stripe_ncols[i];
		}
		stripe_nterms_B[i]  = M * stripe_ncols[i];
		stripe_nterms_C_in[i]  = M * stripe_ncols[i];
		stripe_nterms_C_out[i]  = M * stripe_ncols[i];
		// printf("ROW BALANCING	 stripe = %d\tncols = %d\tpart_A = %d\tpart_B = %d\tpart_C_in = %d\tpart_C_in = %d (start col offset = %d)\n", i, stripe_ncols[i], stripe_nterms_A[i], stripe_nterms_B[i], stripe_nterms_C_in[i], stripe_nterms_C_out[i], stripe_col_offset[i]);
	}

	std::vector<cl::Buffer> buffer_A(nstripe);
	std::vector<cl::Buffer> buffer_B(nstripe);
	std::vector<cl::Buffer> buffer_C_in(nstripe);
	std::vector<cl::Buffer> buffer_C_out(nstripe);

	float* _A[nstripe];
	float* _B[nstripe];
	float* _C_in[nstripe];
	float* _C_out[nstripe];
	
	// Allocate Buffer in Global Memory
	for (int i = 0; i < nstripe; i++) {
		_A[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_A[i]*sizeof(float)));
		memcpy(_A[i],A_triang,stripe_nterms_A[i]*sizeof(float));
		// printf("buffer_A[%d] %lf MB\n", i, stripe_nterms_A[i]*sizeof(float)/(1024*1024.0));

		_B[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_B[i]*sizeof(float)));
		for(int ii=0; ii<M; ii++)
			for(int jj=0; jj<stripe_ncols[i]; jj++)
				_B[i][ii*stripe_ncols[i] + jj] = B[ii*N + jj + stripe_col_offset[i]];
		// printf("buffer_B[%d] %lf MB\n", i, stripe_nterms_B[i]*sizeof(float)/(1024*1024.0));

		_C_in[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_C_in[i]*sizeof(float)));
		for(int ii=0; ii<M; ii++)
			for(int jj=0; jj<stripe_ncols[i]; jj++)
				_C_in[i][ii*stripe_ncols[i] + jj] = C[ii * N + jj + stripe_col_offset[i]];
		// printf("buffer_C_in[%d] %lf MB\n", i, stripe_nterms_C_in[i]*sizeof(float)/(1024*1024.0));

		_C_out[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_C_out[i]*sizeof(float)));
		// printf("buffer_C_out[%d] %lf MB\n\n", i, stripe_nterms_C_out[i]*sizeof(float)/(1024*1024.0));

		OCL_CHECK(err, buffer_A[i]     = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_A[i]*sizeof(float),     _A[i],     &err));
		OCL_CHECK(err, buffer_B[i]     = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_B[i]*sizeof(float),     _B[i],     &err));
		OCL_CHECK(err, buffer_C_in[i]  = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_C_in[i]*sizeof(float),  _C_in[i],  &err));
		OCL_CHECK(err, buffer_C_out[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, stripe_nterms_C_out[i]*sizeof(float), _C_out[i], &err));
	}

	// Set the Kernel Arguments
	for (int i = 0; i < nstripe; i++) {
		int narg=0;

		OCL_CHECK(err, err = krnls[i].setArg(narg++,Side));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,Uplo));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,M));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,stripe_ncols[i]));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,alpha));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,lda));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_B[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,ldb));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,beta));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_C_in[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_C_out[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,ldc));

		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_A[i],buffer_B[i],buffer_C_in[i]}, 0));
	}
	q.finish();
	
	// Launch the Kernel
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
	}
	q.finish();

	// Copy Result from Device Global Memory to Host Local Memory
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_C_out[i]},1));
		for(int ii=0; ii<M; ii++){
			for(int jj=0; jj<stripe_ncols[i]; jj++){
				C[ii*M + jj + stripe_col_offset[i]] = _C_out[i][ii*stripe_ncols[i] + jj];
			}
		}
	}
	q.finish();

	for (int i = 0; i < nstripe; i++){
		clReleaseKernel(krnls[i].get());
	}
	free(A_triang);
}
