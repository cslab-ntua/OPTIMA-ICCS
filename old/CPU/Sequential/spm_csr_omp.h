#ifndef __SPM_CSR_OMP_H__
#define __SPM_CRS_OMP_H__

#include <stdlib.h>
#include <omp.h>
#include <inttypes.h>
#include <assert.h>
#include <sched.h>

#include "vector.h"
#include "spm_csr.h"
#include "mmf.h"

void * spm_csr_omp_init(char *mmf_file, uint32_t *nrows, uint32_t *ncols,
                        uint32_t *nnz);
void spm_csr_omp_destroy(void *spm);
uint64_t spm_csr_omp_size(void *spm);
void spm_csr_omp_multiply(void *spm, vector_t *in, vector_t *out);

#endif


