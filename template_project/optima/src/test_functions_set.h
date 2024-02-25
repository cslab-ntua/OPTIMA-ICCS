#pragma once
#include "oops.hpp"

/*********** L1 functions **************/
void OOPS_axpy(const int N,const int NCU,const int MAX_CUS, const float alpha,const float *X, const int incX, float *Y, const int incY);

void OOPS_copy(const int N,const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY);
