#include "test_functions_set.h"

int bin_search(const int ii, int iend, const int *const v)
{
	int istart = 0;
	int pos = iend/2;
	while (iend-istart > 1){
		if (v[pos] > ii){
			// Get the left interval
			iend = pos;
		} else {
			// Get the right interval
			istart = pos;
		}
		pos = (istart+iend)/2;
	}
	return pos;
}

#undef SWAP // it was defined already in lib/macros/macrolib.h
template <typename TYPE>
inline void SWAP(TYPE &i1, TYPE &i2){TYPE tmp = i1; i1 = i2; i2 = tmp;}
template void SWAP<int>(int &, int & );
template void SWAP<float>(float &, float & );

void heapsort_2v(int* __restrict__ x1, float* __restrict__ x2, const int n)
{
	for (int node = 1; node < n; node ++){
		int i = node;
		int j = i/2;
		while( x1[j] < x1[i] ){
			SWAP(x1[j],x1[i]);
			SWAP(x2[j],x2[i]);
			i = j;
			j = i/2;
			if (i == 0) break;
		}
	}
	
	for (int i = n-1; i > 0; i --){
		SWAP(x1[i],x1[0]);
		SWAP(x2[i],x2[0]);
		int k = i - 1;
		int ik = 0; 
		int jk = 1;
		if (k >= 2){
			if (x1[2] > x1[1]) jk = 2;
		}
		bool cont_cycle = false;
		if (jk <= k){
			if (x1[jk] > x1[ik]) cont_cycle = true;
		}
		while (cont_cycle){
			SWAP(x1[jk],x1[ik]);
			SWAP(x2[jk],x2[ik]);
			ik = jk;
			jk = ik*2;
			if (jk+1 <= k){
				if (x1[jk+1] > x1[jk]) jk = jk+1;
			}
			cont_cycle = false;
			if (jk <= k){
				if (x1[jk] > x1[ik]) cont_cycle = true;
			}
		}
	}
}
/****************************************************/

