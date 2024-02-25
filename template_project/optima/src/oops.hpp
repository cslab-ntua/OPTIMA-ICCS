#pragma once 

#include "xcl2.hpp"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <fstream> // to use i/ofstream
#include <iomanip> // to use setw and setprecision

//----------------------------------------------------------------------------------------

// function that programs device with xclbin file
void program_device(const char *arg1);

//----------------------------------------------------------------------------------------

// OOPS memory allocation. Allocates aligned memory based on OS page size
void *OOPS_malloc(size_t alloc_bytes);

//----------------------------------------------------------------------------------------

// Global variable storing device info
struct cl_interface{
   cl::Context context;
   cl::Program program;
   cl::CommandQueue q;
};

extern cl_interface program_interface;

typedef float ValueType;

#if CU == 1
   #define nstripe 1
#elif CU == 2
   #define nstripe 2
#elif CU == 4
   #define nstripe 4
#elif CU == 8
   #define nstripe 8
#elif CU == 10
   #define nstripe 10
#elif CU == 16
   #define nstripe 16
#elif CU == 28
   #define nstripe 28
#elif CU == 30
   #define nstripe 30
#endif
