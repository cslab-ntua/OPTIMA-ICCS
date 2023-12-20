# export TARGET=Hardware
export TARGET=Emulation-SW
export CU=8
export PROJECT_NAME=optima

# These environmental variables are set up for "magneto"
export XILINX_XRT=/opt/xilinx/xrt
export XILINX_VIVADO=/opt/Xilinx/Vivado/2022.1/
export XILINX_VITIS=/opt/Xilinx/Vitis/2022.1/
export XILINX_VITIS_HLS=/opt/Xilinx/Vitis_HLS/2022.1/
# export VITIS_PLATFORM=xilinx_u280_xdma_201920_3
export VITIS_PLATFORM=xilinx_u55c_gen3x16_xdma_3_202210_1

echo -e "Welcome to build of project : ${PROJECT_NAME}, for platform ${VITIS_PLATFORM} with ${CU} Compute Units and with active build configuration : ${TARGET}"

rm -rf *summary *.csv
cd ${PROJECT_NAME}/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building host side"
make clean PROJECT_NAME=${PROJECT_NAME}
make all -j20 CU=${CU} PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM}

cd ../../${PROJECT_NAME}_kernels/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building fpga side (hw function)"
make clean PROJECT_NAME=${PROJECT_NAME}
make all PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM}

cd ../../${PROJECT_NAME}_system_hw_link/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building hw link"
make clean PROJECT_NAME=${PROJECT_NAME}
make all CU=${CU} PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM}

cd ../../${PROJECT_NAME}_system/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building final wrap-up of system"
make clean PROJECT_NAME=${PROJECT_NAME}
make all CU=${CU} PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM}

cd ../../
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo -e "FINISHED"
