Contents of project template
make_script.sh: selection of which "Build" and how many Compute Units are used
run_script.sh: selection of "Build" and number of Compute Units to be used

Each directory contains "Emulation-SW" and "Hardware" subdirectories.

* optima *
Code executed in CPU
	main: generation of workloads
	oops, xcl2: necessary functions for programming device
	test_function: preparation for kernel execution (buffers, etc.) and collection of final results

For the Alveo-U55C, there needs to be a change in "oops" and "test_function" files, when switching Build from "Emulation-SW" to "Hardware".
Emulation_SW : "xilinx_u55c_gen3x16_xdma_3_202210_1"
Hardware     : "xilinx_u55c_gen3x16_xdma_base_3"

what needs to change for testing other kernels: change workload generation process in "main.cpp", arguments that are fed to "test_function" and "krnl_gemm" in "test_function.cpp".


* optima_kernels *
Code executed in FPGA
	krnl_gemm: code for one Compute Unit

what needs to change for testing other kernels: kernel code in "krnl_gemm.cpp" and all "krnl_gemm" references in "makefile" of build directories.


* optima_system *
what needs to change for testing other kernels: nothing (fix "other configuration files")


* optima_system_hw_link *
what needs to change for testing other kernels: nothing (fix "other configuration files")


* other configuration files *
config_CU<num_of_CUs>_<platform>.cfg files: connectivity between kernel ports and memory banks, different for each Compute Unit and platform configuration 
krnl_<kernel_name>_<platform>.cfg and package_<platform>.cfg: change kernel names and platform used in these files
