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

void OOPS_gemv(const char TransA, const int M, const int N, const float alpha, const float *A, const int lda,
                 const float  *X, const int incX, const float beta, float  *Y, const int incY){

            cl_int err;
            int row_size= M;
            int col_size =N;

            int CUs=14;

            if(TransA=='C'|| TransA=='T'){
                    row_size =N;
                    col_size =M;
            }

            size_t input_vector_size_bytes = sizeof(float) * col_size*incX;
            size_t output_vector_size_bytes = sizeof(float) * (row_size/CUs)*incY;
        size_t matrix_size_bytes = sizeof(float) *N*(M/CUs);

            cl::Kernel krnl;

            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_gemv", &err));

	                // Allocate Buffer in Global Memory
            OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes, (void *) X, &err));

            OCL_CHECK(err, cl::Buffer Sy_1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) Y, &err));
            OCL_CHECK(err, cl::Buffer Sy_2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[2*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[3*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[4*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[5*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[6*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[7*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[8*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[9*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_11(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[10*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_12(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[11*M/CUs], &err));
                OCL_CHECK(err, cl::Buffer Sy_13(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[12*M/CUs], &err));
            OCL_CHECK(err, cl::Buffer Sy_14(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes, (void *) &Y[13*M/CUs], &err));

            OCL_CHECK(err, cl::Buffer Sa_1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) A, &err));
            OCL_CHECK(err, cl::Buffer Sa_2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(2*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(3*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(4*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(5*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(6*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(7*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(8*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(9*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_11(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(10*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_12(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(11*M/CUs)*N], &err));
                OCL_CHECK(err, cl::Buffer Sa_13(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(12*M/CUs)*N], &err));
            OCL_CHECK(err, cl::Buffer Sa_14(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes, (void *) &A[(13*M/CUs)*N], &err));

	    // Set the Kernel Arguments
            int narg = 0;
            OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
            OCL_CHECK(err, err = krnl.setArg(narg++, M));
            OCL_CHECK(err, err = krnl.setArg(narg++, N));
            OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_1));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_2));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_3));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_4));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_5));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_6));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_7));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_8));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_9));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_10));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_11));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_12));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_13));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sa_14));
            OCL_CHECK(err, err = krnl.setArg(narg++, lda));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
            OCL_CHECK(err, err = krnl.setArg(narg++, incX));
            OCL_CHECK(err, err = krnl.setArg(narg++, beta));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_1));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_2));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_3));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_4));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_5));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_6));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_7));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_8));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_9));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_10));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_11));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_12));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_13));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sy_14));
            OCL_CHECK(err, err = krnl.setArg(narg++, incY));

	    // Copy input data to device global memory
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_1}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_2}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_3}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_4}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_5}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_6}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_7}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_8}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_9}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_10}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_11}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_12}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_13}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa_14}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_1}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_2}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_3}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_4}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_5}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_6}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_7}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_8}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_9}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_10}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_11}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_12}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_13}, 0 /* 0 means from host*/));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_14}, 0 /* 0 means from host*/));

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
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_1}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_2}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_3}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_4}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_5}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_6}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_7}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_8}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_9}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_10}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_11}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_12}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_13}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy_14}, CL_MIGRATE_MEM_OBJECT_HOST));

            q.finish();
            clReleaseKernel(krnl.get());

}
