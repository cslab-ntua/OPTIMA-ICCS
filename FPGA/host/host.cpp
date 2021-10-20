//#include "xcl2.hpp"
#include <vector>
#include <cmath> 
unsigned int DATA_SIZE;

void OOPS_copy( const int N, const float *X, const int incX, float *Y, const int incY,char* arg1){
// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);

	    //verify
	    std::vector<float, aligned_allocator<float> > sw_results(DATA_SIZE);


	    int x_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];

	        //verify
	    	sw_results[i] = X[x_index];


	        x_index=x_index + incX;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_copy", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, output_vector_size_bytes,
	                                            Sy_host.data(), &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    y_index=0;
	    for (int i=0;i<N;i++){
	    	Y[y_index]=Sy_host[y_index];
	    	y_index=y_index+incY;
	    }
	    y_index=0;
	  //verify
	    int match = 0;
	    for (int i = 0; i < N; i++) {
	        if (Y[y_index] != sw_results[i]) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout << "i = " << i << " CPU result = " << sw_results[i]
	                      << " Device result = " << Y[y_index] << std::endl;
	            match = 1;
	            break;
	        }

	        y_index=y_index + incY;
	    }

	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

}

float OOPS_ddot( const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, char* arg1){
// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);
	    double *result_host;
	    result_host=(double*)malloc(sizeof(double));
	    *result_host=0;
	    //verify
	    double sw_result=alpha;


	    int x_index=0;
	    int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
	        Sy_host[y_index]=Y[y_index];

	        //verify
	        sw_result = sw_result+((double)(Sx_host[x_index]))*((double)(Sy_host[y_index])));


	        x_index=x_index + incX;
	        y_index=y_index + incY;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_ddot", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, output_vector_size_bytes,
	                                            Sy_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(double),
	    	                                            result_host, &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));
	    OCL_CHECK(err, err = krnl.setArg(narg++, result));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({result}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    int match =0;
	        if (*result_host != sw_result) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout<<"hw:"<<*result_host<<"        "<<"sw:"<<sw_result<<std::endl;
	            match=1;
	        }
	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	    //return (match ? EXIT_FAILURE : EXIT_SUCCESS);
	    return *result_host;

}

float OOPS_dot( const int N, const float *X, const int incX, const float *Y, const int incY, char* arg1){
// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);
	    double *result_host;
	    result_host=(double*)malloc(sizeof(double));
	    *result_host=0;
	    //verify
	    double sw_result=0;


	    int x_index=0;
	    int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
	        Sy_host[y_index]=Y[y_index];

	        //verify
	        sw_result = sw_result+(Sx_host[x_index]*Sy_host[y_index]);


	        x_index=x_index + incX;
	        y_index=y_index + incY;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_dot", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, output_vector_size_bytes,
	                                            Sy_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(double),
	    	                                            result_host, &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));
	    OCL_CHECK(err, err = krnl.setArg(narg++, result));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({result}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    int match =0;
	        if (*result_host != sw_result) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout<<"hw:"<<*result_host<<"        "<<"sw:"<<sw_result<<std::endl;
	            match=1;
	        }
	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	    //return (match ? EXIT_FAILURE : EXIT_SUCCESS);
	    return *result_host;

}

void OOPS_rot( const int N, float *X, const int incX, float *Y, const int incY, const float  c, const float  s,char* arg1){
	DATA_SIZE=N;

	cl_int err;
	cl::Context context;
	cl::CommandQueue q;
	cl::Kernel krnl;
	size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*INCX;
    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*INCY;
    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*INCX);
    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*INCY);

	//verify
    std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);
    std::vector<float, aligned_allocator<float> > y_results(DATA_SIZE);

  
	    int x_index=0;
	    int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
	        Sy_host[y_index]=Y[y_index];

	        //verify
	        x_results[i]=c*X[x_index]+s*Y[y_index];
			y_results[i]=c*Y[y_index]-s*X[x_index];


	        x_index=x_index + incX;
	        y_index=y_index + incY;
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
            OCL_CHECK(err, krnl= cl::Kernel (program, "krnl_rot", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, input_vector_size_bytes,
                                           Sx_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes,
                                            Sy_host.data(), &err));

    // Set the Kernel Arguments
    int narg = 0;
    OCL_CHECK(err, err = krnl.setArg(narg++, N));
    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
    OCL_CHECK(err, err = krnl.setArg(narg++, incY));
	OCL_CHECK(err, err = krnl.setArg(narg++, c));
    OCL_CHECK(err, err = krnl.setArg(narg++, s));

    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(krnl));

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
    //q.finish();

    // OPENCL HOST CODE AREA END
    y_index=0;
	x_index=0;
	for (int i=0;i<N;i++){
		Y[y_index]=Sy_host[y_index];
		X[x_index]=Sx_host[x_index];
		y_index=y_index+incY;
		x_index=x_index+incX;
	}

	//verify
	y_index=0;
	x_index=0;
    int match = 0;
    for (int i = 0; i < DATA_SIZE; i++) {
        if (Sy_host[y_index] != y_results[i]) {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << y_results[i]
                      << " Device result = " << Sy_host[y_index] << std::endl;
            match = 1;
            break;
        }
		if (Sx_host[x_index] != x_results[i]) {
               std::cout << "Error: Result mismatch" << std::endl;
               std::cout << "i = " << i << " CPU result = " << x_results[i]
                         << " Device result = " << Sx_host[x_index] << std::endl;
               match = 1;
               break;
           }

        y_index=y_index + incY;
		x_index=x_index + incX;
    }

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

}

