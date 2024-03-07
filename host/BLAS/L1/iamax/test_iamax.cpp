#include "test_functions_set.h"

int OOPS_iamax(const int N, const int NCU, const int MAX_CUS, const float  *X, const int incX){
	cl_int err;
	std::string cu_id;
	std::string krnl_name = "krnl_iamax";
	std::vector<cl::Kernel> krnl(NCU);

	size_t input_vector_size_bytes = sizeof(float) * (N/NCU)*incX;
	size_t last_input_vector_size_bytes = sizeof(float) * (N/NCU + N%NCU)*incX;
	std::vector<cl::Buffer> Sx(NCU);
	std::vector<cl::Buffer> result(NCU);

	int max_idx = 0;
	int base_idx = N/NCU;
	float max_val;
	int *partial_max_idx = (int*)OOPS_malloc(sizeof(int)*NCU);


	for (int i = 0; i < NCU; i++)
	{
		cu_id = std::to_string(i+1);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_iamax_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		// Here Kernel object is created by specifying kernel name along with
		// compute unit.
		// For such case, this kernel object can only access the specific
		// Compute unit
		OCL_CHECK(err, krnl[i] = cl::Kernel(program_interface.program, krnl_name_full.c_str(), &err));
	}
	
	// Allocate Buffer in Global Memory
	for (int i = 0; i < NCU; i++) 
	{
		if(i!=NCU-1){
			OCL_CHECK(err, Sx[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
								input_vector_size_bytes, (void*) &X[i*(N/NCU)], &err));
		}
		else {
			OCL_CHECK(err, Sx[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
					last_input_vector_size_bytes, (void*) &X[i*(N/NCU)], &err));
		}
			OCL_CHECK(err, result[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY ,
					sizeof(int),(void*)&partial_max_idx[i], &err));
	}

	int narg = 0;
	int num = (N/NCU + N%NCU);

	// Set the Kernel Arguments
	for (int i = 0; i < NCU; i++)
	{
		narg = 0;
		if(i!=NCU-1){
			OCL_CHECK(err, err = krnl[i].setArg(narg++, N/NCU));
		}
		else{
			OCL_CHECK(err, err = krnl[i].setArg(narg++, num));
		}
		OCL_CHECK(err, err = krnl[i].setArg(narg++, Sx[i]));
		OCL_CHECK(err, err = krnl[i].setArg(narg++, incX));
		OCL_CHECK(err, err = krnl[i].setArg(narg++, result[i]));

		//Copy input data to device global memory

		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx[i]}, 0 /* 0 means from host*/));

	}

	OCL_CHECK(err, err = program_interface.q.finish());

	// Launch the Kernel
	for (int i = 0; i < NCU; i++)
	{
		OCL_CHECK(err, err = program_interface.q.enqueueTask(krnl[i]));
	}
	OCL_CHECK(err, err = program_interface.q.finish());
	
	// Copy Result from Device Global Memory to Host Local Memory
	for (int i = 0; i < NCU; i++)
	{
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({result[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
	}
	OCL_CHECK(err, err = program_interface.q.finish());


	max_idx = partial_max_idx[0];
	max_val = X[max_idx];


	for (int i =0; i < NCU; i++)
	{
		if(max_val < X[(base_idx*i) + partial_max_idx[i]])
		{
			max_val = X[(base_idx*i) + partial_max_idx[i]];
			max_idx = (base_idx*i) + partial_max_idx[i];
		}
	}
	free(partial_max_idx);

	return max_idx;
}