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
        int CUs=8;

        std::vector<cl::Kernel> krnls(CUs);
        std::string krnl_name = "krnl_gemv";
        for (int i = 0; i < CUs; i++) {
                std::string cu_id = std::to_string(i + 1);
                std::string krnl_name_full = krnl_name + ":{" + krnl_name +"_" + cu_id + "}";
                printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
                OCL_CHECK(err, krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
        }

        std::vector<cl::Buffer> Sx(CUs);
        std::vector<cl::Buffer> Sy(CUs);
        std::vector<cl::Buffer> Sa(CUs);

        for (int i = 0; i < CUs; i++){
                    OCL_CHECK(err, Sx[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(float)*N*incX, (void *) X, &err));
                    OCL_CHECK(err, Sy[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(float)*(M/CUs)*incY, (void *) &Y[i*M/CUs], &err));
                    OCL_CHECK(err, Sa[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(float)*N*(M/CUs), (void *) &A[(i*M/CUs)*N], &err));
        }

        for (int i = 0; i <CUs; i++){

                int narg = 0;
                OCL_CHECK(err, err = krnls[i].setArg(narg++, TransA));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, M/CUs));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, N));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, alpha));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, Sa[i]));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, lda));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, Sx[i]));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, incX));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, beta));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, Sy[i]));
                OCL_CHECK(err, err = krnls[i].setArg(narg++, incY));

                OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sa[i],Sx[i],Sy[i]}, 0 /* 0 means from host */));

	}

	q.finish();

        /**************************************************************************************/
        pthread_t thread;
        signal(SIGINT, sig_handler);
        pthread_create(&thread, NULL, run_command, (void *) NULL);
        /**************************************************************************************/

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        // Launch the Kernel
        for(int i=0; i<CUs; i++){
        	OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
        }
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
        for(int i=0; i<CUs; i++){
        	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
        }
        q.finish();
        for(int i=0; i<CUs; i++){
                clReleaseKernel(krnls[i].get());
        }
}
