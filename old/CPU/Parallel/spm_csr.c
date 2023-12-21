#include "spm_csr.h"

void *spm_csr_init(char *mmf_file, uint32_t *nrows, uint32_t *ncols,
                   uint32_t *nnz)
{
	// Allocate space for Sparse Matrix in CSR storage format.
	spm_csr_t *spm = (spm_csr_t *) malloc(sizeof(spm_csr_t));
	if (!spm){
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	FILE *mmf = mmf_init(mmf_file, nrows, ncols, nnz);
	spm->nrows = *nrows;
	spm->ncols = *ncols;
	spm->nnz = *nnz;

	spm->values = (double *) malloc(spm->nnz * sizeof(double));
	spm->col_ind = (uint32_t *) malloc(spm->nnz * sizeof(uint32_t));
	spm->row_ptr = (uint32_t *) malloc((spm->nrows + 1) * sizeof(uint32_t));
	if (!spm->values || !spm->col_ind || !spm->row_ptr){
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	// Initialiaze data of CSR matrix format from mmf_file.
	uint32_t row, col, row_prev;
	double val;
	uint32_t row_i = 0, val_i = 0;

	spm->row_ptr[row_i++] = val_i;
	row_prev = 0;
	while (mmf_get_next(mmf, &row, &col, &val)) {

		// Sanity checks.
		assert(row >= row_prev);
		assert(row < spm->nrows);
		assert(col < spm->ncols);
		assert(val_i < spm->nnz);

		// New row.
		if (row != row_prev) {
			uint32_t i;

			// Handle empty rows, just in case.
			for (i = 0; i < row - row_prev; i++)
				spm->row_ptr[row_i++] = val_i;

			row_prev = row;
		}

		// Update values and col_ind.
		spm->values[val_i] = val;
		spm->col_ind[val_i] = col;
		val_i++;
	}
	// Last entry to row_ptr.
	spm->row_ptr[row_i++] = val_i;

	// More sanity checks.
	assert(row_i == spm->nrows + 1);
	assert(val_i == spm->nnz);
	fclose(mmf);

	return spm;
}

void spm_csr_destroy(void *spm_void)
{
	spm_csr_t *spm = (spm_csr_t *) spm_void;
	
	free(spm->values);
	free(spm->col_ind);
	free(spm->row_ptr);
	free(spm);
}

uint64_t spm_csr_size(void *spm_void)
{
	spm_csr_t *spm = (spm_csr_t *) spm_void;
	uint64_t ret = 0;

	ret += spm->nnz * (sizeof(double) + sizeof(uint32_t));
	ret += (spm->nrows + 1) * sizeof(uint32_t);

	return ret;
}

void spm_csr_multiply(void *spm_void, vector_t *in, vector_t *out)
{
	spm_csr_t *spm = (spm_csr_t *) spm_void;

	double *y = out->elements;
	double *x = in->elements;
	
	double *values = spm->values;
	uint32_t *row_ptr = spm->row_ptr;
	uint32_t *col_ind = spm->col_ind;
	uint32_t nrows = spm->nrows;
	register double yr;

	uint32_t i, j;

	for(i = 0; i < nrows; i++) {
		yr = 0;
		for(j = row_ptr[i]; j < row_ptr[i+1]; j++)
			yr += (values[j] * x[col_ind[j]]);

		y[i] = yr;
	}
}

