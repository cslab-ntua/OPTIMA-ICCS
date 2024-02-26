#include "test_functions_set.h"


void OOPS_copy(const int N,const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY){

	cl_int err;
	std::string cu_id;
	std::string krnl_name = "krnl_copy";
	std::vector<cl::Kernel> krnl(NCU);

    size_t input_vector_size_bytes = sizeof(float) * (N/NCU)*incX;
    size_t output_vector_size_bytes = sizeof(float) * (N/NCU)*incY;

    size_t last_input_vector_size_bytes = sizeof(float) * (N/NCU + N%NCU)*incX;
	size_t last_output_vector_size_bytes = sizeof(float) * (N/NCU + N%NCU)*incY;


    std::vector<cl::Buffer> Sx(NCU);
    std::vector<cl::Buffer> Sy(NCU);


    for (int i = 0; i < NCU; i++) 
	{
		cu_id = std::to_string(MAX_CUS-i);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_copy_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), MAX_CUS-1-i);
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

			OCL_CHECK(err, Sy[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY ,
								output_vector_size_bytes, (void*) &Y[i*(N/NCU)], &err));

		}
		else {
			OCL_CHECK(err, Sx[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
					last_input_vector_size_bytes, (void*) &X[i*(N/NCU)], &err));

			OCL_CHECK(err, Sy[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY ,
					last_output_vector_size_bytes, (void*) &Y[i*(N/NCU) ], &err));
		}

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
		OCL_CHECK(err, err = krnl[i].setArg(narg++, Sy[i]));
		OCL_CHECK(err, err = krnl[i].setArg(narg++, incY));

		//Copy input data to device global memory
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx[i]}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy[i]}, 0 /* 0 means from host*/));
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
    	OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
	}
    OCL_CHECK(err, err = program_interface.q.finish());

}