#include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;

//----------------------------------------------------------------------------------------
#include <pthread.h>
#include <signal.h>
bool keep_running = true;
void *run_command(void *arg) {
        // char *command = (char *) arg;
        char str1[] = "export XRT_TOOLS_NEXTGEN=True; xbutil examine -r electrical -d ";
        char *str2;
        str2 = NULL;

        // in order to parse the correct device, you have to give the DEV_ID as an environment 
        // variable in the run_script.sh, e.g. export DEV_ID="0000:0a:00.1".
        str2 = (char *) getenv("DEV_ID");
        char str3[] = " | grep '  Power  ' | cut -d ':' -f 2 | cut -d ' ' -f 2;";
        char command[strlen(str1) + strlen(str2) + strlen(str3) + 1];
        strcpy(command, str1);
        strcat(command, str2);
        strcat(command, str3);
        // printf("command = %s\n", command);

    // while (keep_running)
        //      system(command);

        int cnt = 0;
        double Watt_fpga_sum = 0;
        while (keep_running){
                char buffer[1024];
                FILE *pipe = popen(command,"r");
                if(pipe==NULL)
                        printf("Failed to open pipe to command\n");
                while(fgets(buffer, sizeof(buffer), pipe) != NULL){
                        cnt+=1;
                        // printf("%s", buffer);
                        Watt_fpga_sum+=atof(buffer);
                }
                pclose(pipe);
        }
        double Watt_fpga = Watt_fpga_sum / cnt;
        printf("finally, cnt = %d, average Watt is Watt_fpga = %lf\n", cnt, Watt_fpga);
        pthread_exit((void*) &Watt_fpga);
}

void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("Stopping repetitive execution...\n");
        keep_running = false;
    }
}

//----------------------------------------------------------------------------------------

void *OOPS_malloc(size_t alloc_bytes){
        void *X;

    auto align_sz = sysconf(_SC_PAGESIZE);
    X= (void *)aligned_alloc(align_sz,alloc_bytes);

    return X;
}

void program_device(const char *arg1){

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
           // Creating Context and Command Queue for selected Device
           OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
           OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err));

           std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
           program=cl::Program (context, {device}, bins, nullptr, &err);

           if (err != CL_SUCCESS) {
                            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
           } else {
                            std::cout << "Device[" << i << "]: program successful!\n";
                            valid_device = true;
                            break; // we break because we found a valid device
           }
        }

        if (!valid_device) {
                std::cout << "Failed to program any device found, exit!\n";
            exit(EXIT_FAILURE);
        }
}

void release_device(){

        clReleaseProgram(program.get());
        clReleaseContext(context.get());
        clReleaseCommandQueue(q.get());

}

void OOPS_tbsv(const char Uplo, const char TransA, const char Diag, const int N,
                const int K, const float  *A, const int lda, float  *X, const int incX){

        cl_int err;
        cl::Kernel krnl;

        int N_cols = (K/16 + 1)*16;

        size_t vector_size_bytes = sizeof(float)*N*incX;
        size_t matrix_size_bytes = sizeof(float)*N_cols*N;

        OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_tbsv", &err));

        // Allocate Buffer in Global Memory
        OCL_CHECK(err, cl::Buffer Sx_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, (void *) X, &err));
        OCL_CHECK(err, cl::Buffer Sx_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes, (void *) X, &err));

        OCL_CHECK(err, cl::Buffer Sa1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa11(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa12(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa13(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa14(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa15(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa16(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));

        // Set the Kernel Arguments
        int narg = 0;
        OCL_CHECK(err, err = krnl.setArg(narg++, Uplo));
        OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
        OCL_CHECK(err, err = krnl.setArg(narg++, Diag));
        OCL_CHECK(err, err = krnl.setArg(narg++, N));
        OCL_CHECK(err, err = krnl.setArg(narg++, K));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa1));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa2));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa3));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa4));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa5));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa6));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa7));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa8));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa9));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa10));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa11));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa12));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa13));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa14));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa15));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sa16));
        OCL_CHECK(err, err = krnl.setArg(narg++, lda));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_in));
        OCL_CHECK(err, err = krnl.setArg(narg++, incX));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx_out));


        // Copy input data to device global memory
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx_in}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa1}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa2}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa3}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa4}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa5}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa6}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa7}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa8}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa9}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa10}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa11}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa12}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa13}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa14}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa15}, 0));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa16}, 0));

        q.finish();

     	/**************************************************************************************/
   
       	pthread_t thread;
	signal(SIGINT, sig_handler);    
	pthread_create(&thread, NULL, run_command, (void *) NULL);
    
	/**************************************************************************************/

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	// Launch the Kernel
	OCL_CHECK(err, err = q.enqueueTask(krnl));
	q.finish();
	end = std::chrono::system_clock::now();
    
	/**************************************************************************************/
    
	/* Signal thread to stop running */
	keep_running = false;
	double *w_fpga_ret;    
	pthread_join(thread, (void **) &w_fpga_ret);
    
	/**************************************************************************************/

	std::chrono::duration<double> elapsed_seconds = end - start;    
	std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
        // Copy Result from Device Global Memory to Host Local Memory
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx_out}, CL_MIGRATE_MEM_OBJECT_HOST));

        q.finish();
        clReleaseKernel(krnl.get());

}
