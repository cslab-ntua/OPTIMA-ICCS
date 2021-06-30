void jacobi_matrix(float **Adata, int n, float *Mdata)
{
    int i;		
    
    for (i = 0; i < n; ++i) {
           M[i] += Adata[i][i];
    }
}

void jakobi_precond(float *Mdata, float *x, int n, float *Minvx)
{
    int i;		
    
    for(i=0; i<n; i++){
	    Minvx[i] = 1/Mdata[i]*x[i];
    }
}
