#include "oops.hpp"

#include <unistd.h>

//----------------------------------------------------------------------------------------

// Global variable storing device info
cl_interface program_interface;

//----------------------------------------------------------------------------------------

// function that programs device with xclbin file
void program_device(const char *arg1)
{

   cl_int err;

   //programm_device(arg1,&err,&context, &q, &krnl,krnl_name);
   auto binaryFile = arg1;

   // OPENCL HOST CODE AREA START
   auto devices = xcl::get_xil_devices();

   // Create Program and Kernel
   auto fileBuf = xcl::read_binary_file(binaryFile);
   cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
   bool valid_device = false;
   for (unsigned int i = 0; i < devices.size(); i++) {
       auto device = devices[i];
       // int res = device.getInfo<CL_DEVICE_NAME>().compare("xilinx_u280_xdma_201920_3");
       // int res = device.getInfo<CL_DEVICE_NAME>().compare("xilinx_u55c_gen3x16_xdma_base_3");
      //  int res = device.getInfo<CL_DEVICE_NAME>().compare("xilinx_u55c_gen3x16_xdma_3_202210_1");
      //  if(res==0){
         // Creating Context and Command Queue for selected Device
         OCL_CHECK(err, program_interface.context = cl::Context(device, nullptr, nullptr, nullptr, &err));
         OCL_CHECK(err, program_interface.q = cl::CommandQueue(program_interface.context, device,
                                                               CL_QUEUE_PROFILING_ENABLE, &err));
         
         std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
         program_interface.program=cl::Program (program_interface.context, {device}, bins, nullptr, &err);
         
         if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
         } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            valid_device = true;
            break; // we break because we found a valid device
         }
      //  }
   }

   if (!valid_device) {
      std::cout << "Failed to program any device found, exit!\n";
      exit(EXIT_FAILURE);
   }
}

//----------------------------------------------------------------------------------------

// OOPS memory allocation. Allocates aligned memory based on OS page size
void *OOPS_malloc(size_t alloc_bytes){
   void *X;

   auto align_sz = sysconf(_SC_PAGESIZE);
   X= (void *)aligned_alloc(align_sz,alloc_bytes);

   return X;
}


void convert_packed_matrix_to_Hyperpacked(const char Uplo,const float *Apacked, uint32_t packedSize, float *FPGApacked, uint32_t fpgaPackedSize, uint32_t N, uint32_t VDATA_SIZE)
{
	uint32_t limit=N;

	uint32_t fp_idx = 0;
	uint32_t ap_idx = 0;
	uint32_t usefulRowElems = N;



	if ((Uplo == 'U') || (Uplo == 'u')){
		 printf("Uplo U\n");
		while (ap_idx < packedSize )
		{
			if ( (usefulRowElems%VDATA_SIZE) != 0)
			{
				while( (fp_idx%VDATA_SIZE) < VDATA_SIZE - (usefulRowElems%VDATA_SIZE)  )
				{
//					 printf("Case 1 fp_idx = %u \n", fp_idx);
					FPGApacked[fp_idx] = 0 ;
					fp_idx++;

				}
			}

			while(ap_idx < limit )
			{
//				 printf("Case 2 fp_idx = %u, ap_idx = %u, Apacked[%u] = %f \n", fp_idx, ap_idx, ap_idx, Apacked[ap_idx]);
				 FPGApacked[fp_idx] = Apacked[ap_idx];
//				FPGApacked[fp_idx] = ap_idx+1;
				fp_idx++;
				ap_idx++;

			}
			limit = limit + usefulRowElems - 1;
			usefulRowElems--;
//			 printf("limit = %u usefulRowElems = %u \n ",limit,usefulRowElems);
		}
	}
	else if ((Uplo == 'L') || (Uplo == 'l')){
       printf("Uplo L\n");
    }
    else {
		printf("Define Uplo\n");
    }

}

//The  formula comes form a mathematic analysis of the size of the upper triangular  matrix
// Is based on arithmetic progression Sv = v*(a1+av)/2
int OOPS_cmpt_hyperpacked_triang_mtrx_blks(int N, int VDATA_SIZE ){

return ( N%VDATA_SIZE ) * ( N/VDATA_SIZE + 1 ) + (VDATA_SIZE * (( N/VDATA_SIZE ) * (1 + N/VDATA_SIZE ))/2);
}

int OOPS_Hpacked_triMtx_start_idx(int N, int VDATA_SIZE, int row){
	//The rows that are not a complete set of VDATA_SIZE
	int extra_rows = row % VDATA_SIZE;
	//The number of blocks of VDATA_SIZE numbers that has the provided row
	int row_blks = ((N - row - 1)/VDATA_SIZE) + 1;

	//Arithmetic progression formula is Sv = v(a1+av)/2
	//The number of elements between a1 and av. Practically it means how many block of VDATA size row are in the matrix expept the last one
	// that our current row are in.
	int v = row/VDATA_SIZE;

	//The number of blocks of VDATA_SIZE numbers has the previous VDATA_SIZEnth of rows
	int a1 = row_blks+1;
	//The number of The number of blocks of VDATA_SIZE numbers has the first VDATA_SIZEnth of
	int av = N/VDATA_SIZE;

	int idx = extra_rows*row_blks + (VDATA_SIZE * v * ( a1 + av ) / 2);

	return idx;
}

int OOPS_Hpacked_triMtx_end_idx(int N, int VDATA_SIZE, int row){
	int start_row_idx = OOPS_Hpacked_triMtx_start_idx( N,  VDATA_SIZE,  row);
	//The number of blocks of VDATA_SIZE numbers that has the provided row
	int row_blks = ((N - row - 1)/VDATA_SIZE) + 1;

	int end_row_idx = start_row_idx + row_blks - 1;
	return end_row_idx;
}
