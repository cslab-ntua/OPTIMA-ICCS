
#include "xcl2.hpp"
#include <vector>

#define DATA_SIZE 4096
#define INCX 5
#define INCY 3
#define SA 5

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    auto binaryFile = argv[1];

    // Allocate Memory in Host Memory
    cl_int err;
    cl::Context context;
    cl::CommandQueue q;
    cl::Kernel krnl_copy;


    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*INCX;
    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*INCY;
    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*INCX);
    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*INCY);
    std::vector<float, aligned_allocator<float> > hw_result(1);
    //std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);
    //std::vector<float, aligned_allocator<float> > y_results(DATA_SIZE);
    float sw_result=0 ;
    int x_index=0;
    int y_index=0;

    // Create the test data and Software Result
    for (int i = 0; i < DATA_SIZE; i++) {
        Sx_host[x_index] = ((float) rand()) / (float) RAND_MAX;
        Sy_host[y_index] = ((float) rand()) / (float) RAND_MAX;
        //source_sw_results[i] = SA*Sx_in_host[x_index];
        //x_results[i] = Sy_host[y_index];
        //y_results[i] = Sx_host[x_index];
        sw_result=sw_result+Sx_host[x_index]*Sy_host[y_index];

        x_index=x_index + INCX;

        y_index=y_index + INCY;
    }

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
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            //OCL_CHECK(err, krnl_copy= cl::Kernel (program, "krnl_copy", &err));
            OCL_CHECK(err, krnl_copy= cl::Kernel (program, "krnl_dot", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    /*OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
                                           Sx_in_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, output_vector_size_bytes,
                                            Sy_out_host.data(), &err));
	*/
    // Allocate Buffer in Global Memory
    /*OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, input_vector_size_bytes,
                                           Sx_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes,
                                            Sy_host.data(), &err));
    */
    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
                                               Sx_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, output_vector_size_bytes,
                                                Sy_host.data(), &err));
    OCL_CHECK(err, cl::Buffer hw(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float),
                                                   hw_result.data(), &err));

    int N = DATA_SIZE;
    int k=INCX;
    int l=INCY;
    // Set the Kernel Arguments
    int narg = 0;
    OCL_CHECK(err, err = krnl_copy.setArg(narg++, N));
    OCL_CHECK(err, err = krnl_copy.setArg(narg++, Sx));
    OCL_CHECK(err, err = krnl_copy.setArg(narg++, k));
    OCL_CHECK(err, err = krnl_copy.setArg(narg++, Sy));
    OCL_CHECK(err, err = krnl_copy.setArg(narg++, l));

    //float number=SA;
    //OCL_CHECK(err, err = krnl_copy.setArg(narg++, number));



    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(krnl_copy));

    // Copy Result from Device Global Memory to Host Local Memory
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, CL_MIGRATE_MEM_OBJECT_HOST));
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({hw}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    // OPENCL HOST CODE AREA END

    // Compare the results of the Device to the simulation
    int match = 0;
           if (hw_result[0]!= sw_result) {
               std::cout << "Error: Result mismatch" << std::endl;
               match = 1;
           }
   /* y_index=0;
    int match = 0;
    for (int i = 0; i < DATA_SIZE; i++) {
        if (Sy_host[y_index] != y_results[i]) {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << y_results[i]
                      << " Device result = " << Sy_host[y_index] << std::endl;
            match = 1;
            break;
        }

        y_index=y_index + INCY;
    }*/
    /*x_index=0;
       match = 0;
       for (int i = 0; i < DATA_SIZE; i++) {
           if (Sx_host[x_index] != x_results[i]) {
               std::cout << "Error: Result mismatch" << std::endl;
               std::cout << "i = " << i << " CPU result = " << x_results[i]
                         << " Device result = " << Sx_host[x_index] << std::endl;
               match = 1;
               break;
           }

           x_index=x_index + INCX;
       }*/

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
    return (match ? EXIT_FAILURE : EXIT_SUCCESS);
}
