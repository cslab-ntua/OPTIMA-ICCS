#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <sys/mman.h>

typedef struct {
	double *elements;
	unsigned long size;
} vector_t;

vector_t *vector_create(unsigned long size);
void vector_destroy(vector_t *v);
void vector_init(vector_t *v, double val);
void vector_init_rand_range(vector_t *v, double max, double min);
int vector_compare(vector_t *v1, vector_t *v2);
void vector_print(vector_t *v);

#endif

// vim:expandtab:tabstop=8:shiftwidth=4:softtabstop=4
