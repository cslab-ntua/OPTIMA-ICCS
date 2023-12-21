#include "sequentialSpMV.h"

void sequentialMatrixConversion()
{
    int i, r, c, k, k1, k2, blkr, blkc;
    int **top;

    //set number of row blocks and column blocks
    nrowblks = ceil(numrows/(double)(block_width));
    ncolblks = ceil(numcols/(double)(block_width));

    //allocate memory 
    seqMatrixBlock = (seqblock *)malloc(nrowblks*ncolblks * sizeof(seqblock));

    //top[i][j] is the counter that will be used for
    //total assigned nonzeros to block i,j
    top = (int **)malloc(nrowblks * sizeof(int *));
    for( i = 0 ; i < nrowblks; i++ )
        top[i] = (int *)malloc(ncolblks * sizeof(int));

    //initialization
    for(blkr = 0 ; blkr < nrowblks ; blkr++)
    {
        for(blkc = 0 ; blkc < ncolblks ; blkc++)
        {
            top[blkr][blkc] = 0;
            seqMatrixBlock[blkr * ncolblks + blkc].nnz = 0;
        }
    }
    //calculating nnz per block
    for(c = 0 ; c < numcols ; c++)
    {
        k1 = colptrs[c];
        k2 = colptrs[c + 1] - 1;
        blkc = ceil((c + 1)/(double)block_width);
        for(k = k1 - 1 ; k < k2 ; k++)
        {
            r = irem[k];
            blkr = ceil(r/(double)block_width);
            seqMatrixBlock[(blkr - 1) * ncolblks + (blkc - 1)].nnz++;
        }
    }

    //allocating memory based on nonzero counts of each block
    for(blkr = 0 ; blkr < nrowblks ; blkr++)
    {
        for(blkc = 0 ; blkc < ncolblks ; blkc++)
        {
            seqMatrixBlock[blkr * ncolblks + blkc].roffset = blkr * block_width;
            seqMatrixBlock[blkr * ncolblks + blkc].coffset = blkc * block_width;

            if(seqMatrixBlock[blkr * ncolblks + blkc].nnz>0)
            {
                seqMatrixBlock[blkr * ncolblks + blkc].rloc = (unsigned short int *)malloc(seqMatrixBlock[blkr * ncolblks + blkc].nnz * sizeof(unsigned short int));
                seqMatrixBlock[blkr * ncolblks + blkc].cloc = (unsigned short int *)malloc(seqMatrixBlock[blkr * ncolblks + blkc].nnz * sizeof(unsigned short int));
                seqMatrixBlock[blkr * ncolblks + blkc].val = (float *)malloc(seqMatrixBlock[blkr * ncolblks + blkc].nnz * sizeof(float));
            }
            else
            {
                seqMatrixBlock[blkr * ncolblks + blkc].rloc = NULL;
                seqMatrixBlock[blkr * ncolblks + blkc].cloc = NULL;
            }
        }
    }

    //assigning each nonzero on CSC matrix to its corresponding position on CSB matrix
    for(c = 0 ; c < numcols ; c++)
    {
        k1 = colptrs[c];
        k2 = colptrs[c + 1] - 1;
        blkc = ceil((c + 1)/(double)block_width);

        for(k = k1 - 1 ; k < k2 ; k++)
        {
            r = irem[k];
            blkr = ceil(r/(double)block_width);

            seqMatrixBlock[(blkr - 1) * ncolblks + blkc - 1].rloc[top[blkr - 1][blkc - 1]] = r - seqMatrixBlock[(blkr - 1) * ncolblks + blkc - 1].roffset;
            seqMatrixBlock[(blkr - 1) * ncolblks + blkc - 1].cloc[top[blkr - 1][blkc - 1]] = (c + 1) -  seqMatrixBlock[(blkr - 1) * ncolblks + blkc - 1].coffset;
            seqMatrixBlock[(blkr - 1) * ncolblks + blkc - 1].val[top[blkr - 1][blkc - 1]] = xrem[k];

            top[blkr - 1][blkc - 1] = top[blkr - 1][blkc - 1] + 1;
        }
    }


    for(i = 0 ; i < nrowblks ; i++)
    {
        free(top[i]);
    }
    free(top);
}

void sequentialCSC_SpMV(float *x, float *b)
{
    int i, j;
    for(i = 0; i < numcols; i++)
    {
        for(j = colptrs[i] - 1; j < colptrs[i+1] - 1; j++)
        {
            b[irem[j] - 1] += xrem[j]*x[i];
        }
    }
}

void sequentialCSB_SpMV(float *x, float *b)
{
    int i, j, k;
    seqblock blk;
    for(i = 0; i < nrowblks; i++)
    {
        for(j = 0; j < ncolblks; j++)
        {
            blk = seqMatrixBlock[i * ncolblks + j];
            for(k = 0; k < blk.nnz; k++)
            {
                b[ blk.rloc[k] + blk.roffset - 1 ] += blk.val[k] * x[ blk.cloc[k] + blk.coffset - 1];
            }
        }
    }
}