void OOPS_rotm( const int N, float *X, const int incX, float *Y, const int incY, const float *P,char* arg1){
	DATA_SIZE=N;

	cl_int err;
	cl::Context context;
	cl::CommandQueue q;
	cl::Kernel krnl;
	size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*INCX;
    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*INCY;
    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*INCX);
    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*INCY);
	std::vector<float, aligned_allocator<float> > Params(5);
	//verify
    std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);
    std::vector<float, aligned_allocator<float> > y_results(DATA_SIZE);
	float sflag=P[0];
	float sh11=P[1],sh12=P[2],sh21=P[3],sh22=P[4];

  
	    int x_index=0;
	    int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
	        Sy_host[y_index]=Y[y_index];

	        //verify
			if (sflag==(float)(-1)){
	        x_results[i]=sh11*X[x_index]+sh12*Y[y_index];
			y_results[i]=sh21*Y[y_index]+sh22*X[x_index];
			}
			else if (sflag==(float)(0)){
	        x_results[i]=X[x_index]+sh12*Y[y_index];
			y_results[i]=sh21*Y[y_index]+X[x_index];
			}
			else if (sflag==(float)(1)){
	        x_results[i]=sh11X[x_index]+Y[y_index];
			y_results[i]=-Y[y_index]+sh22*X[x_index];
			}
			else{
				std::cout << "Invalid P[0] value!\n";
        		exit(EXIT_FAILURE);
			}
	        x_index=x_index + incX;
	        y_index=y_index + incY;

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
            OCL_CHECK(err, krnl= cl::Kernel (program, "krnl_rotm", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, input_vector_size_bytes,
                                           Sx_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes,
                                            Sy_host.data(), &err));
	OCL_CHECK(err, cl::Buffer Params(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 5*sizeof(float),
                                            Sy_host.data(), &err));

    // Set the Kernel Arguments
    int narg = 0;
    OCL_CHECK(err, err = krnl.setArg(narg++, N));
    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
    OCL_CHECK(err, err = krnl.setArg(narg++, incY));
	OCL_CHECK(err, err = krnl.setArg(narg++, Params));

    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Params}, 0 /* 0 means from host*/));
    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(krnl));

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, CL_MIGRATE_MEM_OBJECT_HOST));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    // OPENCL HOST CODE AREA END
    y_index=0;
	x_index=0;
	for (int i=0;i<N;i++){
		Y[y_index]=Sy_host[y_index];
		X[x_index]=Sx_host[x_index];
		y_index=y_index+incY;
		x_index=x_index+incX;
	}
	y_index=0;
	x_index=0;
    int match = 0;
    for (int i = 0; i < DATA_SIZE; i++) {
        if (Sy_host[y_index] != y_results[i]) {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << y_results[i]
                      << " Device result = " << Sy_host[y_index] << std::endl;
            match = 1;
            break;
        }
		if (Sx_host[x_index] != x_results[i]) {
               std::cout << "Error: Result mismatch" << std::endl;
               std::cout << "i = " << i << " CPU result = " << x_results[i]
                         << " Device result = " << Sx_host[x_index] << std::endl;
               match = 1;
               break;
           }

        y_index=y_index + incY;
		x_index=x_index + incX;
       
	}
           
    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
}

