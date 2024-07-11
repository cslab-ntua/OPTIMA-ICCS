#include "test_functions_set.h"

bool OOPS_lu(float *L, float *U, int N){ //OOPS_lu3d15df_b

	int NCUs = 1, luVdt = 16;

	cl_int err;
	std::string krnl_name = "krnl_lu"; //krnl_lu3d15df_b
	std::string cu_id;
	std::vector<cl::Kernel> krnls(NCUs);
	// Creating Buffers
	std::vector<cl::Buffer> SURowsOut(NCUs);
	std::vector<cl::Buffer> SLRowsOut(NCUs);
	std::vector<cl::Buffer> SLRows1Out(NCUs);
	std::vector<cl::Buffer> SLRows2Out(NCUs);
	std::vector<cl::Buffer> SLRows3Out(NCUs);
	std::vector<cl::Buffer> SLRows4Out(NCUs);
	std::vector<cl::Buffer> SURowsHBMBuffer(NCUs);
	std::vector<cl::Buffer> SURowsHBMBuffer2(NCUs);
	string tmpUrowsName = "";

	int narg = 0, tmpUrowOutSize = 0;
	float *UrowsOut[NCUs], *LrowsOut[NCUs], *Lrows1Out[NCUs], *Lrows2Out[NCUs], *Lrows3Out[NCUs], *Lrows4Out[NCUs], *URowsHBMBuffer[NCUs], *URowsHBMBuffer2[NCUs];
	int URowsHBMBufferMaxNumOfRows = 0, UrowsOutSize = 0, URowsHBMBufferSize = 0;
	int paddedRowWidth = 0, LrowsOutSize = 0;
	size_t rowBytes = 0, URowsHBMBufferMallocBytes = 0, UrowsOutMallocBytes = 0, LrowsOutMallocBytes = 0;


	paddedRowWidth = roundUpToMultiple(N,luVdt);

	URowsHBMBufferMaxNumOfRows = N;
	URowsHBMBufferSize = URowsHBMBufferMaxNumOfRows*paddedRowWidth;
	URowsHBMBufferMallocBytes = URowsHBMBufferSize*sizeof(float);

	//printf("LU: UrowsOut row sizes (N=%d):\n",N);
	for (int ur=0;ur<N-1;ur++) {
		tmpUrowOutSize = roundUpToMultiple(N-1-ur,luVdt);

		UrowsOutSize = UrowsOutSize + tmpUrowOutSize;
	}

	UrowsOutMallocBytes = UrowsOutSize*sizeof(float);
	LrowsOutSize = UrowsOutSize;
	LrowsOutMallocBytes = LrowsOutSize*sizeof(float);

	for (int i=0;i<NCUs;i++) {
		URowsHBMBuffer[i] = (float *)OOPS_malloc(URowsHBMBufferMallocBytes);
		if (URowsHBMBuffer[i] == NULL) {
			std::cout << "LU: URowsHBMBuffer[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(URowsHBMBuffer[i],0,URowsHBMBufferMallocBytes);
		}
		URowsHBMBuffer2[i] = (float *)OOPS_malloc(URowsHBMBufferMallocBytes);
		if (URowsHBMBuffer2[i] == NULL) {
			std::cout << "LU: URowsHBMBuffer2[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(URowsHBMBuffer2[i],0,URowsHBMBufferMallocBytes);
		}

		UrowsOut[i] = (float *)OOPS_malloc(UrowsOutMallocBytes);
		if (UrowsOut[i] == NULL) {
			std::cout << "LU: UrowsOut[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(UrowsOut[i],0,UrowsOutMallocBytes);
		}
		LrowsOut[i] = (float *)OOPS_malloc(LrowsOutMallocBytes);
		if (LrowsOut[i] == NULL) {
			std::cout << "LU: LrowsOut[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(LrowsOut[i],0,LrowsOutMallocBytes);
		}
		Lrows1Out[i] = (float *)OOPS_malloc(LrowsOutMallocBytes);
		if (Lrows1Out[i] == NULL) {
			std::cout << "LU: Lrows1Out[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(Lrows1Out[i],0,LrowsOutMallocBytes);
		}
		Lrows2Out[i] = (float *)OOPS_malloc(LrowsOutMallocBytes);
		if (Lrows2Out[i] == NULL) {
			std::cout << "LU: Lrows2Out[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(Lrows2Out[i],0,LrowsOutMallocBytes);
		}
		Lrows3Out[i] = (float *)OOPS_malloc(LrowsOutMallocBytes);
		if (Lrows3Out[i] == NULL) {
			std::cout << "LU: Lrows3Out[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(Lrows3Out[i],0,LrowsOutMallocBytes);
		}
		Lrows4Out[i] = (float *)OOPS_malloc(LrowsOutMallocBytes);
		if (Lrows4Out[i] == NULL) {
			std::cout << "LU: Lrows4Out[" << i << "] = NULL abort.." << std::endl;
			return false;
		}
		else {
			memset(Lrows4Out[i],0,LrowsOutMallocBytes);
		}
	}

	for (int i=0;i<NCUs;i++) {
		cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_lu_" + cu_id + "}";
		//printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls[i] = cl::Kernel(program_interface.program, krnl_name_full.c_str(), &err));
	}

	program_interface.q.finish();

	rowBytes = N*sizeof(float);

	//fix the first buffer
	for (int r=0;r<N;r++) {
		memcpy(&URowsHBMBuffer[0][r*paddedRowWidth],&U[r*N],rowBytes);
	}

	//copy the first buffer to the buffers of other CUs (yes, they will calculate the same LU decomposition)
	for (int i=1;i<NCUs;i++) {
		memcpy(&URowsHBMBuffer[i][0],&URowsHBMBuffer[0][0],URowsHBMBufferMallocBytes);
	}



	for (int i=0;i<NCUs;i++) {
		narg = 0;
		OCL_CHECK(err, SURowsOut[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, UrowsOutMallocBytes,&UrowsOut[i][0], &err));
		OCL_CHECK(err, SLRowsOut[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, LrowsOutMallocBytes,&LrowsOut[i][0], &err));
		OCL_CHECK(err, SLRows1Out[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, LrowsOutMallocBytes,&Lrows1Out[i][0], &err));
		OCL_CHECK(err, SLRows2Out[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, LrowsOutMallocBytes,&Lrows2Out[i][0], &err));
		OCL_CHECK(err, SLRows3Out[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, LrowsOutMallocBytes,&Lrows3Out[i][0], &err));
		OCL_CHECK(err, SLRows4Out[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, LrowsOutMallocBytes,&Lrows4Out[i][0], &err));

		OCL_CHECK(err, SURowsHBMBuffer[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, URowsHBMBufferMallocBytes,&URowsHBMBuffer[i][0], &err));
		OCL_CHECK(err, SURowsHBMBuffer2[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, URowsHBMBufferMallocBytes,&URowsHBMBuffer2[i][0], &err));

		OCL_CHECK(err, err = krnls[i].setArg(narg++, N));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SURowsOut[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SLRowsOut[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SLRows1Out[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SLRows2Out[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SLRows3Out[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SLRows4Out[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SURowsHBMBuffer[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SURowsHBMBuffer2[i]));

		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({SURowsHBMBuffer[i]}, 0));
	}

	program_interface.q.finish();


	// Launch the Kernel


	for (int i=0;i<NCUs;i++) {
		OCL_CHECK(err, err = program_interface.q.enqueueTask(krnls[i]));
	}

	program_interface.q.finish();



	for (int i=0;i<NCUs;i++) {
		err = program_interface.q.enqueueMigrateMemObjects({SURowsOut[i],SLRowsOut[i],SLRows1Out[i],SLRows2Out[i],SLRows3Out[i],SLRows4Out[i]}, CL_MIGRATE_MEM_OBJECT_HOST);
	}

	program_interface.q.finish();


	//clear the entire U matrix, except its 1st row
	memset(&U[N],0,N*(N-1)*sizeof(float));


	//copy results back from UrowsOut to U
	int tmpNextUrowOutStartIdx = 0, tmpPaddedRowWidth = 0;
	for (int r=0;r<N-1;r++) {
		tmpPaddedRowWidth = roundUpToMultiple(N-1-r,luVdt);
		memcpy(&U[(r+1)*N+r+1],&UrowsOut[0][tmpNextUrowOutStartIdx],(N-r-1)*sizeof(float));
		tmpNextUrowOutStartIdx = tmpNextUrowOutStartIdx + tmpPaddedRowWidth;
	}

	//copy back data from LrowsOut to L

	int tmpColElemNum = 0, bufId = 0;
	int buff0StartIdx = 0, buff1StartIdx = 0, buff2StartIdx = 0, buff3StartIdx = 0, buff4StartIdx = 0;
	int buff0EndIdx = 0, buff1EndIdx = 0, buff2EndIdx = 0, buff3EndIdx = 0, buff4EndIdx = 0;
	for (int c=0;c<N-1;c++) {

		tmpColElemNum = roundUpToMultiple(N-c-1,luVdt);

		if (bufId == 0) {
			buff0EndIdx = buff0StartIdx + N-c-1;
			//printf("LU: c = %d, bufId = %d: buff0StartIdx = %d, buff0EndIdx = %d\n",c,bufId,buff0StartIdx,buff0EndIdx);
			for (int lr=buff0StartIdx,i=0;lr<buff0EndIdx;lr++,i++) {
				memcpy(&L[(c+1)*N+c+i*N],&LrowsOut[0][lr],sizeof(float));
			}
			buff0StartIdx+=tmpColElemNum;
			bufId++;
		}
		else if (bufId == 1) {
			buff1EndIdx = buff1StartIdx + N-c-1;

			for (int lr=buff1StartIdx,i=0;lr<buff1EndIdx;lr++,i++) {
				memcpy(&L[(c+1)*N+c+i*N],&Lrows1Out[0][lr],sizeof(float));
			}
			buff1StartIdx+=tmpColElemNum;
			bufId++;
		}
		else if (bufId == 2) {
			buff2EndIdx = buff2StartIdx + N-c-1;

			for (int lr=buff2StartIdx,i=0;lr<buff2EndIdx;lr++,i++) {
				memcpy(&L[(c+1)*N+c+i*N],&Lrows2Out[0][lr],sizeof(float));
			}
			buff2StartIdx+=tmpColElemNum;
			bufId++;
		}
		else if (bufId == 3) {
			buff3EndIdx = buff3StartIdx + N-c-1;

			for (int lr=buff3StartIdx,i=0;lr<buff3EndIdx;lr++,i++) {
				memcpy(&L[(c+1)*N+c+i*N],&Lrows3Out[0][lr],sizeof(float));
			}
			buff3StartIdx+=tmpColElemNum;
			bufId++;
		}
		else {
			buff4EndIdx = buff4StartIdx + N-c-1;

			for (int lr=buff4StartIdx,i=0;lr<buff4EndIdx;lr++,i++) {
				memcpy(&L[(c+1)*N+c+i*N],&Lrows4Out[0][lr],sizeof(float));
			}
			buff4StartIdx+=tmpColElemNum;
			bufId = 0;
		}
	}


	for (int i=0;i<NCUs;i++) {
		free(UrowsOut[i]);
		free(LrowsOut[i]); free(Lrows1Out[i]); free(Lrows2Out[i]); free(Lrows3Out[i]); free(Lrows4Out[i]);
		free(URowsHBMBuffer[i]);
		free(URowsHBMBuffer2[i]);
	}

	//NOTE: sometimes it returns "corrupted double-linked list"
	//for (int i=0;i<NCUs;i++) {
	//	clReleaseKernel(krnls[i].get());
	//}

	return true;
}