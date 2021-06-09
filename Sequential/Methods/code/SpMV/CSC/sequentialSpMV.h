#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include <sys/time.h> // for clock_gettime()

typedef struct seqblock
{
	int nnz;
	int roffset, coffset;
	unsigned short int *rloc, *cloc;
	float *val;
}seqblock;

extern int block_width, nrowblks, ncolblks;
extern seqblock *seqMatrixBlock;

extern int numrows, numcols, nnonzero;
extern int *colptrs, *irem;
extern float *xrem;

void sequentialMatrixConversion();

void sequentialCSC_SpMV(float *, float *);

void sequentialCSB_SpMV(float *, float *);