void OOPS_scal( const int N, const float alpha, float *X, const int incX,char* arg1){
	DATA_SIZE=N;

	cl_int err;
	cl::Context context;
	cl::CommandQueue q;
	cl::Kernel krnl;
	size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*INCX;
    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*INCY;
    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*INCX);

	//verify
    std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);
  
	    int x_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];

	        //verify
			x_results[i]=alpha*X[x_index];
			
	        x_index=x_index + incX;
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
            OCL_CHECK(err, krnl= cl::Kernel (program, "krnl_scal", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, input_vector_size_bytes,
                                           Sx_host.data(), &err));

    // Set the Kernel Arguments
    int narg = 0;
    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
    OCL_CHECK(err, err = krnl.setArg(narg++, incX));

    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(krnl));

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    // OPENCL HOST CODE AREA END
	x_index=0;
	for (int i=0;i<N;i++){
		X[x_index]=Sx_host[x_index];
		x_index=x_index+incX;
	}

	x_index=0;
    int match = 0;
    x_index=0;
       for (int i = 0; i < DATA_SIZE; i++) {
           if (Sx_host[x_index] != x_results[i]) {
               std::cout << "Error: Result mismatch" << std::endl;
               std::cout << "i = " << i << " CPU result = " << x_results[i]
                         << " Device result = " << Sx_host[x_index] << std::endl;
               match = 1;
               break;
           }

           x_index=x_index + incX;
       }

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
}

void OOPS_swap( const int N, float *X, const int incX, float *Y, const int incY,char* arg1){
// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);

	    //verify
	    std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);
	 	std::vector<float, aligned_allocator<float> > y_results(DATA_SIZE);

	    int x_index=0;
	    int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
			Sy_host[y_index]= Y[y_index];

	        //verify
	    	x_results[i] = Y[y_index];
			y_results[i]=X[x_index]


	        x_index=x_index + incX;
	        y_index=y_index + incY;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_swap", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes,
	                                            Sy_host.data(), &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({SY}, 0 /* 0 means from host*/));
	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, CL_MIGRATE_MEM_OBJECT_HOST));
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    y_index=0;
		x_index=0;
		for (int i=0;i<N;i++){
			Y[y_index]=Sy_host[y_index];
			X[x_index]=Sx_host[x_index];
			y_index=y_index+incY;
			x_index=x_index+incX;
		}
		y_index=0;
		x_index=0;
		int match = 0;
		for (int i = 0; i < DATA_SIZE; i++) {
			if (Sy_host[y_index] != y_results[i]) {
				std::cout << "Error: Result mismatch" << std::endl;
				std::cout << "i = " << i << " CPU result = " << y_results[i]
						<< " Device result = " << Sy_host[y_index] << std::endl;
				match = 1;
				break;
			}
			if (Sx_host[x_index] != x_results[i]) {
				std::cout << "Error: Result mismatch" << std::endl;
				std::cout << "i = " << i << " CPU result = " << x_results[i]
							<< " Device result = " << Sx_host[x_index] << std::endl;
				match = 1;
				break;
			}

			y_index=y_index + incY;
			x_index=x_index + incX;
		
		}
			
		std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;

}

void OOPS_axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY, char* arg1){
	// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);

	    //verify
	    std::vector<float, aligned_allocator<float> > sw_results(DATA_SIZE);


	    int x_index=0;
		int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
			Sy_host[y_index] = Y[y_index];
			
	        //verify
	    	sw_results[i] =Y[y_index]+alpha* X[x_index];


	        x_index=x_index + incX;
			y_index=y_index+ incY;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_axpy", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes,
	                                            Sy_host.data(), &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
		OCL_CHECK(err, err = krnl.setArg(narg++, alpha));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));

	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    y_index=0;
	    for (int i=0;i<N;i++){
	    	Y[y_index]=Sy_host[y_index];
	    	y_index=y_index+incY;
	    }
	    y_index=0;
	  //verify
	    int match = 0;
	    for (int i = 0; i < N; i++) {
	        if (Y[y_index] != sw_results[i]) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout << "i = " << i << " CPU result = " << sw_results[i]
	                      << " Device result = " << Y[y_index] << std::endl;
	            match = 1;
	            break;
	        }

	        y_index=y_index + incY;
	    }

	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
}

float OOPS_asum(const int N, const float *X, const int incX,char* arg1){
	// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    double *result_host;
	    result_host=(float*)malloc(sizeof(float));
	    *result_host=0;

	    //verify
	    float sw_result=0;

	    int x_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];

	        //verify
	        sw_result = sw_result+abs((Sx_host[x_index]));


	        x_index=x_index + incX;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_asum", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(double),
	    	                                            result_host, &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, result));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({result}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    int match =0;
	        if (*result_host != sw_result) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout<<"hw:"<<*result_host<<"        "<<"sw:"<<sw_result<<std::endl;
	            match=1;
	        }
	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	    return *result_host;

}

