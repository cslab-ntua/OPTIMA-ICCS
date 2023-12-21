#include "vector.h"

vector_t *vector_create(unsigned long size)
{
	vector_t *v = (vector_t *) malloc(sizeof(vector_t));
	if (!v) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	v->size = size;
	v->elements = (double *) malloc(size * sizeof(double));
	if (!v->elements) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	return v;
}

void vector_destroy(vector_t *v)
{
	free(v->elements);
	free(v);
}

void vector_init(vector_t *v, double val)
{
	unsigned long i;

	for (i = 0; i < v->size; i++)
		v->elements[i] = val;
}

void vector_init_rand_range(vector_t *v, double max, double min)
{
	unsigned long i;
	double val;

	for (i = 0; i < v->size; i++) {
		val = ((double) (rand()+i) / ((double) RAND_MAX+1));
		v->elements[i] = min + val*(max-min);
	}
}

static inline int elems_neq(double a, double b)
{
	if (fabs((double)(a-b)/(double)a) > 1.e-7)
		return 1;
	return 0;
}

int vector_compare(vector_t *v1, vector_t *v2)
{
	unsigned long i;

	if (v1->size != v2->size) {
		fprintf(stderr, "v1->size=%lu v2->size=%lu differ", v1->size, v2->size);
		return -2;
	}

	for (i = 0; i < v1->size; i++) {
		if (elems_neq(v1->elements[i], v2->elements[i])) {
			fprintf(stderr, 
			        "element %ld differs: %10.20lf != %10.20lf\n",
			        i, v1->elements[i], v2->elements[i]);
			return -1;
		}
	}

	return 0;
}

void vector_print(vector_t *v)
{
    unsigned long i;

    printf("[ ");
    for (i = 0; i < v->size; i++)
        printf("%lf ", v->elements[i]);
    printf("]\n");
}

// vim:expandtab:tabstop=8:shiftwidth=4:softtabstop=4
