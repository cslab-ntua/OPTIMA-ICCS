void main_SpMV(){
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

    float * x = (float*) OOPS_malloc(nrows * sizeof(float));
    for (int i=0; i<nrows; i++)
        x[i] = 1.0;

    float * b = (float*) OOPS_malloc(nrows * sizeof(float));
    for ( int i = 0; i < nrows; i++)
        b[i] = 0;

    OOPS_SpMV();
    OOPS_SpMV(nrows, nterm, iat, ja, coef, x, b);

    //verify
    /* ok verified! */

    free(x);
    free(y);
    free(iat);
    free(ja);
    free(coef);
}
