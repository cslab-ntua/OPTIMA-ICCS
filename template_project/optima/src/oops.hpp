#pragma once 

#include "xcl2.hpp"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <fstream> // to use i/ofstream
#include <iomanip> // to use setw and setprecision
#include <unistd.h>
#include "ap_int.h"




using namespace std;

typedef ap_uint<32>  ColType;
typedef ap_uint<1> RowType;

//----------------------------------------------------------------------------------------
#define V_DATA_SIZE 16
// function that programs device with xclbin file
void program_device(const char *arg1);

//----------------------------------------------------------------------------------------

// OOPS memory allocation. Allocates aligned memory based on OS page size
void *OOPS_malloc(size_t alloc_bytes);

void convert_packed_matrix_to_Hyperpacked(const char Uplo,const float *Apacked, uint32_t packedSize, float *FPGApacked, uint32_t fpgaPackedSize, uint32_t N, uint32_t VDATA_SIZE);
//----------------------------------------------------------------------------------------
// Function that computes the number of VDATA_SIZE element blocks of a hyperpacked (FPGA friendly) triangular matrix
// N is the real dimension of the triangular matrix, not of the hyperpacked matrix.
// VDATA_SIZE is the number of elements that a block of memory can store. In our case the HBM width is 512bit. So it can store 16 floats or 8 doubles.
   // So VDATA_SIZE could be eiter 512/32=16 or 512/64=8
int OOPS_cmpt_hyperpacked_triang_mtrx_blks(int N, int VDATA_SIZE);

// Functions that computes the VDATA_size block index of the table based on the start or end row provided
// These function are used in multikernel cases
// For the moment the computed idxs are for Upper  Triangular HyperPacked Matrices
int OOPS_Hpacked_triMtx_start_idx(int N, int VDATA_SIZE, int row);
int OOPS_Hpacked_triMtx_end_idx(int N, int VDATA_SIZE, int row);

// extra functions for Jacobi
void printMatrix1d(float *m, string name, int N);
void printMatrix(float *m, string name, int N);
int rowDistribution(int N, int plEngines, int *rowsPerPlEngine);

//extra functions for LU

void printDiagPaddedMatrix(float *m, string name, int N, int vdt);
void printPaddedMatrix1d(float *m, string name, int N, int paddedWidth);


void printMatrix2(float *m, string name, int r,int c);
size_t OOPS_align_bytes(size_t bytes);
int roundUpToMultiple(int number, int multiple);
int luRowDistribution(int N, int plEngines, int *rowsPerPlEngine);

// Global variable storing device info
struct cl_interface{
   cl::Context context;
   cl::Program program;
   cl::CommandQueue q;
};

extern cl_interface program_interface;

typedef float ValueType;




