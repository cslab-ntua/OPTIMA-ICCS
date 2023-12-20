#pragma once
#include "oops.hpp"
#include "ap_int.h"
typedef ap_uint<1> RowType;
typedef ap_uint<32>  ColType;

void test_function(const char *xclbinFilename,
               const int Layout, const int transa, const int transb,
               const int M, const int N, const int K,
               const ValueType alpha, ValueType* __restrict__ A, const int lda,
               ValueType* __restrict__ B, const int ldb,
               const ValueType beta, ValueType* __restrict__ C_in, ValueType* __restrict__ C_out, const int ldc);
