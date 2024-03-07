#pragma once
#include "oops.hpp"

/*********** L1 functions **************/
void OOPS_axpy(const int N,const int NCU,const int MAX_CUS, const float alpha,const float *X, const int incX, float *Y, const int incY);

void OOPS_copy(const int N,const int NCU, const int MAX_CUS, float *X, const int incX, float *Y, const int incY);

float OOPS_ddot(const int N, const int NCU, const int MAX_CUS, const float alpha, const float *X, const int incX, const float *Y, const int incY);

float OOPS_dot(const int N, const int NCU, const int MAX_CUS, const float *X, const int incX, const float *Y, const int incY);

int OOPS_iamax(const int N, const int NCU, const int MAX_CUS, const float  *X, const int incX);

int OOPS_iamin(const int N, const int NCU, const int MAX_CUS, const float  *X, const int incX);
