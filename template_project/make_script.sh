#!/bin/bash
# export TARGET=Hardware
if [ $# -lt 2 ]; then
  echo "Expected at least 2 arguments but found $#."
  echo "The absolute necessary arguments are -k=* or --kernel=* and -c=*|--cus=*"

  exit 1
fi

#default values for the variables
CU_NUM=16
PLATFORM=u55
TARGET=Emulation-SW
BACKCUP=FALSE
export PROJECT_NAME=optima
export XCL_EMULATION_MODE=sw_emu


for i in "$@"; do
      case $i in
            -k=*|--kernel=*)
                TARGET_KRNL="${i#*=}" #keeps the value after the '='
                shift # past argument=value
                ;;
            -c=*|--cus=*)
                CU_NUM="${i#*=}"
                shift # past argument=value
                ;;
            -p=*|--platform=*)
                PLATFORM=${i#*=}
                if [[ "$PLATFORM" != "u55" && "$PLATFORM" != "u280" ]]
                then
                    echo "Error: The specified platform ${PLATFORM} is not suported!"
                    exit 1
                fi
                shift  # past argument=value
                ;;
            -t=*|--target=*)
                TARGET=${i#*=}
                if [[ "$TARGET" != "Emulation-SW" || "$TARGET" != "Emulation-HW" || "$TARGET" != "Hardware" ]]
                then
                    echo "Error: The specified compilation target ${TARGET} is not suported!"
                    exit 1
                fi
                shift  # past argument=value
                ;;
            -b|--backup)
                BACKCUP=FALSE
                shift # past argument with no value
                ;;
            -*|--*)
                echo "Unknown option $i"
                exit 1
                ;;
            *)
                echo "Unknown option $i"
                exit 1
                  ;;
            esac
done


# These environmental variables are set up for "magneto"
export XILINX_XRT=/opt/xilinx/xrt
export XILINX_VIVADO=/opt/Xilinx/Vivado/2022.1/
export XILINX_VITIS=/opt/Xilinx/Vitis/2022.1/
export XILINX_VITIS_HLS=/opt/Xilinx/Vitis_HLS/2022.1/

if [[ "$PLATFORM" == "u55" ]]
then
    export VITIS_PLATFORM=xilinx_u55c_gen3x16_xdma_3_202210_1
else
    export VITIS_PLATFORM=xilinx_u280_xdma_201920_3
fi

echo -e "Welcome to build of project : ${PROJECT_NAME}, for platform ${VITIS_PLATFORM} with ${CU_NUM} Compute Units and with active build configuration : ${TARGET}"

KRNL_NAME=krnl_${TARGET_KRNL}

KRNL_COMPILE_CFG=$(find . -type f -name "${KRNL_NAME}-compile_${VITIS_PLATFORM}.cfg")
echo ${KRNL_COMPILE_CFG}
KRNL_COMPILE_CFG=${KRNL_COMPILE_CFG##*/}
echo ${KRNL_COMPILE_CFG}

if [ -z "$KRNL_COMPILE_CFG" ]
then
      echo "Error: The config_files directory does not contain any .cfg file for the compilation of the targeted ${TARGET_KRNL} kernel!"
      echo "Check the coresponding files in the config_files directory"
      exit 1
fi

KRNL_LINK_CFG_PATH=$(find . -type f -name "${KRNL_NAME}-link_${CU_NUM}CU_${VITIS_PLATFORM}.cfg")
echo ${KRNL_LINK_CFG_PATH}
##Try to find the name of the binary container from the field messaDB in the config file

KRNL_LINK_CFG=${KRNL_LINK_CFG_PATH##*/}
echo ${KRNL_LINK_CFG}

if [ -z "$KRNL_LINK_CFG" ]
then
      echo "Error: The config_files directory does not contain any .cfg that matches the targeted kernel or the number of specified CUs!"
      echo "Check the coresponding files in the config_files directory"
      exit 1
fi

MDB=$(grep "messageDb=" $KRNL_LINK_CFG_PATH) 
BIN_CONT_NAME=${MDB:10} # remove the 'messsageDB=' from the name 
BIN_CONT_NAME=${BIN_CONT_NAME%.*}; # retain the part before the dot
echo ${BIN_CONT_NAME}
if [ -z "$BIN_CONT_NAME" ]
then
      echo "Error: Cannot find the name of the binany_container from the link .cfg file"
      echo "Check the coresponding files in the config_files directory "
      exit 1
fi


rm -rf *summary *.csv
cd ${PROJECT_NAME}/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building host side"
make clean PROJECT_NAME=${PROJECT_NAME}
make all -j20 CU=${CU_NUM} PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM}

cd ../../${PROJECT_NAME}_kernels/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building fpga side (hw function)"
make clean PROJECT_NAME=${PROJECT_NAME}
make all PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM} KRNL_NAME=${KRNL_NAME} KRNL_COMPILE_CFG=${KRNL_COMPILE_CFG}

cd ../../${PROJECT_NAME}_system_hw_link/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building hw link"
make clean PROJECT_NAME=${PROJECT_NAME}
make all CU=${CU_NUM} PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM} TARGET=${TARGET} KRNL_NAME=${KRNL_NAME} KRNL_LINK_CFG=${KRNL_LINK_CFG} BIN_CONT_NAME=${BIN_CONT_NAME}

cd ../../${PROJECT_NAME}_system/$TARGET
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo "Building final wrap-up of system"
make clean PROJECT_NAME=${PROJECT_NAME}
make all CU=${CU_NUM} PROJECT_NAME=${PROJECT_NAME} XILINX_VITIS=${XILINX_VITIS} XILINX_XRT=${XILINX_XRT} XILINX_VIVADO=${XILINX_VIVADO} XILINX_VITIS_HLS=${XILINX_VITIS_HLS} VITIS_PLATFORM=${VITIS_PLATFORM} TARGET=${TARGET} BIN_CONT_NAME=${BIN_CONT_NAME}

cd ../../
echo -e "\n-------------------------------------"
echo -e "-------------------------------------"
echo -e "FINISHED"
