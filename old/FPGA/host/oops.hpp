#include "xcl2.hpp"
#include <vector>
#include <cmath>

void OOPS_copy( const int N, const float *X, const int incX, float *Y, const int incY);

float OOPS_dot( const int N, const float *X, const int incX, const float *Y, const int incY);

float OOPS_ddot( const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY);

void OOPS_rot( const int N, float *X, const int incX, float *Y, const int incY, const float  c, const float  s);

void OOPS_rotm( const int N, float *X, const int incX, float *Y, const int incY, const float *P);

void OOPS_scal( const int N, const float alpha, float *X, const int incX);

void OOPS_swap( const int N, float *X, const int incX, float *Y, const int incY);

void OOPS_axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY);

void OOPS_xpay(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY);

float OOPS_asum(const int N, const float *X, const int incX);

int OOPS_iamax(const int N, const float  *X, const int incX);
