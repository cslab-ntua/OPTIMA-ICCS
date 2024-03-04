#!/bin/bash

if [[ "$#" -lt 1 ]] 
then
  echo "Expected at least 1 argument but found $#."
  exit 1
fi

BACKCUP=FALSE

for i in "$@"; do
      case $i in
            -k=*|--kernel=*)
                  TARGET_KRNL="${i#*=}" #keeps the value after the '='
                  shift # past argument=value
                  ;;
            -b|--backup)
                  BACKCUP=TRUE
                  shift # past argument with no value
                  ;;
            -*|--*)
                  echo "Unknown option $i" #check for wrong arguments
                  exit 1
                  ;;
            *)
                  echo "Unknown option $i" #check for wrong arguments
                  exit 1
                  ;;
            esac
done



HOST_REPO_PATH=$(find ../host -type d -name "$TARGET_KRNL")
if [ -z "$HOST_REPO_PATH" ]
then
      echo "Error: Directory with host files for Kernel $TARGET_KRNL does not exist. Please check the input arguments!"
      exit 1
else
      echo "Directory with host files for Kernel $TARGET_KRNL found!"
fi
KRNL_REPO_PATH=$(find ../kernel -type d -name "$TARGET_KRNL")
if [ -z "$KRNL_REPO_PATH" ]
then
      echo "Error: Directory with kernel file for kernel $TARGET_KRNL does not exist. Please check the input arguments!"
      exit 1
else
      echo "Directory with kernel file for kernel $TARGET_KRNL found!"
fi

CFG_REPO_PATH=$(find ../kernel_config_files -type d -name "$TARGET_KRNL")
if [ -z "$CFG_REPO_PATH" ]
then
      echo "Error: Directory with config files for kernel $TARGET_KRNL does not exist. Please check the input arguments!"
      exit 1
else
      echo "Directory with config files for kernel $TARGET_KRNL found!"
fi

HOST_SRC_PATH=optima/src
KRNL_PRJ_PATH=optima_kernels/src
CFG_PRJ_PATH=config_files


# if [ -d "$HOST_REPO_PATH" ]; then
#     echo "Kernel $TARGET_KRNL exists."
# else
#     echo "Error: Kernel $TARGET_KRNL does not exists!"
#     exit 1
# fi


echo "Entering $HOST_SRC_PATH directory"
cd $HOST_SRC_PATH

#optional back up the old host files
# if [ $2 eq "-b" ]; then
if [[ "$BACKUP" == "TRUE" ]]; then
    echo "Backing up old main.cpp test_function.cpp and test_fucntio.h files"
    mv main.cpp main.old.cpp
    mv test_function.cpp test_function.old.cpp
#     mv test_function.h test_function.old.h
fi

echo "Copying host files for $TARGET_KRNL kernel to $HOST_SRC_PATH"
cp  ../../$HOST_REPO_PATH/main_$TARGET_KRNL.cpp ./main.cpp
cp  ../../$HOST_REPO_PATH/test_$TARGET_KRNL.cpp ./test_function.cpp
# cp  ../../$HOST_REPO_PATH/test_$TARGET_KRNL.h ./test_function.h
cd ../../

echo "Entering $KRNL_PRJ_PATH directory"
cd $KRNL_PRJ_PATH
echo "Copying kernel file for $TARGET_KRNL kernel to $KRNL_PRJ_PATH"
cp ../../$KRNL_REPO_PATH/* ./
cd ../../

echo "Entering $CFG_PRJ_PATH directory"
cd $CFG_PRJ_PATH
echo "Copying .cfg files for $TARGET_KRNL kernel to config_files directory"

cp -r ../$CFG_REPO_PATH/* ./
cd ../






