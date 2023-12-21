#include "oops.hpp"
#include <chrono>
#include <ctime>
#include <ratio>
using namespace std::chrono;
std::chrono::time_point<std::chrono::system_clock> start, ending;
// common functions

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
