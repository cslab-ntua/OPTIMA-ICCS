#include "xcl2.hpp"
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <unistd.h>
#include "ap_int.h"

using namespace std;

typedef ap_uint<32>  ColType;
typedef ap_uint<1> RowType;

//function that programs device with xclbin file
void program_device(const char *arg1);

//OOPS memory allocation. Allocates aligned memory based on OS page size
void *OOPS_malloc(size_t alloc_bytes);

//function that release OpenCL objects and device
void release_device();

inline cl::Context context;
inline cl::Program program;
inline cl::CommandQueue q;
