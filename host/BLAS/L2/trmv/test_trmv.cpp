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

void OOPS_trmv(const char Uplo, const char TransA, const char Diag,
                 const int N, const float  *A, const int lda, float  *X, const int incX){
                cl_int err;
                ///////////////////////////////////////
                int CUs = 14;
                int values =0;
                for (int i=1; i<=N;i++){
                        values +=i;
                }

                int values_per_stream = values/CUs + 1;

                int rows_counter[CUs-1];
                for(int i=0;i<CUs-1;i++)
                        rows_counter[i]=0;

                int j=0;
                for(int i=N;i>=0;i--){
                        if(values_per_stream > i and j<CUs-1){
                                values_per_stream -= i;
                                rows_counter[j]++;
                        }
                        else{
                                values_per_stream = values/CUs+1;
                                j++;
                        }
                }

                for(int i=1;i<CUs-1;i++){
                        rows_counter[i]+=rows_counter[i-1];
                }

                for(int i=0;i<CUs-1;i++){
                        cout << rows_counter[i] << endl;
                }

                ///////////////////////////////////////

            cl::Kernel krnl;
            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_trmv", &err));
	    
	    // Allocate Buffer in Global Memory
            OCL_CHECK(err, cl::Buffer Sx1_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx2_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx3_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx4_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx5_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx6_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx7_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx8_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx9_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx10_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx11_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx12_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
                OCL_CHECK(err, cl::Buffer Sx13_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx14_in(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * N*incX, (void *) X, &err));

            OCL_CHECK(err, cl::Buffer Sa1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * rows_counter[0]*N, (void *) A, &err));
            OCL_CHECK(err, cl::Buffer Sa2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[1]-rows_counter[0])*N, (void *) &A[rows_counter[0]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa3(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[2]-rows_counter[1])*N, (void *) &A[rows_counter[1]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa4(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[3]-rows_counter[2])*N, (void *) &A[rows_counter[2]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa5(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[4]-rows_counter[3])*N, (void *) &A[rows_counter[3]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa6(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[5]-rows_counter[4])*N, (void *) &A[rows_counter[4]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa7(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[6]-rows_counter[5])*N, (void *) &A[rows_counter[5]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa8(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[7]-rows_counter[6])*N, (void *) &A[rows_counter[6]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa9(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[8]-rows_counter[7])*N, (void *) &A[rows_counter[7]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa10(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[9]-rows_counter[8])*N, (void *) &A[rows_counter[8]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa11(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[10]-rows_counter[9])*N, (void *) &A[rows_counter[9]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa12(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[11]-rows_counter[10])*N, (void *) &A[rows_counter[10]*N], &err));
                OCL_CHECK(err, cl::Buffer Sa13(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (rows_counter[12]-rows_counter[11])*N, (void *) &A[rows_counter[11]*N], &err));
            OCL_CHECK(err, cl::Buffer Sa14(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, sizeof(float) * (N-rows_counter[12])*N, (void *) &A[rows_counter[12]*N], &err));

	    OCL_CHECK(err, cl::Buffer Sx1_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * rows_counter[0]*incX, (void *) X, &err));
            OCL_CHECK(err, cl::Buffer Sx2_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[1]-rows_counter[0])*incX, (void *) &X[rows_counter[0]], &err));
            OCL_CHECK(err, cl::Buffer Sx3_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[2]-rows_counter[1])*incX, (void *) &X[rows_counter[1]], &err));
        OCL_CHECK(err, cl::Buffer Sx4_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[3]-rows_counter[2])*incX, (void *) &X[rows_counter[2]], &err));
            OCL_CHECK(err, cl::Buffer Sx5_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[4]-rows_counter[3])*incX, (void *) &X[rows_counter[3]], &err));
            OCL_CHECK(err, cl::Buffer Sx6_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[5]-rows_counter[4])*incX, (void *) &X[rows_counter[4]], &err));
            OCL_CHECK(err, cl::Buffer Sx7_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[6]-rows_counter[5])*incX, (void *) &X[rows_counter[5]], &err));
            OCL_CHECK(err, cl::Buffer Sx8_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[7]-rows_counter[6])*incX, (void *) &X[rows_counter[6]], &err));
            OCL_CHECK(err, cl::Buffer Sx9_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[8]-rows_counter[7])*incX, (void *) &X[rows_counter[7]], &err));
            OCL_CHECK(err, cl::Buffer Sx10_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[9]-rows_counter[8])*incX, (void *) &X[rows_counter[8]], &err));
            OCL_CHECK(err, cl::Buffer Sx11_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[10]-rows_counter[9])*incX, (void *) &X[rows_counter[9]], &err));
            OCL_CHECK(err, cl::Buffer Sx12_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[11]-rows_counter[10])*incX, (void *) &X[rows_counter[10]], &err));
            OCL_CHECK(err, cl::Buffer Sx13_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (rows_counter[12]-rows_counter[11])*incX, (void *) &X[rows_counter[11]], &err));
            OCL_CHECK(err, cl::Buffer Sx14_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float) * (N-rows_counter[12])*incX, (void *) &X[rows_counter[12]], &err));

	       // Set the Kernel Arguments
            int narg = 0;
            OCL_CHECK(err, err = krnl.setArg(narg++, Uplo));
            OCL_CHECK(err, err = krnl.setArg(narg++, TransA));
            OCL_CHECK(err, err = krnl.setArg(narg++, Diag));
            OCL_CHECK(err, err = krnl.setArg(narg++, N));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[0]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[1]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[2]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[3]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[4]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[5]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[6]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[7]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[8]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[9]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[10]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[11]));
            OCL_CHECK(err, err = krnl.setArg(narg++, rows_counter[12]));
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
            OCL_CHECK(err, err = krnl.setArg(narg++, lda));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx1_in));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx2_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx3_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx4_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx5_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx6_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx7_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx8_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx9_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx10_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx11_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx12_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx13_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx14_in));
            OCL_CHECK(err, err = krnl.setArg(narg++, incX));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx1_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx2_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx3_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx4_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx5_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx6_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx7_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx8_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx9_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx10_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx11_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx12_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx13_out));
            OCL_CHECK(err, err = krnl.setArg(narg++, Sx14_out));

            // Copy input data to device global memory
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx1_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx2_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx3_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx4_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx5_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx6_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx7_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx8_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx9_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx10_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx11_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx12_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx13_in}, 0));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx14_in}, 0));
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
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx1_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx2_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx3_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx4_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx5_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx6_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx7_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx8_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx9_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx10_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx11_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx12_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx13_out}, CL_MIGRATE_MEM_OBJECT_HOST));
            OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx14_out}, CL_MIGRATE_MEM_OBJECT_HOST));

            q.finish();

            clReleaseKernel(krnl.get());
}

