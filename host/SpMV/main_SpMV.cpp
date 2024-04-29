#include <omp.h>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>

#include "oops.hpp"
#include "matrix_vector_generation.hpp"
#include "test_functions_set.h"


using namespace std;
using namespace std::chrono;


int main(int argc, const char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    
    printf("----------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\n(0) Program the device\n");
    program_device(argv[1]);

    int nrows = 0;
    int nterm = 0;
    int * irow = NULL;
    int * iat = NULL;
    int * ja = NULL;
    float * coef = NULL;

    ifstream file_A_ascii("m3e_matrix.coo");
    file_A_ascii >> nrows >> nterm;
    printf("\tnrows = %d\n",nrows);
    printf("\tnterm = %d\n",nterm);

    // Allocate space
    irow = (int*) OOPS_malloc(nterm * sizeof(int));
    iat  = (int*) OOPS_malloc((nrows+1) * sizeof(int));
    ja  = (int*) OOPS_malloc(nterm * sizeof(int));
    coef = (float*) OOPS_malloc(nterm * sizeof(float));

    // Read entries
    for ( int i = 0; i < nterm; i++) {
        file_A_ascii >> irow[i] >> ja[i] >> coef[i];
        irow[i]--;
        ja[i]--;
    }
    // for(int i=0; i<100; i++)
    //    std::cout << irow[i] << "\t" << ja[i] << "\t" << coef[i] << "\n";

    // Assembly iat for the Compressed Sparse Row (CSR) format
    // Note that the matrix is symmetric so CSR format == CSC format
    iat[0] = 0;
    int j = 0;
    for ( int i = 0; i < nterm; i++) {
        if( irow[i] > j ) {
            iat[j+1] = i;
            j++;
        }
    }
    iat[nrows] = nterm;

    // Free scratch
    free(irow);

    // Close file
    file_A_ascii.close();

    // number of columns for this kind of matrices is equal to number of rows
    float * x = (float*) OOPS_malloc(nrows * sizeof(float));
    for (int i=0; i<nrows; i++)
        x[i] = 1.0;

    float * b = (float*) OOPS_malloc(nrows * sizeof(float));
    for ( int i = 0; i < nrows; i++)
        b[i] = 0;


    OOPS_SpMV(nrows, nterm, iat, ja, coef, x, b);

    //verify
    /* ok verified! */

    free(x);
    free(b);
    free(irow);
    free(iat);
    free(ja);
    free(coef);


    //-------------------------------------------------------------------------------------
    printf("\n(5) Close OpenCL objects\n");
    clReleaseProgram(program_interface.program.get());
    clReleaseContext(program_interface.context.get());
    clReleaseCommandQueue(program_interface.q.get());

    //-------------------------------------------------------------------------------------

    // End
    printf("\n");

    return 0;
}

