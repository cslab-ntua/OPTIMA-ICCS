#!/bin/bash
# export BUILD=Hardware
if [ $# -lt 1 ]; then
  echo "Expected at least 1 argument but found $#."
  echo "The absolute necessary argument is -c=*|--cus=*"

  exit 1
fi

for i in "$@"; do
      case $i in
            -c=*|--cus=*)
                CU_NUM="${i#*=}"
                shift # past argument=value
                ;;
            -t=*|--target=*)
                TARGET=${i#*=}
                if [[ "$TARGET" != "Emulation-SW" && "$TARGET" != "Emulation-HW" && "$TARGET" != "Hardware" ]]
                then
                    echo "Error: The specified compilation target ${TARGET} is not suported!"
                    exit 1
                fi
                shift  # past argument=value
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


if [[ "$TARGET" == "Emulation-SW" ]]
then
  export BUILD=Emulation-SW
  export XCL_EMULATION_MODE=sw_emu
elif [[ "$TARGET" == "Emulation-HW" ]]
then
  export BUILD=Emulation-HW
  export XCL_EMULATION_MODE=hw_emu
else
  export BUILD=Hardware
fi

# export BUILD=Hardware
export PROJECT=optima

BIN_CONT_FILE=$(find ./${PROJECT}/${BUILD}/ -type f -name "*${CU_NUM}.xclbin")
if [ -z "$BIN_CONT_FILE" ]
then
      echo "Error: Cannot find proper xclbin file in ./${PROJECT}/${BUILD} path!"
      exit 1
fi

./${PROJECT}/${BUILD}/${PROJECT}_${CU_NUM} ${BIN_CONT_FILE}

