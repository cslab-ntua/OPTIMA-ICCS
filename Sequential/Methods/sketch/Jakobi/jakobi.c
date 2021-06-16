void jacobi_matrix(double *M, double **Adata, int n)
{
    int i;		
    
    for (i = 0; i < n; ++i) {
           M[i] += Adata[i][i];
    }
}

void jakobi_precond(double *Minvx, double *Mdata, double *x, int n)
{
    int i;		
    
    for(i=0; i<n; i++){
	Minvx[i] = 1/Mdata[i]*x[i];
    }
}
