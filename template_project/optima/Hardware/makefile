HOST_CXX ?= g++
RM = rm -f
RMDIR = rm -rf

CXXFLAGS += -std=c++0x -DVITIS_PLATFORM=$(VITIS_PLATFORM) -D__USE_XOPEN2K8 -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -I$(XILINX_VITIS_HLS)/include/ -O2 -g -Wall -c -fmessage-length=0
CXXFLAGS += -DCU=${CU} -fopenmp
LDFLAGS += -lxilinxopencl -lpthread -lrt -lstdc++ -L$(XILINX_XRT)/lib/ -Wl,-rpath-link,$(XILINX_XRT)/lib -fopenmp

HOST_OBJECTS += src/main.o
HOST_OBJECTS += src/oops.o
HOST_OBJECTS += src/test_function.o
HOST_OBJECTS += src/xcl2.o

HOST_EXE =  $(PROJECT_NAME)_$(CU)

BUILD_SUBDIRS += src/

.PHONY: all clean
all:  $(HOST_EXE)

clean:
	-$(RM) -rf $(HOST_EXE) $(HOST_OBJECTS) $(BUILD_SUBDIRS)

.PHONY: incremental
incremental: all


nothing:

src/main.o: ../src/main.cpp ../src/oops.hpp ../src/test_function.h
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

src/oops.o: ../src/oops.cpp ../src/oops.hpp
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

src/test_function.o: ../src/test_function.cpp ../src/test_function.h ../src/oops.hpp
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

src/xcl2.o: ../src/xcl2.cpp ../src/xcl2.hpp
	-@mkdir -p $(@D)
	$(HOST_CXX) $(CXXFLAGS) -o "$@" "$<"

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)
