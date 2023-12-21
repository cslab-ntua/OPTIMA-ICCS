int precond_cg(float **Adata, float *b, float *x, double rtol, int n, int maxiter, int *niters)
{
    const int nbytes = n * sizeof(float);

    double bnorm2;              /* ||b||^2 */
    double rnorm2;              /* Νόρμα υπολοίπου στο τετράγωνο */
    double rz, rzold;           /* r'*z για 2 διαδοχικές επαναλήψεις */
    double alpha, beta;
    double rz_local,rnorm2_local,bnorm2_local;

    float *s;                  /* Κατεύθυνση αναζήτησης */
    float *r;                  /* Υπόλοιπο         */
    float *z;                  /* Προσωρινό διάνυσμα */

    int i = 0,j;                /* Τρέχουσα επανάληψη */

    s = (float *) malloc(nbytes);
    r = (float *) malloc(nbytes);
    z = (float *) malloc(nbytes);

 
    bnorm2    = ddot(b, b, n);
    
    memset(x, 0, nbytes);	//αρχικοποίηση λύσης
    memcpy(r, b, nbytes);	//και υπολοίπου - r0=b-A*x0 (x0=0)

    jacobi_matrix(Adata, n, Mdata);
    jacobi_precond(Mdata, r, n, z);	//εφαρμογή του preconditioner - z0 = (M στην -1)*r0

    memcpy(s, z, nbytes);	//αρχικοποίηση κατεύθυνσης αναζήτησης	- p0 = z0

    /* Αρχικοποίηση rz και rnorm2 */
    rz        = ddot(r, z, n);
    rnorm2    = ddot(r, r, n);
    

    for (i = 0; i < maxiter ; ++i) {
        printf("Επανάληψη #%d\n", i);

        spmv(z, Adata, s, n);	//z:=A*pk 

        
        /* Ddot*/
        alpha = rz / ddot(s, z, n);	//ak = rkT*zk/pkT*A*pk
        axpy(x, alpha, s, x, n);	//xk+1 = xk + ak*pk
        axpy(r, -alpha, z, r, n);	//rk+1 = rk - ak*A*pk
  
        jacobi_precond(Mdata, r, n, z);		//zk+1 = (M στην -1)*rk+1

        rzold = rz;
        

        rz = ddot(r, z, n);  		//rTk+1*zk+1
        beta = -rz / rzold;		//β = rTk+1*zk+1/rTk*zk 
        axpy(s, -beta, s, z, n);	//pk+1 = zk+1+βk*pk

        printf("(%d)rz=%2.15f,alpha=%2.15f,rnorm=%2.15f\n",i,rz,alpha,rnorm2);
        
        
        /* check error */
        rnorm2     = ddot(r, r, n);
        if(rnorm2 <= bnorm2 * rtol * rtol)
        	break;
        
    }
    
    free(z);
    free(r);
    free(s);

    *niters = i;
}

//check also
//https://juliahub.com/docs/Krylov/0fcC3/0.5.5/solvers/
