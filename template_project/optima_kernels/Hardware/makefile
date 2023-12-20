VPP ?= ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

VPP_OPTS = --target hw

BUILD_SUBDIRS += build
HW_KERNEL_OBJS += build/krnl_gemm.xo
SRC_KERNEL_OBJS += build/krnl_gemm.xo

ALL_MESSAGE_FILES = $(subst .xo,.o,.mdb,$(SRC_KERNEL_OBJS))

.PHONY: all clean
all: $(HW_KERNEL_OBJS)


clean:
	-$(RM) $(SRC_KERNEL_OBJS) $(ALL_MESSAGE_FILES) 
	-$(RMDIR) $(BUILD_SUBDIRS)
	-$(RMDIR) .Xil *.log

.PHONY: incremental
incremental: all

nothing:

build/krnl_gemm.xo: ../src/krnl_gemm.cpp ../../krnl_gemm-compile_$(VITIS_PLATFORM).cfg
	-@mkdir -p $(@D)
	-@$(RM) $@
	$(VPP) $(VPP_OPTS) --compile -I"$(<D)" --config $(word 2,$^) -o"$@" "$<"
