void OOPS_scal( const uint64_t N, const uint32_t NCU, const uint32_t MAX_CUS, const float alpha, float *X, const int incX){

	cl_int err;
	std::string cu_id;
	std::string krnl_name = "krnl_scal";
	std::vector<cl::Kernel> krnl(NCU);

	size_t input_vector_size_bytes = sizeof(float) * (N/NCU)*incX;
	size_t last_input_vector_size_bytes = sizeof(float) * (N/NCU + N%NCU)*incX;
	std::vector<cl::Buffer> Sx(NCU);


	for (uint32_t i = 0; i < NCU; i++) 
	{
		cu_id = std::to_string(MAX_CUS-i);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_scal_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), MAX_CUS-1-i);
		// Here Kernel object is created by specifying kernel name along with
		// compute unit.
		// For such case, this kernel object can only access the specific
		// Compute unit
		OCL_CHECK(err, krnl[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}


	// Allocate Buffer in Global Memory
	for (uint32_t i = 0; i < NCU; i++)
	{
		if(i!=NCU-1){
			OCL_CHECK(err, Sx[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
								input_vector_size_bytes, (void*) &X[i*(N/NCU)], &err));
		}
		else {
			OCL_CHECK(err, Sx[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,
					last_input_vector_size_bytes, (void*) &X[i*(N/NCU)], &err));
		}
	}


   int narg = 0;
   int num = (N/NCU + N%NCU);

   // Set the Kernel Arguments
   for (uint32_t i = 0; i < NCU; i++)
   {
	narg = 0;
	if(i!=NCU-1){
		OCL_CHECK(err, err = krnl[i].setArg(narg++, int(N/NCU)));
	}
	else{
		OCL_CHECK(err, err = krnl[i].setArg(narg++, num));
	}
	OCL_CHECK(err, err = krnl[i].setArg(narg++, alpha));
	OCL_CHECK(err, err = krnl[i].setArg(narg++, Sx[i]));
	OCL_CHECK(err, err = krnl[i].setArg(narg++, incX));

	//Copy input data to device global memory
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx[i]}, 0 /* 0 means from host*/));
   }

   OCL_CHECK(err, err = q.finish());


   // Launch the Kernel
	for (uint32_t i = 0; i < NCU; i++)
	{
		OCL_CHECK(err, err = q.enqueueTask(krnl[i]));
	}
   OCL_CHECK(err, err = q.finish());
  
   // Copy Result from Device Global Memory to Host Local Memory
   for (uint32_t i = 0; i < NCU; i++)
	{
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
	}
   OCL_CHECK(err, err = q.finish());


}