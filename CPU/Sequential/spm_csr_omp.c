#include "spm_csr_omp.h"

void *spm_csr_omp_init(char *mmf_file, uint32_t *nrows, uint32_t *ncols,
                       uint32_t *nnz)
{
	int nthreads = omp_get_max_threads();
	omp_sched_t schedule;
	int chunksize;
		
	printf("Number of Threads: %d\n", nthreads);
	omp_get_schedule(&schedule, &chunksize);
	if (schedule) {
		printf("Schedule: ");
		switch (schedule) {
			case(1) :
				printf("static\n");
				printf("Chunksize: %d\n", chunksize);
				break;
			case(2) :
				printf("dynamic\n");
        printf("Chunksize: %d\n", chunksize);
        break;
			case(3) :
				printf("guided\n");
        printf("Chunksize: %d\n", chunksize);
        break;
			case(4) :
				printf("auto\n");
				break;
			default :
				printf("uknown\n");
		}
	}

	return spm_csr_init(mmf_file, nrows, ncols, nnz);
}


void spm_csr_omp_destroy(void *spm)
{
	spm_csr_destroy(spm);
}

uint64_t spm_csr_omp_size(void *spm)
{
	return spm_csr_size(spm);
}

void spm_csr_omp_multiply(void *spm_void, vector_t *in, vector_t *out)
{
	spm_csr_t *spm =  (spm_csr_t *) spm_void;
	double *y = out->elements;
	double *x = in->elements;
	double *values = spm->values;
	uint32_t *row_ptr = spm->row_ptr;
	uint32_t *col_ind = spm->col_ind;
	unsigned long nrows = spm->nrows;
	register double yr;
	long i, j;
	
	#pragma omp parallel for private(yr, j) schedule(static,1)
	for(i = 0; i < nrows; i++) {
		yr = 0;
		for(j = row_ptr[i]; j < row_ptr[i+1]; j++)
			yr += (values[j] * x[col_ind[j]]);

		y[i] = yr;
	}
}

