
#include "test_function.h"

#include <chrono>
#include <math.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip> // to use setw and setprecision

#include <omp.h>

#include "oops.hpp"

#include <pthread.h>
#include <signal.h>

using namespace std;
using namespace std::chrono;

int verbose = 1;

//----------------------------------------------------------------------------------------

void GEMM_CPU(const int nthreads, 
	const int M, const int K, const int N, 
	const ValueType alpha, const ValueType beta,
	const ValueType* A, const ValueType* B, ValueType* C)
{
	#pragma omp parallel for num_threads(nthreads) collapse(2)
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			int index_C = i * N + j;
			ValueType sum = 0;
			ValueType c_old = C[index_C];
			for (int k = 0; k < K; k++) {
				int index_A = i * K + k;
				int index_B = k * N + j;
				sum += alpha * A[index_A] * B[index_B];
			}
			C[index_C] = sum + beta * c_old;
		}
	}
}

//----------------------------------------------------------------------------------------

bool keep_running = true;
void *run_command(void *arg) {
	// char *command = (char *) arg;
	char str1[] = "export XRT_TOOLS_NEXTGEN=True; xbutil examine -r electrical -d ";
	char *str2;
	str2 = NULL;
	str2 = (char *) getenv("DEV_ID");
	char str3[] = " | grep '  Power  ' | cut -d ':' -f 2 | cut -d ' ' -f 2;";
	char command[strlen(str1) + strlen(str2) + strlen(str3) + 1];
	strcpy(command, str1);
	strcat(command, str2);
	strcat(command, str3);
	// printf("command = %s\n", command);

	// while (keep_running)
	// 	system(command);

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

void test_function(const char *xclbinFilename,
               const int Layout, const int transa, const int transb,
               const int M, const int N, const int K,
               const ValueType alpha, ValueType* __restrict__ A, const int lda,
               ValueType* __restrict__ B, const int ldb,
               const ValueType beta, ValueType* __restrict__ C_in, ValueType* __restrict__ C_out, const int ldc)
{

	std::vector<cl::Device> devices;
	cl::Device device;
	std::vector<cl::Platform> platforms;
	bool found_device = false;
	cl_int err;
	
	//initialization of b in case of empty stripes
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			C_out[i * N + j]=0;

	std::string krnl_name = "krnl_gemm";
	//traversing all Platforms To find Xilinx Platform and targeted
	//Device in Xilinx Platform
	cl::Platform::get(&platforms);
	int d_id = -1;
	for(size_t i = 0; (i < platforms.size() ) & (found_device == false) ;i++){
		cl::Platform platform = platforms[i];
		std::string platformName = platform.getInfo<CL_PLATFORM_NAME>();
		if ( platformName == "Xilinx"){
			devices.clear();
			platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
			for (unsigned int j = 0; j < devices.size(); j++) {
				device = devices[j];
				// int res = device.getInfo<CL_DEVICE_NAME>().compare("xilinx_u280_xdma_201920_3");
				// int res = device.getInfo<CL_DEVICE_NAME>().compare("xilinx_u55c_gen3x16_xdma_base_3");
				int res = device.getInfo<CL_DEVICE_NAME>().compare("xilinx_u55c_gen3x16_xdma_3_202210_1");
				if(res==0){
					d_id = j;
					devices[0] = devices[d_id];
					found_device = true;
					break;
				}
			}
		}
	}

	if (found_device == false){
		std::cout << "Error: Unable to find Target Device " 
			<< device.getInfo<CL_DEVICE_NAME>() << std::endl;
		return; 
	}

	// Creating Context and Command Queue for selected device
	std::vector<cl::Kernel> krnls_gemm(nstripe);
	cl::Context context(device);
	cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

	// Load xclbin 
	std::cout << "Loading: '" << xclbinFilename << "'\n";
	std::ifstream bin_file(xclbinFilename, std::ifstream::binary);
	bin_file.seekg (0, bin_file.end);
	unsigned nb = bin_file.tellg();
	bin_file.seekg (0, bin_file.beg);
	char *buf = new char [nb];
	bin_file.read(buf, nb);

	// Creating Program from Binary File
	cl::Program::Binaries bins;
	bins.push_back({buf,nb});
	devices.resize(1);
	cl::Program program(context, devices, bins);

	// This call will get the kernel object from program. A kernel is an 
	// OpenCL function that is executed on the FPGA.
	for (int i = 0; i < nstripe; i++) {
		std::string cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + "krnl_gemm_" + cu_id + "}";
		if(verbose)	printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls_gemm[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	} 
	if(verbose)	printf("Bye CU\n");

	std::vector<int> stripe_nrows(nstripe);
	std::vector<int> stripe_start_A(nstripe);
	std::vector<int> stripe_start_C(nstripe);
	std::vector<int> stripe_nterms_A(nstripe);
	std::vector<int> stripe_nterms_B(nstripe);
	std::vector<int> stripe_nterms_C(nstripe);

	auto start_rowbal = high_resolution_clock::now(); 

	int stripesize = M/nstripe;
	int remainder  = M%nstripe;
	for (int i = 0; i < nstripe; i++) {
		// set-up stripe info
		stripe_nterms_B[i] = K * N;
		if ( i <= remainder ) {
			stripe_nrows[i]   = stripesize + 1;
			if ( i == remainder )
				stripe_nrows[i]--;
			stripe_nterms_A[i]  = stripe_nrows[i] * K;
			stripe_nterms_C[i]  = stripe_nrows[i] * N;
			if(i>0){
				stripe_start_A[i]	= stripe_start_A[i-1] + stripe_nterms_A[i-1];
				stripe_start_C[i]	= stripe_start_C[i-1] + stripe_nterms_C[i-1];
			}
		} else {
			stripe_nrows[i]		= stripesize;
			stripe_nterms_A[i]  = stripe_nrows[i] * K;
			stripe_nterms_C[i]  = stripe_nrows[i] * N;
			if(i>0){
				stripe_start_A[i]	= stripe_start_A[i-1] + stripe_nterms_A[i-1];
				stripe_start_C[i]	= stripe_start_C[i-1] + stripe_nterms_C[i-1];
			}
		}
		printf("ROW BALANCING	 stripe = %d\tnrows = %d\tpart_A = %d (start = %d)\tpart_B = %d\tpart_C = %d (start = %d)\n", i, stripe_nrows[i], stripe_nterms_A[i], stripe_start_A[i], stripe_nterms_B[i], stripe_nterms_C[i], stripe_start_C[i]);
	}
	auto stop_rowbal = high_resolution_clock::now(); 
	auto duration_rowbal = duration_cast<microseconds>(stop_rowbal - start_rowbal);
	cout << "ROW BALANCING			" << duration_rowbal.count() << " us" << endl;

	std::vector<cl::Buffer> buffer_A(nstripe);
	std::vector<cl::Buffer> buffer_B(nstripe);
	std::vector<cl::Buffer> buffer_C_in(nstripe);
	std::vector<cl::Buffer> buffer_C_out(nstripe);

	ValueType* _A[nstripe];
	ValueType* _B[nstripe];
	ValueType* _C_in[nstripe];
	ValueType* _C_out[nstripe];
	
	/******************************** TIME IT ****************************/
	auto start_buffer_move = high_resolution_clock::now(); 
	for (int i = 0; i < nstripe; i++) {
		_A[i]= (ValueType*) OOPS_malloc((size_t)(stripe_nterms_A[i]*sizeof(ValueType)));
		memcpy(_A[i],A+stripe_start_A[i],stripe_nterms_A[i]*sizeof(ValueType));
		OCL_CHECK(err, buffer_A[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_A[i]*sizeof(ValueType), _A[i], &err));
		if(verbose)	printf("buffer_A[%d] %lf MB\n", i, stripe_nterms_A[i]*sizeof(ValueType)/(1024*1024.0));

		_B[i]= (ValueType*) OOPS_malloc((size_t)(stripe_nterms_B[i]*sizeof(ValueType)));
		memcpy(_B[i],B,stripe_nterms_B[i]*sizeof(ValueType));
		OCL_CHECK(err, buffer_B[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_B[i]*sizeof(ValueType), _B[i], &err));
		if(verbose)	printf("buffer_B[%d] %lf MB\n", i, stripe_nterms_B[i]*sizeof(ValueType)/(1024*1024.0));

		_C_in[i]= (ValueType*) OOPS_malloc((size_t)(stripe_nterms_C[i]*sizeof(ValueType)));
		memcpy(_C_in[i],C_in+stripe_start_C[i],stripe_nterms_C[i]*sizeof(ValueType));
		OCL_CHECK(err, buffer_C_in[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, stripe_nterms_C[i]*sizeof(ValueType), _C_in[i], &err));
		if(verbose)	printf("buffer_C_in[%d] %lf MB\n", i, stripe_nterms_C[i]*sizeof(ValueType)/(1024*1024.0));

		_C_out[i]= (ValueType*) OOPS_malloc((size_t)(stripe_nterms_C[i]*sizeof(ValueType)));
		OCL_CHECK(err, buffer_C_out[i] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,	stripe_nterms_C[i]*sizeof(ValueType), _C_out[i], &err));
		if(verbose)	printf("buffer_C_out[%d] %lf MB\n\n", i, stripe_nterms_C[i]*sizeof(ValueType)/(1024*1024.0));
	}
	if(verbose)	printf("Bye copy\n");
	// set the kernel Arguments
	int ret_setarg = 0;
	for (int i = 0; i < nstripe; i++) {
		int narg=0;
		ret_setarg = krnls_gemm[i].setArg(narg++,Layout);
		if(verbose & ret_setarg) printf("ret_setarg = %d (Layout)\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,transa);
		if(verbose & ret_setarg) printf("ret_setarg = %d (transa)\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,transb);
		if(verbose & ret_setarg) printf("ret_setarg = %d (transb)\n", ret_setarg);

		ret_setarg = krnls_gemm[i].setArg(narg++,stripe_nrows[i]);
		if(verbose & ret_setarg) printf("ret_setarg = %d (stripe_nrows[i])\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,N);
		if(verbose & ret_setarg) printf("ret_setarg = %d (N)\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,K);
		if(verbose & ret_setarg) printf("ret_setarg = %d (K)\n", ret_setarg);

		ret_setarg = krnls_gemm[i].setArg(narg++,alpha);
		if(verbose & ret_setarg) printf("ret_setarg = %d (alpha)\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,buffer_A[i]);
		if(verbose & ret_setarg) printf("ret_setarg = %d (buffer_A[i])\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,lda);
		if(verbose & ret_setarg) printf("ret_setarg = %d (lda)\n", ret_setarg);

		ret_setarg = krnls_gemm[i].setArg(narg++,buffer_B[i]);
		if(verbose & ret_setarg) printf("ret_setarg = %d (buffer_B[i])\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,ldb);
		if(verbose & ret_setarg) printf("ret_setarg = %d (ldb)\n", ret_setarg);

		ret_setarg = krnls_gemm[i].setArg(narg++,beta);
		if(verbose & ret_setarg) printf("ret_setarg = %d (beta)\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,buffer_C_in[i]);
		if(verbose & ret_setarg) printf("ret_setarg = %d (buffer_C_in[i])\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,buffer_C_out[i]);
		if(verbose & ret_setarg) printf("ret_setarg = %d (buffer_C_out[i])\n", ret_setarg);
		ret_setarg = krnls_gemm[i].setArg(narg++,ldc);
		if(verbose & ret_setarg) printf("ret_setarg = %d (ldc)\n", ret_setarg);

		// https://xilinx.github.io/Vitis_Accel_Examples/2019.2/html/data_transfer.html
		// After creating buffer using Host Mem Pointer, clEnqueueMigrateMemObjects can be used for immediate migration
		// of data without considering the fact that data is actually needed or not by kernel operation.
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_A[i],buffer_B[i],buffer_C_in[i],buffer_C_out[i]}, 0));
	}
	q.finish();

	auto stop_buffer_move = high_resolution_clock::now(); 
	auto duration_buffer_move = duration_cast<microseconds>(stop_buffer_move - start_buffer_move);
	cout << "BUFFER MOVE			 " << duration_buffer_move.count() << " us" << endl;
	/******************************** TIME IT ****************************/

	/***********************************/
	pthread_t thread;
	signal(SIGINT, sig_handler);
	pthread_create(&thread, NULL, run_command, (void *) NULL);
	/***********************************/

	//Launch the Kernel
	auto start = high_resolution_clock::now(); 
	for (int i = 0; i < nstripe; i++){
		q.enqueueTask(krnls_gemm[i]);
	}
	q.finish();
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<microseconds>(stop - start);  
	cout << "Task FPGA time in us: " << duration.count() << endl;

	/***********************************/
	/* Signal thread to stop running */
	keep_running = false;
	double *w_fpga_ret;
	pthread_join(thread, (void **) &w_fpga_ret);
	double W_avg_fpga = *w_fpga_ret;
	double J_estimated_fpga = W_avg_fpga * duration.count()*1.0/1e6;
	printf("J_estimated_fpga = %lf\tW_avg_fpga = %lf\n", J_estimated_fpga, W_avg_fpga);
	/***********************************/

	//////////////////////////////////////////////////////////////////////////////////////////
	// The result of the previous kernel execution will need to be retrieved in
	// order to view the results. This call will transfer the data from FPGA to
	// source_results vector
	for (int i = 0; i < nstripe; i++){
		q.enqueueMigrateMemObjects({buffer_C_out[i]},1);
	}
	q.finish();

	for (int i = 0; i < nstripe; i++){
		memcpy(C_out+stripe_start_C[i],_C_out[i], stripe_nrows[i]*N*sizeof(ValueType));
	}
	//////////////////////////////////////////////////////////////////////////////////////////

	if(M<=1024){
		ValueType* C_cpu;
		C_cpu = (ValueType*) malloc(M * N * sizeof(ValueType));
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				C_cpu[i * N + j] = C_in[i * N + j];

		int nthreads = omp_get_max_threads();
		auto start_cpu = high_resolution_clock::now();
		GEMM_CPU(nthreads, M, K, N, alpha, beta, A, B, C_cpu);
		auto stop_cpu = high_resolution_clock::now(); 
		auto duration_cpu = duration_cast<microseconds>(stop_cpu - start_cpu);
		std::cout << "Task CPU time in us: " << duration_cpu.count() << endl;
		
		int flag=0;
		for (int i = 0; i < M*N; i++) {
			ValueType diff = fabs(C_out[i]-C_cpu[i]);
			if(diff > 1e-4 || diff != diff) { // if ValueType == float, need to have different threshold... leave it for now
				printf("Ooops (%i,%i) %.7f %.7f\n", i/N, i%N, C_out[i], C_cpu[i]);
				flag=1;
				break;
			}
			// else
			// 	printf("Ooops RIGHT (%i,%i) %.7f %.7f\n", i/N, i%N, C_out[i], C_cpu[i]);
		}
		free(C_cpu);
		if(flag==0) printf("Passed!\n");
	}

	// duration is in microseconds
	double gflops = (2*M*0.000000001)/(duration.count()*0.000001)*N*K;
	double gflops_per_watt = gflops / W_avg_fpga;
	std::cout << "GEMM " << M << "x" << K << "x" << N << " : GFLOPs=" << gflops << " GFLOPs/W=" << gflops_per_watt << "\n";
}
