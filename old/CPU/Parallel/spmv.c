#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <libgen.h>
#include <assert.h>

#include "spm_csr.h"
#include "spm_csr_omp.h"
#include "spmv_loops.h"


static char *progname = NULL;
extern int optind;

static void help()
{
	fprintf(stderr, "Usage: %s [ -h -c -b [-l nr_loops] [-L outer_loops]] "
	        "mmf_file [method]\n", progname);
}

int main(int argc, char **argv)
{
	progname = argv[0];

	// Parse options.
	int opt_check = 0;
	int opt_bench = 0;
	int inner_loops = 16;
	int outer_loops = 5;
	int c;

	while ((c = getopt(argc, argv, "hcbl:L:d:")) != -1){
		switch (c) {
		case 'c':
			opt_check = 1;
			break;

		case 'b':
			opt_bench = 1;
			break;

		case 'h':
			help();
			exit(0);

		case 'L':
			outer_loops = atol(optarg);
			break;

		case 'l':
			inner_loops = atol(optarg);
			break;

		default:
			fprintf(stderr, "Error parsing arguments: -%c-\n", c);
			help();
			exit(1);
		}
	}

	if (!opt_check && !opt_bench)
		opt_bench = 1;

	int remargs = argc - optind;

	if (remargs < 1) {
		help();
		exit(1);
	}

	argv = &argv[optind-1];

	char *mmf_file = argv[1];
	char *method;	

	if (remargs >= 2)
		method = argv[2];
	else
		method = "csr";

	void *(*init_fn)(char *, uint32_t *, uint32_t *, uint32_t *) = NULL;
  void (*destroy_fn)(void *) = NULL;
  uint64_t (*size_fn)(void *) = NULL;
  void (*multiply_fn)(void *, vector_t *, vector_t *) = NULL;

	// Declare any new method here.
	if (strcmp(method, "csr") == 0) {
		init_fn = &spm_csr_init;
		destroy_fn = &spm_csr_destroy;
		size_fn = &spm_csr_size;
		multiply_fn = &spm_csr_multiply;
	} else if (strcmp(method, "csr_omp") == 0) {
    init_fn = &spm_csr_omp_init;
    destroy_fn = &spm_csr_omp_destroy;
    size_fn = &spm_csr_omp_size;
    multiply_fn = &spm_csr_omp_multiply;
  } else {
		fprintf(stderr, "You must define method %s in spmv.c\n", method);
		exit(1);
	}

	uint32_t nrows, ncols, nnz;
	void *spm = (*init_fn)(mmf_file, &nrows, &ncols, &nnz); 

	if (opt_check) {
		uint32_t nrows2, ncols2, nnz2;
		
		void *spm2 = spm_csr_init(mmf_file, &nrows2, &ncols2, &nnz2);
		if (nrows != nrows2 || ncols != ncols2 || nnz != nnz2) {
			fprintf(stderr, "sizes do not match (f:%s) (m:%s)\n", mmf_file, method);
			exit(1);
		}
		spmv_check_loop(spm, spm2, multiply_fn, &spm_csr_multiply, 1, nrows, ncols);
		printf("check for %s in %s verified\n", method, basename(mmf_file));
		
		spm_csr_destroy(spm2);
	}

	if (opt_bench){
		int count;
		double t = -666.0;

		for (count = 0; count < outer_loops; count++) {
			t = spmv_bench_loop(spm, multiply_fn, inner_loops, nrows, ncols);			
			double flops = (double) (inner_loops * nnz * 2) / ((double) 1000 * 1000 * t);
			printf("m:%s f:%s s:%" PRIu64 " t:%lf r:%lf\n", method, basename(mmf_file), (*size_fn)(spm), t, flops);
		}

	}

	(*destroy_fn)(spm);

	return 0;
}
