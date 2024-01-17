

bool OOPS_lu(float *L, float *U, int N, duration<double> *mallocTime, duration<double> *krnlTime, duration<double> *memcpyTime, duration<double> *h2dTime){

	int NCUs = 1, luVdt = 16;

	cl_int err;
	std::string krnl_name = "krnl_lu";
	std::string cu_id;
	std::vector<cl::Kernel> krnls(NCUs);
	// Creating Buffers
	std::vector<cl::Buffer> SURowsOut(NCUs);
	std::vector<cl::Buffer> SLRowsOut(NCUs);
	std::vector<cl::Buffer> SURowsHBMBuffer(NCUs);
	std::vector<cl::Buffer> SURowsHBMBuffer2(NCUs);
	string tmpUrowsName = "";

	int narg = 0, tmpUrowOutSize = 0;
	float *UrowsOut[NCUs], *LrowsOut[NCUs], *URowsHBMBuffer[NCUs], *URowsHBMBuffer2[NCUs];
	int URowsHBMBufferMaxNumOfRows = 0, UrowsOutSize = 0, URowsHBMBufferSize = 0;
	int paddedRowWidth = 0, LrowsOutSize = 0;
	size_t rowBytes = 0, URowsHBMBufferMallocBytes = 0, UrowsOutMallocBytes = 0, LrowsOutMallocBytes = 0;

	high_resolution_clock::time_point tmpStart = high_resolution_clock::now();
	*krnlTime = duration_cast<duration<double>>(tmpStart - tmpStart);

	*memcpyTime = duration_cast<duration<double>>(tmpStart - tmpStart);
	*h2dTime = duration_cast<duration<double>>(tmpStart - tmpStart);
	*mallocTime = duration_cast<duration<double>>(tmpStart - tmpStart);

	high_resolution_clock::time_point mallocHwStart = high_resolution_clock::now();

	paddedRowWidth = roundUpToMultiple(N,luVdt);

	URowsHBMBufferMaxNumOfRows = N;
	URowsHBMBufferSize = URowsHBMBufferMaxNumOfRows*paddedRowWidth;
	URowsHBMBufferMallocBytes = URowsHBMBufferSize*sizeof(float);

	printf("LU: UrowsOut row sizes (N=%d):\n",N);
	for (int ur=0;ur<N-1;ur++) {
		tmpUrowOutSize = roundUpToMultiple(N-1-ur,luVdt);
		//printf("LU: UrowsOut row %d has %d elements (%d w padding)\n",ur,N-1-ur,tmpUrowOutSize);
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
	}



	for (int i=0;i<NCUs;i++) {
		cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_lu_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}

	q.finish();

	rowBytes = N*sizeof(float);

	//fix the first buffer
	for (int r=0;r<N;r++) {
		memcpy(&URowsHBMBuffer[0][r*paddedRowWidth],&U[r*N],rowBytes);
	}

	//copy the first buffer to the buffers of other CUs (yes, they will calculate the same LU decomposition)
	for (int i=1;i<NCUs;i++) {
		memcpy(&URowsHBMBuffer[i][0],&URowsHBMBuffer[0][0],URowsHBMBufferMallocBytes);
	}

	high_resolution_clock::time_point mallocHwEnd = high_resolution_clock::now();

	*mallocTime = *mallocTime + duration_cast<duration<double>>(mallocHwEnd - mallocHwStart);

	high_resolution_clock::time_point h2dTimeStart = high_resolution_clock::now();

	for (int i=0;i<NCUs;i++) {
		narg = 0;
		OCL_CHECK(err, SURowsOut[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, UrowsOutMallocBytes,&UrowsOut[i][0], &err));
		OCL_CHECK(err, SLRowsOut[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, LrowsOutMallocBytes,&LrowsOut[i][0], &err));

		OCL_CHECK(err, SURowsHBMBuffer[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, URowsHBMBufferMallocBytes,&URowsHBMBuffer[i][0], &err));
		OCL_CHECK(err, SURowsHBMBuffer2[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, URowsHBMBufferMallocBytes,&URowsHBMBuffer2[i][0], &err));

		OCL_CHECK(err, err = krnls[i].setArg(narg++, N));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SURowsOut[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SLRowsOut[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SURowsHBMBuffer[i]));
		OCL_CHECK(err, err = krnls[i].setArg(narg++, SURowsHBMBuffer2[i]));

		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({SURowsHBMBuffer[i]}, 0));
	}

	q.finish();

	high_resolution_clock::time_point h2dTimeEnd = high_resolution_clock::now();

	*h2dTime = *h2dTime + duration_cast<duration<double>>(h2dTimeEnd - h2dTimeStart);

	printf("LU: Launching kernels..\n");
	// Launch the Kernel
	high_resolution_clock::time_point hwStart = high_resolution_clock::now();

	for (int i=0;i<NCUs;i++) {
		OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
	}

	q.finish();

	high_resolution_clock::time_point hwEnd = high_resolution_clock::now();
	printf("LU: Kernels done!\n");

	*krnlTime = *krnlTime + duration_cast<duration<double>>(hwEnd - hwStart);

	printf("LU: copying results back to host memory..\n");

	high_resolution_clock::time_point d2hTimeStart = high_resolution_clock::now();

	for (int i=0;i<NCUs;i++) {
		err = q.enqueueMigrateMemObjects({SURowsOut[i],SLRowsOut[i]}, CL_MIGRATE_MEM_OBJECT_HOST);
	}

	q.finish();

	high_resolution_clock::time_point d2hTimeEnd = high_resolution_clock::now();

	printf("LU: copy done!\n");

	*h2dTime = *h2dTime + duration_cast<duration<double>>(d2hTimeEnd - d2hTimeStart);

	//printMatrix1d(LrowsOut,"LrowsOut",LrowsOutSize);

	high_resolution_clock::time_point memCpyHwStart2 = high_resolution_clock::now();

	//clear the entire U matrix, except its 1st row
	memset(&U[N],0,N*(N-1)*sizeof(float));

	//printPaddedMatrix1d(UrowsOut[0],"UrowsOut[0]",UrowsOutSize,paddedRowWidth);

	//copy results back from UrowsOut to U
	int tmpNextUrowOutStartIdx = 0, tmpPaddedRowWidth = 0;
	for (int r=0;r<N-1;r++) {
		tmpPaddedRowWidth = roundUpToMultiple(N-1-r,luVdt);
		memcpy(&U[(r+1)*N+r+1],&UrowsOut[0][tmpNextUrowOutStartIdx],(N-r-1)*sizeof(float));
		tmpNextUrowOutStartIdx = tmpNextUrowOutStartIdx + tmpPaddedRowWidth;
	}

	//copy back data from LrowsOut to L
	//printf("LU: copy back data from LrowsOut to L..\n");

	int tmpColElemNum = 0, tmpNextStart = 0, tmpNextEnd = 0;
	for (int c=0;c<N-1;c++) {
		tmpColElemNum = roundUpToMultiple(N-c-1,luVdt);
		tmpNextEnd = tmpNextStart+N-c-1;
		//printf("LU: c = %d, tmpColElemNum = %d, tmpNextStart = %d, tmpNextEnd = %d\n",c,tmpColElemNum,tmpNextStart,tmpNextEnd);
		for (int lr=tmpNextStart,i=0;lr<tmpNextEnd;lr++,i++) {
			//printf("LU: lr start = %d, i = %d, lr end = %d, LrowsOut[0][%d] = %08f\n",lr,i,tmpNextEnd,lr,LrowsOut[0][lr]);
			memcpy(&L[(c+1)*N+c+i*N],&LrowsOut[0][lr],sizeof(float));
		}
		tmpNextStart = tmpNextStart + tmpColElemNum;
	}

	high_resolution_clock::time_point memCpyHwEnd2 = high_resolution_clock::now();

	*memcpyTime = *memcpyTime + duration_cast<duration<double>>(memCpyHwEnd2 - memCpyHwStart2);

	//printMatrix(L,"L_hw",N);
	//printMatrix(U,"U_hw",N);

	for (int i=0;i<NCUs;i++) {
		free(UrowsOut[i]);
		free(LrowsOut[i]);
		free(URowsHBMBuffer[i]);
		free(URowsHBMBuffer2[i]);
	}

	//NOTE: sometimes it returns "corrupted double-linked list"
	//for (int i=0;i<NCUs;i++) {
	//	clReleaseKernel(krnls[i].get());
	//}

	return true;
}


