#include "spmv_loops.h"

float spmv_bench_loop(void *matrix, void (*fn)(void *, vector_t *, vector_t *),
                      unsigned long loops, unsigned long nrows,
                      unsigned long ncols)
{
	vector_t *x;
	vector_t *y;

	struct timeval start;
	struct timeval end;
	double secs;

	unsigned long i;

	x = vector_create(ncols);
	y = vector_create(nrows);

	vector_init_rand_range(x, -1000, 1000);
	vector_init(y, 0);

	gettimeofday(&start, NULL);
	for (i = 0; i < loops; i++)
		fn(matrix, x, y);
	gettimeofday(&end, NULL);

	secs = (double) end.tv_sec - (double) start.tv_sec;
	secs += ((double) end.tv_usec - (double) start.tv_usec) / 1000000.0;

	vector_destroy(x);
	vector_destroy(y);

	return secs;
}

void spmv_check_loop(void *m1, void *m2,
                     void (*fn1)(void *, vector_t *, vector_t *),
                     void (*fn2)(void *, vector_t *, vector_t *),
                     unsigned long loops, unsigned long nrows,
                     unsigned long ncols)
{
	unsigned long i;
	vector_t *x;
	vector_t *y1;
	vector_t *y2;

	x = vector_create(ncols);
	y1 = vector_create(nrows);
	y2 = vector_create(nrows);

	for (i = 0; i < loops;  i++) {
		vector_init_rand_range(x, -1000, 1000);
		/* We start from 21 in order to check if our output is
		   not affected by its initial value.*/
		vector_init(y1, 21);

		fn1(m1, x, y1);
		fn2(m2, x, y2);

		if (vector_compare(y1, y2) < 0)
			exit(1);
	}

	vector_destroy(x);
	vector_destroy(y1);
	vector_destroy(y2);
}