void OOPS_SpMV(const int nrows, const int nterm,
               const long int* iat, const int* ja, const float* __restrict__ coef,
               const float* __restrict__ x, float* __restrict__ b)
{

	cl_int err;
	// number of Compute Units to spawn
	int nstripe = 16;
	std::vector<cl::Kernel> krnls(nstripe);

	// This call will get the kernel object from program. A kernel is an OpenCL function that is executed on the FPGA.
	std::string krnl_name = "krnl_SpMV";
	for (int i = 0; i < nstripe; i++) {
		std::string cu_id = std::to_string(i + 1);
		std::string krnl_name_full = krnl_name + ":{" + krnl_name +"_" + cu_id + "}";
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i);
		OCL_CHECK(err, krnls[i] = cl::Kernel(program_interface.program, krnl_name_full.c_str(), &err));
	}

	// allocate padded buffers
	int padding_factor = 64 / sizeof(float); // 64 is 512-bits, which is the width of the HBM interface
	// printf("padding_factor = %d\n", padding_factor);
	int nterm_padded = nterm + nrows * padding_factor;
	long int * iat_padded  = (long int*) OOPS_malloc((size_t)((nrows+1) * sizeof(long int)));
	int * ja_padded   = (ColType*) OOPS_malloc((size_t)(nterm_padded * sizeof(ColType)));
	float * coef_padded = (float*) OOPS_malloc((size_t)(nterm_padded * sizeof(float)));

	/*******************************************************************************************/

	long int * irow_padded = (long int*) OOPS_malloc((size_t)(nterm_padded * sizeof(long int)));
	long int * ja_padded_int = (long int*) OOPS_malloc((size_t)( nterm_padded * sizeof(long int))); // this is needed for padding to be more fast

	// irow_padded initialization
	for (int i = 0; i < nterm_padded; i++)
		irow_padded[i] = -1;

	// allocate scratch buffers
	int *nt_add = (int*) OOPS_malloc((size_t)(m * sizeof(int)));

	// get number of new adding for each row
	for (int i = 0; i < nrows; i++) {
		int nt_row = iat[i+1]-iat[i];
		if(nt_row != 0){
			int resto  = nt_row % padding_factor;
			int mult   = int(nt_row / padding_factor);
			if ( resto == 0 ) {
				nt_add[i] = 0;
			} else {
				int nt_row_p  = (mult+1)*padding_factor;
				nt_add[i] = nt_row_p - nt_row;
				// printf("nt_add %d %d %d %d %d\n",i,nt_row,resto,mult,nt_add[i]);
			}
		}
		else
			nt_add[i] = padding_factor;
	}

	// copy entries
	for (int i = 0; i < nrows; i++) {
		int nt_row = iat[i+1]-iat[i];
		int jj = iat[i]+padding_factor*i;
		for (int j = 0; j < nt_row; j++) {
			irow_padded[jj+j] = i;
			ja_padded_int[jj+j] = ja[iat[i]+j];
			coef_padded[jj+j] = a[iat[i]+j];
		}
	}

	// right padding
	for (int i = 0; i < nrows/2; i++) {
		int nt_row = iat[i+1]-iat[i];

		int icol = i;
		for (int j = 0; j < nt_add[i]; j++) {
			bool find_icol = true;
			while(find_icol) {
				icol++;
				if ( icol > nrows-1 ) {
					printf("****ERROR PAD 1****\n");
					exit(-1);
				}
				// check if icol is already in the i-row
				int pos = bin_search(icol,nt_row,&(ja[iat[i]]));
				if (icol != ja[iat[i]+pos] ) find_icol = false;
			}
			// add zero term on icol-col of i-row
			int jj = iat[i]+padding_factor*i+nt_row+j;
			irow_padded[jj] = i;
			ja_padded_int[jj]   = icol;
			coef_padded[jj] = 0.;
		}

		// sorting
		if ( nt_add[i] > 0 ) {
			int jj = iat[i]+padding_factor*i;
			heapsort_2v(&(ja_padded_int[jj]),&(coef_padded[jj]),nt_row+nt_add[i]);
		}
	}

	// left padding
	for (int i = nrows-1; i >= nrows/2; i--) {
		int nt_row = iat[i+1]-iat[i];
		int icol = i;
		int icol_right=n-1;
		for (int j = 0; j < nt_add[i]; j++) {
			bool find_icol = true;
			while(find_icol) {
				icol--;
				// if ( icol > nrows-1 ) {
				if ( icol < 0 ) {
					// printf("****ERROR PAD 2****\n");
					// exit(-2);
					icol_right++;
					int pos = bin_search(icol_right,nt_row,&(ja[iat[i]]));
					if (icol_right != ja[iat[i]+pos] ) find_icol = false;
				}
				// check if icol is already in the i-row
				int pos = bin_search(icol,nt_row,&(ja[iat[i]]));
				if (icol != ja[iat[i]+pos] ) find_icol = false;
			}

			// add zero term on icol-col of i-row
			int jj = iat[i]+padding_factor*i+nt_row+j;
			irow_padded[jj] = i;
			if(icol<0){
				ja_padded_int[jj]   = icol_right;
			}
			else{
				ja_padded_int[jj]   = icol;
			}
			coef_padded[jj] = 0.;
		}
		// sorting
		if ( nt_add[i] > 0 ) {
			int jj = iat[i]+padding_factor*i;
			heapsort_2v(&(ja_padded_int[jj]),&(coef_padded[jj]),nt_row+nt_add[i]);
		}
	}

	// compaction
	long int ntermA_wp = 0;
	for (int i = 0; i < nrows; i++) {
		int nt_row = iat[i+1]-iat[i];
		int jj = iat[i]+padding_factor*i;
		for (int j = 0; j < nt_row+nt_add[i]; j++) {
			irow_padded[ntermA_wp+j] = irow_padded[jj+j];
			ja_padded_int[ntermA_wp+j]   = ja_padded_int[jj+j];
			coef_padded[ntermA_wp+j] = coef_padded[jj+j];
		}
		ntermA_wp += nt_row+nt_add[i];
	}
	free(nt_add);

	// Assembly iat for the Compressed Sparse Row (CSR) format
	// Note that the matrix is symmetric so CSR format == CSC format
	iat_padded[0] = 0;
	{
		int j = 0;
		for ( int i = 0; i < ntermA_wp; i++) {
		 ja_padded[i].range(0,30) = ja_padded_int[i];
		 if( irow_padded[i] > j ) {
				iat_padded[j+1] = i;
				ja_padded[i-1].range(31,31) = 1;
				j++;
			}
		}
		ja_padded[i].range(31,31) = 0;
	}
	iat_padded[m] = ntermA_wp;
	ja_padded[ntermA_wp-1].range(31,31) = 1;
	nterm_padded=ntermA_wp;
	
	free(irow_padded);
	free(ja_padded_int);

	/*******************************************************************************************/
	float* x_stream;
	RowType* iatbit;
	iatbit = (RowType*) OOPS_malloc((size_t)(nterm*sizeof(RowType)));
	x_stream = (float*) OOPS_malloc((size_t)(nterm*sizeof(float)));
	ColType col;
	for(int i = 0; i < nterm; i++) {
		col = ja_padded[i].range(0,30);
		iatbit[i] = ja_padded[i].range(31,31);
		x_stream[i] = x[col];
	}
	/*******************************************************************************************/

	std::vector<int> stripe_nrows(nstripe);
	std::vector<int> stripe_start_row(nstripe);
	std::vector<int> stripe_start_index(nstripe);
	std::vector<int> stripe_nterm(nstripe);

	// For each Compute Unit (nstripe), specify the number of elements each Compute Unit will be assigned with
	int stripesize = nrows/nstripe;
	int remainder  = nrows%nstripe;
	for (int i = 0; i < nstripe; i++) {
		// set-up stripe info
		if ( i <= remainder ) {
			stripe_nrows[i]        = stripesize + 1;
			stripe_start_row[i]    = i*stripe_nrows[i];
			stripe_start_index[i]  = iat_padded[stripe_start_row[i]];
			if ( i == remainder ) stripe_nrows[i]--;
		} else {
			stripe_nrows[i]        = stripesize;
			stripe_start_row[i]    = i*stripe_nrows[i] + remainder;
			stripe_start_index[i]  = iat_padded[stripe_start_row[i]];
		}
		stripe_nterm[i] = iat_padded[stripe_start_row[i]+stripe_nrows[i]] - stripe_start_index[i];
		printf("ROW_BAL: stripe = %d\tnrows = %d\tnterm = %d\n", i, stripe_nrows[i], stripe_nterm[i]);
	}
	double nterm_min, nterm_max, nterm_avg, nterm_std, nterm_skew;
	calculate_min_max_mean_std_skew(stripe_nterm, nstripe, &nterm_min, &nterm_max, &nterm_avg, &nterm_std, &nterm_skew);

	if(nterm_std/nterm_avg > 1000){
		printf("ROW_BAL: nterm_min = %lf, nterm_max = %lf, nterm_avg = %lf, nterm_std = %lf, nterm_skew = %lf\n", nterm_min, nterm_max, nterm_avg, nterm_std, nterm_skew);
		printf("Need to do NNZ_BAL and not ROW_BAL\n");
		int stripesize2 = nterm_padded/nstripe;
		// printf("NNZ_BAL NOW\tit should be %d nonzeros per CU\n", stripesize2);
		int curr_nterm = 0;
		int curr_nrow = 0;
		int stripe_id = 0;
		stripe_start_row[0] = 0;
		stripe_start_index[0] = 0;
		for(int i=0; i<m; i++){
			// if not in last stripe
			if(stripe_id < (nstripe-1))
			{
				curr_nrow++;
				curr_nterm += (iat_padded[i+1] - iat_padded[i]);
				if(curr_nterm > stripesize2){
					// means we have to cut-off the specific row and move to next stripe
					// next stripe will begin from next row
					// perhaps check one row forward (for better load balancing) - or cut off right before row changes (but need to go back? somehow)
					
					stripe_nrows[stripe_id] = curr_nrow;
					stripe_start_row[stripe_id+1] = i+1;
					stripe_start_index[stripe_id+1] = iat_padded[stripe_start_row[stripe_id+1]];
					// printf("stripe_id = %d\tcurr_nterm = %d\tstripe_nrows = %d\tstripe_start_row = %d\tstripe_start_index = %d\n", stripe_id, curr_nterm, stripe_nrows[stripe_id], stripe_start_row[stripe_id], stripe_start_index[stripe_id]);

					stripe_nterm[stripe_id] = iat_padded[stripe_start_row[stripe_id]+stripe_nrows[stripe_id]] - stripe_start_index[stripe_id];
					// printf("NNZ_BAL stripe = %d\tnrows = %d\tnterm = %d\n", stripe_id, stripe_nrows[stripe_id], stripe_nterm[stripe_id]);

					// reset and move on to next stripe
					curr_nrow = 0;
					curr_nterm = 0;
					stripe_id++;
				}
			}
			else{
				// we are in the last stripe
				// just get what is remaining
				// stripe_start_row will be ok
				// need to handle the rest of them
				stripe_nrows[stripe_id] = nrows - i;// + 1;
				stripe_nterm[stripe_id] = iat_padded[stripe_start_row[stripe_id]+stripe_nrows[stripe_id]] - stripe_start_index[stripe_id];
				printf("NNZ_BAL: stripe = %d\tnrows = %d\tnterm = %d\n", stripe_id, stripe_nrows[stripe_id], stripe_nterm[stripe_id]);
				break;
			}
		}
		calculate_min_max_mean_std_skew(stripe_nterm, nstripe, &nterm_min, &nterm_max, &nterm_avg, &nterm_std, &nterm_skew);
		printf("NNZ_BAL: nterm_min = %lf, nterm_max = %lf, nterm_avg = %lf, nterm_std = %lf, nterm_skew = %lf\n", nterm_min, nterm_max, nterm_avg, nterm_std, nterm_skew);
		// printf("time_rowbal vs time_nnzbal = %lf vs %lf\n", time_rowbal, time_nnzbal);
	}

	/*******************************************************************************************/

	std::vector<cl::Buffer> buffer_coef(nstripe);
	std::vector<cl::Buffer> buffer_iat(nstripe);
	std::vector<cl::Buffer> buffer_x(nstripe);
	std::vector<cl::Buffer> buffer_b(nstripe);

	float* _coef[nstripe];
	float* _b[nstripe];
	float* _x[nstripe];
	RowType* _iat[nstripe];

	for (int i = 0; i < nstripe; i++) {
		if(stripe_nterm[i]!=0){
			_coef[i]= (float*) OOPS_malloc((size_t)(stripe_nterm[i]*sizeof(float)));
			memcpy(_coef[i],coef_padded+stripe_start_index[i],stripe_nterm[i]*sizeof(float));
			// printf("buffer_coef[%d] %4.3lf MB\n", i, stripe_nterm[i]*sizeof(float)/(1024*1024.0));

			_iat[i]= (RowType*) OOPS_malloc((size_t)(stripe_nterm[i]*sizeof(RowType)));
			memcpy(_iat[i],iatbit+stripe_start_index[i],stripe_nterm[i]*sizeof(RowType));
			// printf("buffer_iat[%d] %lf MB\n", i, stripe_nterm[i]*sizeof(RowType)/(1024*1024.0));

			_x[i]= (float*) OOPS_malloc((size_t)(stripe_nterm[i]*sizeof(float)));
			memcpy(_x[i],x_stream+stripe_start_index[i],stripe_nterm[i]*sizeof(float));
			// printf("buffer_x[%d] %lf MB\n", i, stripe_nterm[i]*sizeof(float)/(1024*1024.0));

			_b[i]= (float*) OOPS_malloc((size_t)(stripe_nrows[i]*sizeof(float)));
			// printf("buffer_b[%d] %lf MB\n\n", i, stripe_nrows[i]*sizeof(float)/(1024*1024.0));

			OCL_CHECK(err, buffer_coef[i] = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterm[i]*sizeof(float),  _coef[i], &err));
			OCL_CHECK(err, buffer_iat[i]  = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterm[i]*sizeof(RowType), _iat[i], &err));
			OCL_CHECK(err, buffer_x[i]    = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  stripe_nterm[i]*sizeof(float),     _x[i], &err));
			OCL_CHECK(err, buffer_b[i]    = cl::Buffer(program_interface.context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, stripe_nrows[i]*sizeof(float),     _b[i], &err));
		}
	}

	// Set the Kernel Arguments
	for (int i = 0; i < nstripe; i++) {
		if(stripe_nterm[i]!=0){
			int narg=0;
			printf("Kernel %d nterm %d nrows %d\n", i, stripe_nterm[i], stripe_nrows[i]);
			OCL_CHECK(err, err = krnls[i].setArg(narg++,128)); // number of iterations
			OCL_CHECK(err, err = krnls[i].setArg(narg++,stripe_nrows[i]));
			OCL_CHECK(err, err = krnls[i].setArg(narg++,stripe_nterm[i]));
			OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_iat[i]));
			OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_coef[i]));
			OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_x[i]));
			OCL_CHECK(err, err = krnls[i].setArg(narg++,buffer_b[i]));

			OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({buffer_coef[i],buffer_ja[i],buffer_x[i]}, 0));
		}
	}
	program_interface.q.finish();
	
	// Launch the Kernel
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = program_interface.q.enqueueTask(krnls[i]));
	}
	program_interface.q.finish();

	// Copy Result from Device Global Memory to Host Local Memory
	for (int i = 0; i < nstripe; i++){
		OCL_CHECK(err, err = program_interface.q.enqueueMigrateMemObjects({buffer_b[i]},1));
		if(stripe_nterm[i]!=0){
			// printf("i=%d, stripe_nterm = %d\n", i, stripe_nterm[i]);
			memcpy(b+stripe_start_row[i],_b[i],stripe_nrows[i]*sizeof(float));
		}
	}
	program_interface.q.finish();

	for (int i = 0; i < nstripe; i++){
		clReleaseKernel(krnls[i].get());
	}

	free(iat_padded);
	free(ja_padded);
	free(coef_padded);

	for(int i = 0; i < nstripe; i++){
		free(_coef[i]);
		free(_b[i]);
		free(_ja[i]);
	}
}