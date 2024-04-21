#include "test_functions_set.h"
#include "oops.hpp"

void OOPS_trsv(const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *A, const int lda, float  *X, const int incX){
	float* A_trans = (float*) OOPS_malloc(sizeof(float)*N*N);
	for (int j = 0; j < N; j++){
		for (int i = 0; i < N; i++){
			A_trans[j*N+i] = A[i*N + j];
		}
	}

	cl_int err;

	// number of Compute Units to spawn
	int nstripe = 1;
	std::vector<cl::Kernel> krnls(nstripe);

	// This call will get the kernel object from program_interface.program. A kernel is an OpenCL function that is executed on the FPGA.
	std::string krnl_name = "krnl_trsv";
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
	std::vector<int> stripe_nterms_X(nstripe);
	
	int stripesize = 1; // B cannot be split in tiles, as it is 1D. just copy it from TRSM and specialize for 1 column
	int remainder  = 0;
	for (int i = 0; i < nstripe; i++) {
		// set-up stripe info
		stripe_nterms_A[i] = N * N; // since A is an upper triangular matrix

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
		stripe_nterms_B[i]  = N * stripe_ncols[i];
		stripe_nterms_X[i]  = N * stripe_ncols[i];
		// printf("ROW BALANCING	 stripe = %d\tncols = %d\tpart_A = %d\tpart_B = %d\tpart_X = %d (start col offset = %d)\n", i, stripe_ncols[i], stripe_nterms_A[i], stripe_nterms_B[i], stripe_nterms_X[i], stripe_col_offset[i]);
	}

	/*******************************************************************************************/
	int W = 16;
	int values = (N*(N+1))/2; // total nonzero values of A triangular matrix
	int num_of_CUs = 16;
	int values_per_stream2 = values/num_of_CUs;
	int cnt = 0;
	int cnt2 = 0;
	int row_break[num_of_CUs-1];
	for (int i=1; i<=N; i++){
		cnt += i;
		if(cnt >= values_per_stream2)
		{
			cnt = 0;
			row_break[cnt2] = i;
			// printf("cnt2 = %d, split in line %d (before)\n", cnt2, row_break[cnt2]);
			row_break[cnt2] = row_break[cnt2]/W;
			row_break[cnt2] = row_break[cnt2]*W;
			// printf("cnt2 = %d, split in line %d (after)\n", cnt2, row_break[cnt2]);
			cnt2++;
		}
	}
	// for(int i=0; i<num_of_CUs-1; i++)
	// 	printf("finally, row_break[%d] = %d\n", i, row_break[i]);
	/*******************************************************************************************/

	std::vector<cl::Buffer> buffer_A1(nstripe);
	std::vector<cl::Buffer> buffer_A2(nstripe);
	std::vector<cl::Buffer> buffer_A3(nstripe);
	std::vector<cl::Buffer> buffer_A4(nstripe);
	std::vector<cl::Buffer> buffer_A5(nstripe);
	std::vector<cl::Buffer> buffer_A6(nstripe);
	std::vector<cl::Buffer> buffer_A7(nstripe);
	std::vector<cl::Buffer> buffer_A8(nstripe);
	std::vector<cl::Buffer> buffer_A9(nstripe);
	std::vector<cl::Buffer> buffer_A10(nstripe);
	std::vector<cl::Buffer> buffer_A11(nstripe);
	std::vector<cl::Buffer> buffer_A12(nstripe);
	std::vector<cl::Buffer> buffer_A13(nstripe);
	std::vector<cl::Buffer> buffer_A14(nstripe);
	std::vector<cl::Buffer> buffer_A15(nstripe);
	std::vector<cl::Buffer> buffer_A16(nstripe);
	std::vector<cl::Buffer> buffer_B(nstripe);
	std::vector<cl::Buffer> buffer_X(nstripe);

	float* _A[nstripe];
	float* _B[nstripe];
	float* _X[nstripe];
	
	// Allocate Buffer in Global Memory
	for (int i = 0; i < nstripe; i++) {
		_A[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_A[i]*sizeof(float)));
		memcpy(_A[i],A_m_trans,stripe_nterms_A[i]*sizeof(float));
		
		_B[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_B[i]*sizeof(float)));
		for(int ii=0; ii<N; ii++)
			for(int jj=0; jj<stripe_ncols[i]; jj++)
				_B[i][ii*stripe_ncols[i] + jj] = B[ii*1 + jj + stripe_col_offset[i]];

		_X[i]= (float*) OOPS_malloc((size_t)(stripe_nterms_X[i]*sizeof(float)));


		OCL_CHECK(err, buffer_A1[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A2[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A3[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A4[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A5[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A6[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A7[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A8[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A9[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A10[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A11[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A12[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A13[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A14[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A15[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_A16[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(float), _A[i], &err));
		OCL_CHECK(err, buffer_B[i]   = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,   stripe_nterms_B[i]*sizeof(float), _B[i],  &err));
		OCL_CHECK(err, buffer_X[i]   = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,	stripe_nterms_X[i]*sizeof(float), _X[i], &err));
	}

	// Set the Kernel Arguments
	for (int i = 0; i < nstripe; i++) {
		int narg=0;

		OCL_CHECK(err, err = krnls[i].setArg(narg++,Uplo));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,TransA));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,Diag));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,K));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A1[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A2[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A3[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A4[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A5[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A6[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A7[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A8[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A9[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A10[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A11[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A12[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A13[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A14[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A15[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_A16[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,lda));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_B[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_X[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++,ldb));

		for(int k=0; k<num_of_CUs-1; k++)
			OCL_CHECK(err, err =krnls[i].setArg(narg++,row_break[k]));

		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({buffer_A1[i],buffer_A2[i],buffer_A3[i],buffer_A4[i],buffer_A5[i],buffer_A6[i],buffer_A7[i],buffer_A8[i],buffer_A9[i],buffer_A10[i],buffer_A11[i],buffer_A12[i],buffer_A13[i],buffer_A14[i],buffer_A15[i],buffer_A16[i],buffer_B[i],buffer_X[i]}, 0));

	}
	program_interface.q.finish();
	
	// Launch the Kernel
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = program_interface.q.enqueueTask(krnls[i]));
	}
	program_interface.q.finish();

	// Copy Result from Device Global Memory to Host Local Memory
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({buffer_X[i]},1));
		for(int ii=0; ii<K; ii++){
			for(int jj=0; jj<stripe_ncols[i]; jj++){
				X[ii*1 + jj + stripe_col_offset[i]] = _X[i][ii*stripe_ncols[i] + jj];
			}
		}
	}
	program_interface.q.finish();

	for (int i = 0; i < nstripe; i++){
		clReleaseKernel(krnls[i].get());
	}
    free(A_trans);
}
