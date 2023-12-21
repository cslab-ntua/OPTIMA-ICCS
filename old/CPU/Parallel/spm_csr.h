#ifndef __SPM_CRS_H__
#define __SPM_CRS_H__

#include <inttypes.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "mmf.h"
#include "vector.h"

typedef struct {
	double *values;
	uint32_t *col_ind, *row_ptr;
	uint32_t nnz, nrows, ncols;
} spm_csr_t;

void *spm_csr_init(char *mmf_file, uint32_t *nrows, uint32_t *ncols,
                   uint32_t *nnz);
void spm_csr_destroy(void *spm);
uint64_t spm_csr_size(void *spm);
void spm_csr_multiply(void *spm, vector_t *x, vector_t *y);

#endif
