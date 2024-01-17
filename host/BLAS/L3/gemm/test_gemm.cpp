void OOPS_gemm(const char TransA, const char TransB, const int M, const int N, const int K, const float alpha, const float  *A, const int lda,
				const float  *B, const int ldb, const float beta, float  *C, const int ldc)
{
	cl_int err;

	// number of Compute Units to spawn
	int nstripe = 8;
	std::vector<cl::Kernel> krnls(nstripe);

	// This call will get the kernel object from program. A kernel is an OpenCL function that is executed on the FPGA.
	std::string krnl_name = "krnl_gemm";
	for (int i = 0; i < nstripe; i++) {
		std::string cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + krnl_name +"_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}

	std::vector<int> stripe_nrows(nstripe);
	std::vector<int> stripe_start_A(nstripe);
	std::vector<int> stripe_start_C(nstripe);
	std::vector<int> stripe_nterms_A(nstripe);
	std::vector<int> stripe_nterms_B(nstripe);
	std::vector<int> stripe_nterms_C(nstripe);

	int stripesize = M/nstripe;
	int remainder  = M%nstripe;
	for (int i = 0; i < nstripe; i++) {
		// set-up stripe info
		stripe_nterms_B[i] = K * N;
		if ( i <= remainder ) {
			stripe_nrows[i]   = stripesize + 1;
			if ( i == remainder )
				stripe_nrows[i]--;
			stripe_nterms_A[i]  = stripe_nrows[i] * K;
			stripe_nterms_C[i]  = stripe_nrows[i] * N;
			if(i>0){
				stripe_start_A[i]	= stripe_start_A[i-1] + stripe_nterms_A[i-1];
				stripe_start_C[i]	= stripe_start_C[i-1] + stripe_nterms_C[i-1];
			}
		} else {
			stripe_nrows[i]		= stripesize;
			stripe_nterms_A[i]  = stripe_nrows[i] * K;
			stripe_nterms_C[i]  = stripe_nrows[i] * N;
			if(i>0){
				stripe_start_A[i]	= stripe_start_A[i-1] + stripe_nterms_A[i-1];
				stripe_start_C[i]	= stripe_start_C[i-1] + stripe_nterms_C[i-1];
			}
		}
		// printf("ROW BALANCING	 stripe = %d\tnrows = %d\tpart_A = %d (start = %d)\tpart_B = %d\tpart_C = %d (start = %d)\n", i, stripe_nrows[i], stripe_nterms_A[i], stripe_start_A[i], stripe_nterms_B[i], stripe_nterms_C[i], stripe_start_C[i]);
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
		memcpy(_A[i],A+stripe_start_A[i],stripe_nterms_A[i]*sizeof(float));
		// printf("buffer_A[%d] %lf MB\n", i, stripe_nterms_A[i]*sizeof(float)/(1024*1024.0));

		_B[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_B[i]*sizeof(float)));
		memcpy(_B[i],B,stripe_nterms_B[i]*sizeof(float));
		// printf("buffer_B[%d] %lf MB\n", i, stripe_nterms_B[i]*sizeof(float)/(1024*1024.0));

		_C_in[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_C[i]*sizeof(float)));
		memcpy(_C_in[i],C+stripe_start_C[i],stripe_nterms_C[i]*sizeof(float));
		// printf("buffer_C_in[%d] %lf MB\n", i, stripe_nterms_C[i]*sizeof(float)/(1024*1024.0));

		_C_out[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_C[i]*sizeof(float)));
		// printf("buffer_C_out[%d] %lf MB\n\n", i, stripe_nterms_C[i]*sizeof(float)/(1024*1024.0));

		OCL_CHECK(err, buffer_A[i]     = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_A[i]*sizeof(float), _A[i],     &err));
		OCL_CHECK(err, buffer_B[i]     = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_B[i]*sizeof(float), _B[i],     &err));
		OCL_CHECK(err, buffer_C_in[i]  = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_C[i]*sizeof(float), _C_in[i],  &err));
		OCL_CHECK(err, buffer_C_out[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, stripe_nterms_C[i]*sizeof(float), _C_out[i], &err));
	}

	// Set the Kernel Arguments
	for (int i = 0; i < nstripe; i++) {
		int narg=0;
		OCL_CHECK(err, err = krnls[i].setArg(narg++,Layout));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,TransA));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,TransB));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,stripe_nrows[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,N));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,K));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,alpha));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,lda));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_B[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,ldb));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,beta));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_C_in[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_C_out[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,ldc));

		// https://xilinx.github.io/Vitis_Accel_Examples/2019.2/html/data_transfer.html
		// After creating buffer using Host Mem Pointer, clEnqueueMigrateMemObjects can be used for immediate migration
		// of data without considering the fact that data is actually needed or not by kernel operation.
		// Copy input data to device global memory
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_A[i],buffer_B[i],buffer_C_in[i]}, 0 /* 0 means from host */));
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
		memcpy(C_out+stripe_start_C[i],_C_out[i], stripe_nrows[i]*N*sizeof(float));
	}
	q.finish();

	for (int i = 0; i < nstripe; i++){
		clReleaseKernel(krnls[i].get());
	}
}


