#ifndef __SPMV_LOOPS_H__
#define __SPMV_LOOPS_H__

#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>

#include "vector.h"

float spmv_bench_loop(void *matrix, void (*fn)(void *, vector_t *, vector_t *),
                      unsigned long loops, unsigned long nrows,
                      unsigned long ncols);

void spmv_check_loop(void *matrix1, void *matrix2,
                     void (*fn1)(void *, vector_t *, vector_t *),
                     void (*fn2)(void *, vector_t *, vector_t *),
                     unsigned long loops, unsigned long nrows,
                     unsigned long ncols);

#endif

// vim:expandtab:tabstop=8:shiftwidth=4:softtabstop=4
