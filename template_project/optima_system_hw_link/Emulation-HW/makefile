VPP_LINKER ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

VPP_OPTS = --target hw_emu

BINARY_CONTAINERS += ${BIN_CONT_NAME}.xclbin

BUILD_SUBDIRS += ${BIN_CONT_NAME}.build
BINARY_CONTAINER_1_OBJS += ../../$(PROJECT_NAME)_kernels/$(TARGET)/build/$(KRNL_NAME).xo

ALL_MESSAGE_FILES = $(subst .xclbin,.mdb,$(BINARY_CONTAINERS))

.PHONY: all clean
all: $(BINARY_CONTAINERS)

clean:
	-$(RM) $(BINARY_CONTAINERS) $(ALL_MESSAGE_FILES) 
	-$(RM) *.xclbin.sh *.xclbin.info *.xclbin.link_summary*
	mkdir -p $(BUILD_SUBDIRS)
	find $(BUILD_SUBDIRS) -type f -exec rm -f {} +
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil .ipcache *.log *.ltx

.PHONY: incremental
incremental: all

nothing:

${BIN_CONT_NAME}.xclbin: $(BINARY_CONTAINER_1_OBJS) ../../config_files/$(KRNL_LINK_CFG)
	-@echo $(VPP_LINKER) $(VPP_OPTS) --link --config $(word 2,$^) -o"$@" $(BINARY_CONTAINER_1_OBJS) > ${BIN_CONT_NAME}.xclbin.sh
	$(VPP_LINKER) $(VPP_OPTS) --link --config $(word 2,$^) -o"$@" $(BINARY_CONTAINER_1_OBJS)
