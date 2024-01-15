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

void OOPS_gbmv(const char TransA, const int M, const int N, const int KL, const int KU, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY){

        cl_int err;

        int CUs=10;
        int row_size= M;
        int col_size =N;
        int N_cols = ((KU+KL)/16 + 1)*16;

        size_t input_vector_size_bytes = sizeof(float) *N_cols*(row_size/CUs);
        size_t vector_size_bytes = sizeof(float) * (row_size/CUs)*incY;
        size_t matrix_size_bytes = sizeof(float) *N_cols*(row_size/CUs);


        cl::Kernel krnl;
        OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_gbmv", &err));

        // Allocate Buffer in Global Memory
        OCL_CHECK(err, cl::Buffer Sx1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) X, &err));
        OCL_CHECK(err, cl::Buffer Sx2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(2*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(3*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(4*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(5*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(6*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(7*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(8*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sx10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) &X[(9*M/CUs)*N_cols], &err));

        OCL_CHECK(err, cl::Buffer Sy1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) Y, &err));
        OCL_CHECK(err, cl::Buffer Sy2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[2*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[3*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[4*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[5*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[6*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[7*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[8*M/CUs], &err));
        OCL_CHECK(err, cl::Buffer Sy10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, vector_size_bytes, (void *) &Y[9*M/CUs], &err));
        
	OCL_CHECK(err, cl::Buffer Sa1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
        OCL_CHECK(err, cl::Buffer Sa2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(2*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(3*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(4*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(5*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(6*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(7*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(8*M/CUs)*N_cols], &err));
        OCL_CHECK(err, cl::Buffer Sa10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(9*M/CUs)*N_cols], &err));


        // Set the Kernel Arguments
        int narg = 0;
        OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
        OCL_CHECK(err, err = krnl.setArg(narg++, M));
        OCL_CHECK(err, err = krnl.setArg(narg++, N));
        OCL_CHECK(err, err = krnl.setArg(narg++, KL));
        OCL_CHECK(err, err = krnl.setArg(narg++, KU));
        OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
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
        OCL_CHECK(err, err = krnl.setArg(narg++, lda));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx1));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx2));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx3));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx4));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx5));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx6));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx7));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx8));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx9));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sx10));
        OCL_CHECK(err, err = krnl.setArg(narg++, incX));
        OCL_CHECK(err, err = krnl.setArg(narg++, beta));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy1));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy2));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy3));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy4));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy5));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy6));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy7));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy8));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy9));
        OCL_CHECK(err, err = krnl.setArg(narg++, Sy10));
        OCL_CHECK(err, err = krnl.setArg(narg++, incY));
        
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx1}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx2}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx3}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx4}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx5}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx6}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx7}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx8}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx9}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx10}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa1}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa2}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa3}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa4}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa5}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa6}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa7}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa8}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa9}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa10}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy1}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy2}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy3}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy4}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy5}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy6}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy7}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy8}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy9}, 0 /* 0 means from host*/));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy10}, 0 /* 0 means from host*/));

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
        //double W_avg_fpga = *w_fpga_ret;
        //double J_estimated_fpga = W_avg_fpga * duration.count()*1.0/1e6;
        //printf("J_estimated_fpga = %lf\tW_avg_fpga = %lf\n", J_estimated_fpga, W_avg_fpga);
        //std::cout << "W_avg_fpga: " << W_avg_fpga << std::endl;
        /**************************************************************************************/

        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

        // Copy Result from Device Global Memory to Host Local Memory
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy1}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy2}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy3}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy4}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy5}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy6}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy7}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy8}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy9}, CL_MIGRATE_MEM_OBJECT_HOST));
        OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy10}, CL_MIGRATE_MEM_OBJECT_HOST));

        q.finish();
        clReleaseKernel(krnl.get());

}
