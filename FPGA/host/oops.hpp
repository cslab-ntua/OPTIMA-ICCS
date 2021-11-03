#include "xcl2.hpp"
#include <vector>
#include <cmath>
unsigned int DATA_SIZE;

void OOPS_copy( const int N, const float *X, const int incX, float *Y, const int incY,char* arg1);

float OOPS_dot( const int N, const float *X, const int incX, const float *Y, const int incY, char* arg1);

float OOPS_ddot( const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, char* arg1);

void OOPS_rot( const int N, float *X, const int incX, float *Y, const int incY, const float  c, const float  s,char* arg1);

void OOPS_rotm( const int N, float *X, const int incX, float *Y, const int incY, const float *P,char* arg1);

void OOPS_scal( const int N, const float alpha, float *X, const int incX,char* arg1);

void OOPS_swap( const int N, float *X, const int incX, float *Y, const int incY,char* arg1);

void OOPS_axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY, char* arg1);

void OOPS_xpay(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY, char* arg1);

float OOPS_asum(const int N, const float *X, const int incX,char* arg1);

int OOPS_iamax(const int N, const float  *X, const int incX, char* arg1);
