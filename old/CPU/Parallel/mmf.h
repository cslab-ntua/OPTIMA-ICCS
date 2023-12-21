#ifndef __MMF_H__
#define __MMF_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

FILE *mmf_init(char *filename,
               uint32_t *nrows, uint32_t *ncols,
               uint32_t *nnz);

int mmf_get_next(FILE *mmf,
                 uint32_t *row, uint32_t *col,
                 double *val);

#endif