int OOPS_iamax(const int N, const float  *X, const int incX, char* arg1){
	// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    int *result_host;
	    result_host=(int*)malloc(sizeof(int));
	    *result_host=0;

	    //verify
	    int sw_result=0;
		float max_num=X[0];

	    int x_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];

	        //verify
			if(X[x_index]>max_num){
				max_num=X[x_index];
				sw_result = i);
			}
	        
	        x_index=x_index + incX;
	    }
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
		        cl::Program program(context, {device}, bins, nullptr, &err);
		        if (err != CL_SUCCESS) {
		            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		        } else {
		            std::cout << "Device[" << i << "]: program successful!\n";
		            OCL_CHECK(err, krnl= cl::Kernel (program,"krnl_iamax", &err));
		            valid_device = true;
		            break; // we break because we found a valid device
		        }
		    }
		    if (!valid_device) {
		        std::cout << "Failed to program any device found, exit!\n";
		        exit(EXIT_FAILURE);
		    }
	    // Allocate Buffer in Global Memory
	    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
	                                           Sx_host.data(), &err));
	    OCL_CHECK(err, cl::Buffer result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(int),
	    	                                            result_host, &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, result));

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    // Launch the Kernel
	    OCL_CHECK(err, err = q.enqueueTask(krnl));
	    // Copy Result from Device Global Memory to Host Local Memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({result}, CL_MIGRATE_MEM_OBJECT_HOST));
	    q.finish();

	    int match =0;
	        if (*result_host != sw_result) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout<<"hw:"<<*result_host<<"        "<<"sw:"<<sw_result<<std::endl;
	            match=1;
	        }
	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	    return *result_host;

}


void main_copy(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
	OOPS_copy( N, X, incX, Y, incY,arg1);

}

void main_ddot(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    double result;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    result=OOPS_ddot( N, alpha, X,  incX, Y, incY,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;
}

void main_dot(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    float result;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }

    result=OOPS_dot( N, X,  incX, Y, incY,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;

}

void main_rot(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float c=((float) rand()) / (float) RAND_MAX,s=((float) rand()) / (float) RAND_MAX;

    OOPS_rot( N, X, incX, Y, incY, c, s,arg1);
	
}

void main_rotm(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    float* P;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
	P= (float *)malloc(sizeof(float)*5);
    int x_index=0;
    int y_index=0;
	P[0]=((float)(1));
	//P[0]=((float)(0));
	//P[0]=((float)(-1));
	for int(i=1;i<5;i++){
		P[i]=((float) rand())/(float) RAND_MAX;
	}
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    

    OOPS_rotm( N, X, incX, Y, incY, P,arg1);
}

void main_scal(char* arg1){
	const int N=4096, incX=5;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    OOPS_scal(  N,  alpha, X,  incX,arg1);
}

void main_swap(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }

    OOPS_swap( N, X, incX, Y, incY,arg1);
}

void main_axpy(char* arg1){
	const int N=4096, incX=5, incY=3;
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    OOPS_axpy( N, alpha, X, incX, Y, incY,arg1);
	
}

void main_asum(char* arg1){
	const int N=4096, incX=5;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }

    result=OOPS_asum( N, X, incX,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;
}

void main_iamax(char* arg1){
	const int N=4096, incX=5;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
    result=OOPS_iamax( N, X, incX,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;
}



int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    //main_copy(argv[1]);
	
	main_ddot(argv[1]);
	
	//main_dot(argv[1]);
	
	//main_asum(argv[1]);
	
	//main_swap(argv[1]);
	
	//main_iamax(argv[1]);
	
	//main_axpy(argv[1]);
	
	//main_rot(argv[1]);
	
	//main_rotm(argv[1]);
	
	//main_scal(argv[1]);


  

    return 0;
    /*
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
    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, input_vector_size_bytes,
                                           Sx_in_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, output_vector_size_bytes,
                                            Sy_out_host.data(), &err));

    // Allocate Buffer in Global Memory
    OCL_CHECK(err, cl::Buffer Sx(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, input_vector_size_bytes,
                                           Sx_host.data(), &err));
    OCL_CHECK(err, cl::Buffer Sy(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, output_vector_size_bytes,
                                            Sy_host.data(), &err));

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

	*/

    // Copy input data to device global memory
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
    // Launch the Kernel
    //OCL_CHECK(err, err = q.enqueueTask(krnl_copy));

    // Copy Result from Device Global Memory to Host Local Memory
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, CL_MIGRATE_MEM_OBJECT_HOST));
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, CL_MIGRATE_MEM_OBJECT_HOST));
    //OCL_CHECK(err, err = q.enqueueMigrateMemObjects({hw}, CL_MIGRATE_MEM_OBJECT_HOST));
    //q.finish();

    // OPENCL HOST CODE AREA END
    /*
    // Compare the results of the Device to the simulation
    int match = 0;
           if (hw_result[0]!= sw_result) {
               std::cout << "Error: Result mismatch" << std::endl;
               match = 1;
           }
    y_index=0;
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
    }
    x_index=0;
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
       }

    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
    return (match ? EXIT_FAILURE : EXIT_SUCCESS);
    */
}
