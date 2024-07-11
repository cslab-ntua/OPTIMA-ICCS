#include "oops.hpp"


void OOPS_jacobi( const int N, float *X, float *Y){

	int NCUs = 16, plEngines = 1;  //NCUs are the "hardwired" HLS CUs, plEngines is the number of hardware kernels
	cl_int err;
	std::string krnl_name = "krnl_jacobi";
	std::string cu_id;
	std::vector<cl::Kernel> krnls(plEngines);
	std::vector<cl::Buffer> Sx(NCUs);
	std::vector<cl::Buffer> Sy(NCUs);
	float* diagX[NCUs], *diagY[NCUs];
	int rowsPerPlEngine[NCUs];
	int narg = 0, cuStartLine = 0;

	rowDistribution(N,NCUs,&rowsPerPlEngine[0]);

	for (int i=0;i<NCUs;i++) {

		diagX[i] = (float *)OOPS_malloc(rowsPerPlEngine[i]*sizeof(float));
		diagY[i] = (float *)OOPS_malloc(rowsPerPlEngine[i]*sizeof(float));
		memset(&diagX[i][0],0,rowsPerPlEngine[i]*sizeof(float));
		memset(&diagY[i][0],0,rowsPerPlEngine[i]*sizeof(float));

		for (int j=0, iX=cuStartLine;j<rowsPerPlEngine[i];j++,iX++) {
			if (X[iX*(N+1)] != 0.0f) {
				memcpy(&diagX[i][j],&X[iX*(N+1)],sizeof(float));
			}
		}

		cuStartLine+=rowsPerPlEngine[i];
	}

	for (int i = 0; i < plEngines; i++) {
		cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_jacobi_" + cu_id + "}";
		OCL_CHECK(err, krnls[i] = cl::Kernel(program_interface.program, krnl_name_full.c_str(), &err));
	}

	size_t vectorSizeBytes = 0;
	for (int i = 0; i < NCUs; i++) {
		vectorSizeBytes = rowsPerPlEngine[i] * sizeof(float);
		OCL_CHECK(err, Sx[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vectorSizeBytes,&diagX[i][0], &err));
		OCL_CHECK(err, Sy[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vectorSizeBytes,&diagY[i][0], &err));
	}


	narg = 0;
	for (int i = 0; i < NCUs; i++) {
		OCL_CHECK(err, err = krnls[0].setArg(narg++, rowsPerPlEngine[i]));
		OCL_CHECK(err, err = krnls[0].setArg(narg++, Sx[i]));
		OCL_CHECK(err, err = krnls[0].setArg(narg++, Sy[i]));
		// Copy input data to device global memory
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sx[i]}, 0 /* 0 means from host*/));
	}

    program_interface.q.finish();

    // Launch the Kernel
    
    for (int i = 0; i < plEngines; i++) {
    	OCL_CHECK(err, err = program_interface.q.enqueueTask(krnls[i]));
	}
   
    program_interface.q.finish();

  
    // Copy Result from Device Global Memory to Host Local Memory
    for (int i = 0; i < NCUs; i++) {
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({Sy[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
	}
    program_interface.q.finish();

   
    cuStartLine = 0;
    for (int i=0;i<NCUs;i++) {
    	for (int j=0, iX=cuStartLine;j<rowsPerPlEngine[i];j++,iX++) {
    		memcpy(&Y[iX*(N+1)],&diagY[i][j],sizeof(float));
		}
    	cuStartLine+=rowsPerPlEngine[i];
    }

   
    for (int i=0;i<NCUs;i++) {
    	free(diagX[i]);
		free(diagY[i]);
    }

}

