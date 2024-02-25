RM = rm -f
RMDIR = rm -rf

VPP ?= ${XILINX_VITIS}/bin/v++

all: package

package:
	-@echo $(VPP) --package --config ../../package_$(VITIS_PLATFORM).cfg ../../$(PROJECT_NAME)_system_hw_link/$(TARGET)/${BIN_CONT_NAME}.xclbin -o ${BIN_CONT_NAME}.xclbin
	-@$(VPP) --package --config ../../package_$(VITIS_PLATFORM).cfg ../../$(PROJECT_NAME)_system_hw_link/$(TARGET)/${BIN_CONT_NAME}.xclbin -o ${BIN_CONT_NAME}.xclbin
	cp -f ${BIN_CONT_NAME}.xclbin ../../$(PROJECT_NAME)/$(TARGET)/${BIN_CONT_NAME}_CU$(CU).xclbin

clean:
	$(RMDIR) ./package.build
	${RMDIR} .Xil package *.log *.package_summary ${BIN_CONT_NAME}.xclbin

.PHONY: all package clean
