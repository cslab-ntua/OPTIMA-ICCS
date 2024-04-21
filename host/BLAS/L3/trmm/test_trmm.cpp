#include "test_functions_set.h"

void OOPS_trmm(const char Side, const char Uplo, const  char TransA, const char Diag, const int M, const int N, const float alpha,
				const float *A, const int lda, const float *B, const int ldb, float *C)
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
	int nstripe = 4;
	std::vector<cl::Kernel> krnls(nstripe);

	// This call will get the kernel object from program. A kernel is an OpenCL function that is executed on the FPGA.
	std::string krnl_name = "krnl_trmm";
	for (int i = 0; i < nstripe; i++) {
		std::string cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + krnl_name +"_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls[i] = cl::Kernel(program_interface.program, krnl_name_full.c_str(), &err));
	}

	std::vector<int> stripe_ncols(nstripe);
	std::vector<int> stripe_col_offset(nstripe);
	std::vector<int> stripe_nterms_A(nstripe);
	std::vector<int> stripe_nterms_B(nstripe);
	std::vector<int> stripe_nterms_C(nstripe);

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
		stripe_nterms_C[i]  = M * stripe_ncols[i];
		// printf("ROW BALANCING	 stripe = %d\tncols = %d\tpart_A = %d\tpart_B = %d\tpart_C = %d (start col offset = %d)\n", i, stripe_ncols[i], stripe_nterms_A[i], stripe_nterms_B[i], stripe_nterms_C[i], stripe_col_offset[i]);
	}

	std::vector<cl::Buffer> buffer_A(nstripe);
	std::vector<cl::Buffer> buffer_B(nstripe);
	std::vector<cl::Buffer> buffer_C(nstripe);

	float* _A[nstripe];
	float* _B[nstripe];
	float* _C[nstripe];
	
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

		_C[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_C[i]*sizeof(float)));
		// printf("buffer_C[%d] %lf MB\n\n", i, stripe_nterms_C[i]*sizeof(float)/(1024*1024.0));

		OCL_CHECK(err, buffer_A[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_B[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterms_B[i]*sizeof(float), _B[i], &err));
		OCL_CHECK(err, buffer_C[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, stripe_nterms_C[i]*sizeof(float), _C[i], &err));
	}

	// Set the Kernel Arguments
	for (int i = 0; i < nstripe; i++) {
		int narg=0;
		OCL_CHECK(err, err = krnls[i].setArg(narg++,Side));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,Uplo));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,TransA));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,Diag));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,M));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,stripe_ncols[i]));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,alpha));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,lda));

		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_B[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_C[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,ldb));

		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({buffer_A[i],buffer_B[i]}, 0));
	}
	program_interface.q.finish();
	
	// Launch the Kernel
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = program_interface.q.enqueueTask(krnls[i]));
	}
	program_interface.q.finish();

	// Copy Result from Device Global Memory to Host Local Memory
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({buffer_C[i]},1));
		for(int ii=0; ii<M; ii++){
			for(int jj=0; jj<stripe_ncols[i]; jj++){
				C[ii*N + jj + stripe_col_offset[i]] = _C[i][ii*stripe_ncols[i] + jj];
			}
		}
	}
	program_interface.q.finish();

	for (int i = 0; i < nstripe; i++){
		clReleaseKernel(krnls[i].get());
	}
	free(A_triang);
}
