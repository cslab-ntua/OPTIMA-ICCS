#include "oops.hpp"
unsigned int DATA_SIZE;
const char* arg1="/home/users/egraf/vitis_workspace/tewsd_system/Emulation-HW/binary_container_1.xclbin";
void OOPS_copy( const int N, const float *X, const int incX, float *Y, const int incY){
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

	    int y_index=0;
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

float OOPS_ddot( const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY){
// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    size_t result_size = sizeof(float);
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);
	    float *result_host =NULL;
	    //std::cout<<*result_host <<" should be 0"<<std::endl;
	    posix_memalign((void **)&result_host, 4096, sizeof(float));
	    std::cout<< *result_host <<" should be 0"<<std::endl;
	    *result_host =0;
	    std::cout<< *result_host <<" should be 0"<<std::endl;

	    //verify
	    double sw_result=((float)alpha);


	    int x_index=0;
	    int y_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];
	        Sy_host[y_index]=Y[y_index];

	        //verify
	        sw_result = sw_result+((Sx_host[x_index])*(Sy_host[y_index]));


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
	    OCL_CHECK(err, cl::Buffer result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, result_size,
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
	        if (*result_host != (float)sw_result) {
	            std::cout << "Error: Result mismatch" << std::endl;
	            std::cout<<"hw:"<<*result_host<<"        "<<"sw:"<<sw_result<<std::endl;
	            match=1;
	        }
	    std::cout << "TEST " << (match ? "FAILED" : "PASSED") << std::endl;
	    //return (match ? EXIT_FAILURE : EXIT_SUCCESS);
	    return *result_host;

}

float OOPS_dot( const int N, const float *X, const int incX, const float *Y, const int incY){
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
	    float *result_host =NULL;
	  	//std::cout<<*result_host <<" should be 0"<<std::endl;
	  	posix_memalign((void **)&result_host, 4096, sizeof(float));
	    //verify
	    float sw_result=0;


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
	    OCL_CHECK(err, cl::Buffer result(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, sizeof(float),
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

void OOPS_rot( const int N, float *X, const int incX, float *Y, const int incY, const float  c, const float  s){
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
	        Sy_host[y_index]=Y[y_index];

	        //verify
	        x_results[i]=c*X[x_index]+s*Y[y_index];
			y_results[i]=c*Y[y_index]-s*X[x_index];


	        x_index=x_index + incX;
	        y_index=y_index + incY;
	    }

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
    for (int i = 0; i < N; i++) {
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

void OOPS_rotm( const int N, float *X, const int incX, float *Y, const int incY, const float *P){
	DATA_SIZE=N;

		cl_int err;
		cl::Context context;
		cl::CommandQueue q;
		cl::Kernel krnl;
		size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    size_t output_vector_size_bytes = sizeof(float) * DATA_SIZE*incY;
	    size_t params_size_bytes = sizeof(float) * 5;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    std::vector<float, aligned_allocator<float> > Sy_host(DATA_SIZE*incY);
	    std::vector<float, aligned_allocator<float> > Params_host(DATA_SIZE*incY);


		//verify
	    std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);
	    std::vector<float, aligned_allocator<float> > y_results(DATA_SIZE);


		    int x_index=0;
		    int y_index=0;
		    float sflag=P[0];
		    float sh[2][2];

		    for(int i=0;i<5;i++){
		    		Params_host[i]=P[i];
		    	}
		    if (sflag==-1){
		    	sh[0][0]=P[1];
		    	sh[1][0]=P[2];
		    	sh[0][1]=P[3];
		    	sh[1][1]=P[4];
		    }

		    else if(sflag==1){
		    		sh[0][1]=(float)1;
		    		sh[1][0]=(float)(-1);
		    	}
		    	else if(sflag==0){
		    			sh[0][0]=(float)1;
		    			sh[0][0]=(float)1;
		    	}
		    	else{
		    		std::cout << "Invalid P[0] value!\n";
		    		exit(EXIT_FAILURE);
		    	}

		    for (int i = 0; i < N; i++) {
		        Sx_host[x_index] = X[x_index];
		        Sy_host[y_index]=Y[y_index];

		        //verify
		        x_results[i]=sh[0][0]*X[x_index]+sh[0][1]*Y[y_index];
				y_results[i]=sh[1][0]*Y[y_index]+sh[1][1]*X[x_index];


		        x_index=x_index + incX;
		        y_index=y_index + incY;
		    }

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
	    OCL_CHECK(err, cl::Buffer Params(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, params_size_bytes,
	    	                                            Params_host.data(), &err));

	    // Set the Kernel Arguments
	    int narg = 0;
	    OCL_CHECK(err, err = krnl.setArg(narg++, N));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sx));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incX));
	    OCL_CHECK(err, err = krnl.setArg(narg++, Sy));
	    OCL_CHECK(err, err = krnl.setArg(narg++, incY));
		OCL_CHECK(err, err = krnl.setArg(narg++, Params));
		std::cout<<" added kernel arguments"<<std::endl;

	    // Copy input data to device global memory
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sx}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
	    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Params}, 0 /* 0 means from host*/));
	    std::cout<< "copied data to device"<<std::endl;
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

		//verify
		y_index=0;
		x_index=0;
	    int match = 0;
	    for (int i = 0; i < N; i++) {
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

void OOPS_scal( const int N, const float alpha, float *X, const int incX){
	DATA_SIZE=N;

	cl_int err;
	cl::Context context;
	cl::CommandQueue q;
	cl::Kernel krnl;
	size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);

	//verify
    std::vector<float, aligned_allocator<float> > x_results(DATA_SIZE);

	    int x_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];

	        //verify
			x_results[i]=alpha*X[x_index];

	        x_index=x_index + incX;
	    }

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
       for (int i = 0; i < N; i++) {
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

void OOPS_swap( const int N, float *X, const int incX, float *Y, const int incY){
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
			y_results[i]=X[x_index];


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
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({Sy}, 0 /* 0 means from host*/));
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
		for (int i = 0; i < N; i++) {
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

void OOPS_axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY){
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

void OOPS_xpay(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY){
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
	    	sw_results[i] =alpha* Y[y_index]+X[x_index];


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

float OOPS_asum(const int N, const float *X, const int incX){
	// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    float *result_host =NULL;
		//std::cout<<*result_host <<" should be 0"<<std::endl;
		posix_memalign((void **)&result_host, 4096, sizeof(float));

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

int OOPS_iamax(const int N, const float  *X, const int incX){
	// set DATA_SIZE =N;
	DATA_SIZE=N;

	   cl_int err;
	   cl::Context context;
	   cl::CommandQueue q;
	   cl::Kernel krnl;

	    size_t input_vector_size_bytes = sizeof(float) * DATA_SIZE*incX;
	    std::vector<float, aligned_allocator<float> > Sx_host(DATA_SIZE*incX);
	    int *result_host =NULL;
		//std::cout<<*result_host <<" should be 0"<<std::endl;
		posix_memalign((void **)&result_host, 4096, sizeof(int));

	    //verify
	    int sw_result=0;
		float max_num=X[0];

	    int x_index=0;
	    for (int i = 0; i < N; i++) {
	        Sx_host[x_index] = X[x_index];

	        //verify
			if(X[x_index]>max_num){
				max_num=X[x_index];
				sw_result = i;
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
