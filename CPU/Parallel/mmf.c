#include "mmf.h"

FILE *mmf_init(char *mmf_file,
               uint32_t *nrows, uint32_t *ncols, uint32_t *nnz)
{
	char input[4096];
	int ret;
	FILE *f;

	f = fopen(mmf_file, "r");
	if (!f) {
		perror("mmf_init: fopen");
		exit(1);
	}

	while (fgets(input, 4096, f)) {
		if (input[0] == '#') {
			continue;
		}

		ret = sscanf(input, "%" PRIu32 " %" PRIu32 " %" PRIu32, nrows,
		             ncols, nnz);

		if (ret != 3) {
			fprintf(stderr, "mmf header error: sscanf");
			exit(1);
		}

		break;
	}

	return f;
}

int mmf_get_next(FILE *mmf, uint32_t *row, uint32_t *col, double *val)
{
	char input[4096];

	if (!fgets(input, 4096, mmf))
		return 0;

	if (sscanf(input,  "%"PRIu32" %"PRIu32" %lf",  row, col, val) != 3) {
		fprintf(stderr, "mmf file error: sscanf failed\n");
		fprintf(stderr, "**input:\n%s\n**\n", input);
		exit(1);
	}

	(*row)--;
	(*col)--;

	return 1;
}

