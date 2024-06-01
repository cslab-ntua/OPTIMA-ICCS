#!/bin/bash
#Script that deletes all extra files from the template project and return it to its initial state

#clean config_files
cd config_files
find . -type f -not \( -name 'package_xilinx_u55c_gen3x16_xdma_3_202210_1.cfg' \
 -or -name 'package_xilinx_u280_xdma_201920_3.cfg' \) -delete
 cd ../

#clean optima folder
cd optima/Emulation-SW
find . -type f -not \( -name 'makefile' -or -name 'xrt.ini' \) -delete
find . -type d -delete

cd ../Emulation-HW
find . -type f -not \( -name 'makefile' -or -name 'xrt.ini' \) -delete
find . -type d -delete

cd ../Hardware
find . -type f -not \( -name 'makefile' -or -name 'xrt.ini' \) -delete
find . -type d -delete

cd ../src
find . -type f -not \( -name 'common.hpp' -or -name 'matrix_vector_generation.hpp' \
 -or -name 'oops.cpp' -or -name 'oops.hpp' -or -name 'test_functions_set.h' -or -name 'xcl2.cpp' -or -name 'xcl2.hpp' \) -delete

cd ../../ 

#clean optima_kernels folder
cd optima_kernels/Emulation-SW
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../Emulation-HW
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../Hardware
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../src
find . -type f -not \( -name 'common.hpp' -or -name 'global.hpp' \) -delete

cd ../../ 

#clean optima_system folder
cd optima_system/Emulation-SW
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../Emulation-HW
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../Hardware
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../../ 

#clear optima_system_hw_link
cd optima_system_hw_link/Emulation-SW
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../Emulation-HW
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../Hardware
find . -type f -not -name 'makefile' -delete
find . -type d -delete

cd ../../

find . -maxdepth 1 -type f -not \( -name 'clean_project.sh' -or -name 'copy_files.sh' \
 -or -name 'make_script.sh' -or -name 'readme.txt' \
 -or -name 'run_script.sh' -or -name 'run_sw_emu.sh' \) -delete


