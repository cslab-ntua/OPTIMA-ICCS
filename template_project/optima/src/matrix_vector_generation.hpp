#ifndef MATRIX_VECTOR_GENERATION_HPP
#define MATRIX_VECTOR_GENERATION_HPP
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <unistd.h>

const float FLAG[4] = {-1.0, 0.0, 1.0, -2.0};

inline int max_(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

/**
 * Find minimum between two numbers.
 */
inline int min_(int num1, int num2)
{
    return (num1 > num2 ) ? num2 : num1;
}

inline void vector_N(float *X,int N,int incX){
      int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
}

inline void std_vector_N(float *X,int N,int incX, float num){
      int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = num;
        x_index=x_index + incX;
    }
}

inline void pls_std_vector_N(float *X,int N,int incX){
      int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = float(i);
        x_index=x_index + incX;
    }
}

inline void gen_rotm_param(float *fH){
//	float flag[4] = {-1.0, 0.0, 1.0, -2.0};
	srand(time(NULL));
	float chosen_flag = FLAG[rand()%4]; //Choose randomly between the 4 values of the flag array
	fH[0] = chosen_flag;
	#ifdef DEBUG_MATR
	std::cout << "Chosen flag is :" << chosen_flag << std::endl;
	#endif
	if(chosen_flag == 0.0)
	{
		fH[1] = 1.0;
		fH[2] = ((float) rand()) / (float) RAND_MAX;
		fH[3] = ((float) rand()) / (float) RAND_MAX;
		fH[4] = 1.0;
	}
	else if(chosen_flag == 1.0)
	{
		fH[1] = ((float) rand()) / (float) RAND_MAX;
		fH[2] = 1.0;
		fH[3] = -1.0;
		fH[4] = ((float) rand()) / (float) RAND_MAX;
	}
	else if(chosen_flag == -2.0)
	{
		fH[1] = 1.0;
		fH[2] = 0.0;
		fH[3] = 0.0;
		fH[4] = 1.0;
	}
	else
	{
		fH[1] = ((float) rand()) / (float) RAND_MAX;
		fH[2] = ((float) rand()) / (float) RAND_MAX;
		fH[3] = ((float) rand()) / (float) RAND_MAX;
		fH[4] = ((float) rand()) / (float) RAND_MAX;
	}

}

// inline void banded_MxN_matrix(float *A,int rows,int columns,int KU, int KL){
//     float *Atemp;
//     Atemp= (float *)OOPS_malloc(sizeof(float)*rows*columns);
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0 ;j < columns; j++){
//             //Atemp[i*columns+j] = ((float) rand()) / (float) RAND_MAX;
//             Atemp [i*columns+j]=i*10+j;
//             std::cout <<"Atemp["<<i<<"]["<<j<<"]="<<Atemp[i*columns+j]<<std::endl;
//         }
//     }
//     for (int j=0;j<columns;j++){
//         for (int i=std::max(0,j-KU);i<std::min(rows,j+KL+1);i++){
//             A[(KU+i-j)*columns+j]=Atemp[i*columns+j];
//         }
//     }
//     free (Atemp);

// }

inline void banded_MxN_matrix(float *A, float *Apadded,int rows,int columns,int KU, int KL){
    float* Atemp= (float *)malloc(sizeof(float)*rows*columns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0 ;j < columns; j++){
            Atemp[i*columns+j] = ((float) rand()) / (float) RAND_MAX;
        }
    }
    //column-layout
    /*for (int j=0;j<columns;j++){
        for (int i=std::max(0,j-KU);i<std::min(rows,j+KL+1);i++){
            A[(KU+i-j)*columns+j]=Atemp[i*columns+j];
        }
    }*/
        int N_cols = ((KU+KL)/16 + 1)*16;

    //row-layout
    for (int i=0;i<rows;i++){
        for (int j=std::max(0,i-KL);j<std::min(columns,i+KU+1);j++){
                A[KL-i+j+i*(KU+KL+1)]=Atemp[i*rows+j];
        }
    }

    for (int i=0;i<rows;i++){
        for (int j=0;j<KU+KL+1;j++){
                Apadded[i*N_cols+j]=A[i*(KU+KL+1)+j];
        }
    }

    /*for (int j=0;j<rows;j++){
        for (int i=0;i<KU+KL+1;i++){
            std::cout <<A[j*(KU+KL+1)+i]<<"    ";
        }
        std::cout <<endl;
    }
    std::cout <<endl;
    std::cout <<endl;
    std::cout <<endl;

    for (int j=0;j<rows;j++){
        for (int i=0;i<N_cols;i++){
            std::cout <<Apadded[j*N_cols+i]<<"    ";
        }
        std::cout <<endl;
    }*/

    free (Atemp);
}

inline void trband_matrix(char Uplo,float *A, float *Apadded,int N,int K){

    
	float *Atemp= (float *)malloc(sizeof(float)*N*N);

    	for (int i = 0; i < N; i++) {
    		for (int j = 0 ;j < N; j++){
    			Atemp[i*N+j] = ((float) rand()) / (float) RAND_MAX;
    		}
    	}

	int N_cols = (K/16 + 1)*16;

    	//row-layout
	if(Uplo == 'U'){
		for (int i=0;i<N;i++){
			for (int j=i;j<std::min(N,i+K+1);j++){
				A[-i+j+i*(K+1)]=Atemp[i*N+j];
			}
		}

		for (int i=0;i<N;i++){
			for (int j=0;j<K+1;j++){
				Apadded[i*N_cols+j]=A[i*(K+1)+j];
			}
		}
	}

	free(Atemp);

}

inline void symmetric_matrix(float *A,int N){
    for (int i = 0; i < N; i++) {
        for (int j = i ;j < N; j++){
            A[i*N+j] = ((float) rand()) / (float) RAND_MAX;
            A[j*N+i] = A[i*N+j];
        }
    }
}
inline void symmetric_banded_matrix();
inline void symmetric_packed_matrix();
inline void triangular_NxN_matrix(const char Uplo,float *A, int N){
    if (Uplo == 'U' or Uplo == 'u'){
        for (int i = 0; i < N; i++) {
            for (int j = i ;j < N; j++){
                A[i*N+j] = ((float) rand()) / (float) RAND_MAX;
            }
        }
    }
    else if (Uplo == 'L' or Uplo == 'l'){
        for (int i = 0; i < N; i++) {
            for (int j = 0 ;j < i+1; j++){
                A[i*N+j] = ((float) rand()) / (float) RAND_MAX;
            }
        }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }
}
inline void triangular_NxN_matrix_pls1(const char Uplo,float *A, int N){
	int k = 1;
    if (Uplo == 'U' or Uplo == 'u'){
        for (int i = 0; i < N; i++) {
            for (int j = i ;j < N; j++){
                A[i*N+j] = float(k);
                k++;
            }
        }
    }
    else if (Uplo == 'L' or Uplo == 'l'){
        for (int i = 0; i < N; i++) {
            for (int j = 0 ;j < i+1; j++){
                A[i*N+j] = float(k);
                k++;
            }
        }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }
}
inline void triangular_banded_MxN_matrix();
inline void MxN_matrix(float *A,int rows,int columns){
    for (int i = 0; i < rows; i++) {
        for (int j = 0 ;j < columns; j++){
            A[i*columns+j] = ((float) rand()) / (float) RAND_MAX;
        }
    }
}


inline void convert_triangular_matrix_to_packed(const char Uplo,float *A, float *A_new, uint32_t N, uint32_t packedMatrixSize)
{
	uint32_t i=0;
	uint32_t j=0;
	uint32_t k=0;
	if (Uplo == 'U' or Uplo == 'u'){
		while (k<packedMatrixSize)	{
			A_new[k] = A[i*N+j];
			k++;
			if(j<N-1){
				j++;
			}
			else {
				i++;
				j=i;
			}
		}
	}
	else if (Uplo == 'L' or Uplo == 'l'){
        while (k<packedMatrixSize) {
        	A_new[k] = A[i*N+j];
        	k++;
        	if(i==j){
        		i++;
        		j=0;
        	}
        	else{
        		j++;
        	}
        }
    }
    else {
        std::cout<<"Define Uplo"<<std::endl;
    }
}

inline void printMatrix(float *A, uint32_t rows, uint32_t columns){
//	printf("Tab[]:\n");
	for (uint32_t i = 0; i < rows; i++) {
			for (uint32_t j = 0 ;j < columns; j++){
				printf(" %f ",A[i*columns+j]);
			}
			printf("\n");
			}
}

inline void printVector(float *X, uint32_t N){
//	printf("Vec:\n");
	for (uint32_t i = 0; i < N; i++) {
		printf(" %f ",X[i]);
	}
	printf("\n");
}

inline uint32_t calc_packed_matrix_usefull_data(uint32_t N)
{
	uint32_t new_size=0;
	for (uint32_t i=0; i<N; i++)
	{
		new_size += (i+1);
	}
	return new_size;
}

inline void triang_packed_matrix_vector_mult(const char Uplo, float *A, float *X, float *resultVec, uint32_t packedMatrixSize, uint32_t vecSize)
{
	uint32_t mIdx;
	uint32_t limit;
	uint32_t vCurrIdx;
	uint32_t rVidx;
	uint32_t limitIncr;

	if(Uplo=='U' or Uplo=='u')
	{
		mIdx = 0;
		limit = vecSize;
		vCurrIdx=0;
		rVidx=0;
		limitIncr = vecSize-1;// limitIncr here is the number of usefull elements in a row of the matrix
		while(mIdx < packedMatrixSize)
			{
				resultVec[rVidx] += A[mIdx]*X[vCurrIdx];

				if(mIdx<limit-1)
				{
					vCurrIdx++;
				}
				else
				{
					limit = limit + limitIncr;
					limitIncr--;
					rVidx++;
					vCurrIdx = rVidx;
				}
				mIdx++;
			}
	}
	else if(Uplo == 'L' or Uplo == 'l')
	{
		mIdx = 0;
		limit = 1;
		vCurrIdx=0;
		rVidx=0;
		limitIncr = 2;
		while(mIdx < packedMatrixSize)
		{
			resultVec[rVidx] += A[mIdx]*X[vCurrIdx];

			if(mIdx<limit-1)
			{
				vCurrIdx++;
			}
			else
			{
				limit = limit + limitIncr;
				limitIncr++;
				rVidx++;
				vCurrIdx = 0;
			}
			mIdx++;
		}
	}
	else
	{
		std::cout<<"Define Uplo"<<std::endl;
	}

}

inline void triang_packed_matrix_vector_mult_prl_read(const char Uplo, float *A, float *X, float *resultVec, uint32_t packedMatrixSize , uint32_t extendedPackedMatrixSize, uint32_t vecSize)
{
	uint32_t mIdx;
	uint32_t rowElemsLimit;
	uint32_t vCurrIdx;
	uint32_t rVidx;
	uint32_t usefulRowElems;
	uint32_t memAddr=0;
	uint32_t parallelNumbersRead = 4;
	uint32_t nextRowElemsLimit = 0;
	uint8_t nextRowFlag = 0;



	if(Uplo=='U' or Uplo=='u')
	{
		mIdx = 0;
		rowElemsLimit = vecSize; // to idx tou oriou twn stoixeiwn thn grammhs tou monodiastatou packed pinaka
		vCurrIdx=0;
		rVidx=0;
		usefulRowElems = vecSize-1;// o ari8mos xrhsimwn stoixeiwn ths grammhs tou trigwnikou pinaka
		while(memAddr < extendedPackedMatrixSize)
		{
			printf("Mesa sto while\n");
			if(memAddr + (parallelNumbersRead - 1)  < rowElemsLimit)// shmainei oti oi ari8moi pou diavazoume apo thn mnhmh einai sthn idia grammh
			{
				if(nextRowFlag==0)
				{
					printf("1.Kanonikh ektelesh\n");
					while(mIdx < memAddr + parallelNumbersRead  )// To loop prepei na kanei praxeis me osa stoixeia hr8an apo to parallhlo diavasma sthn mnhma
					{
						resultVec[rVidx] += A[mIdx]*X[vCurrIdx];
						printf("### rVidx = %u, mIdx = %u, vCurrIdx =  %u ###\n", rVidx, mIdx, vCurrIdx);
						mIdx++;
						vCurrIdx++;
					}
				}
				else
				{
					printf("2.FlagAyros shkwneiros\n");
					while(mIdx < nextRowElemsLimit)// edw to loop kanei praxeis me ta ypoloipa stoixeia pou eixe diavasei prin apo thn mnhmh
					{
						resultVec[rVidx] += A[mIdx]*X[vCurrIdx];
						printf("*** rVidx = %u, mIdx = %u, vCurrIdx =  %u ***\n", rVidx, mIdx, vCurrIdx);
						mIdx++;
						vCurrIdx++;
					}
					nextRowFlag=0;
				}

			memAddr+=parallelNumbersRead; //Ftasame ws edw gt ta stoixeia pou fernei apo thn mnhmh einai mesa sta oria ths grammhs tou pinaka
						// opote fernoume ta epomena
			}
			else // an mpei sto else shmainei oti ta data ap thn mnhmh periexoun stoixeia apo 2 grammes
			{	// opote prepei na ta 3exwrisoume
				nextRowElemsLimit = memAddr + parallelNumbersRead;
				printf("3.Digrameiros otinaneiros\n");
				while(mIdx < rowElemsLimit)// pollaplasiazoume kai pros8etoume ta ypoloipa stoixeia ths grammhs tou pinaka
				{
					resultVec[rVidx] += A[mIdx]*X[vCurrIdx];
					printf("!!! rVidx = %u, mIdx = %u, vCurrIdx =  %u !!!\n", rVidx, mIdx, vCurrIdx);
					mIdx++;
					vCurrIdx++;
				}
				nextRowFlag = 1;
				rowElemsLimit = rowElemsLimit + usefulRowElems ;// afxanoume to limit ths grammhs ston monodiastato packed trigwniko pinaka
				usefulRowElems--; // ta xrhsima stoixeia ths grammhs meiwnontai kata 1 afou pame mia grammh parakatw
				rVidx++;			// pame 1 parakatw to idx tou result vector
				vCurrIdx = rVidx;	//to idx tou vector ginetai iso me to result vector kathos ta prwta stoixeia tou pinaka einai mhdenika ston anw trigwniko oso proxwrame se grammes
				printf("``` rowElemsLimit = %u, usefulRowElems = %u, rVidx =  %u, vCurrIdx =  %u ```\n", rowElemsLimit, usefulRowElems, rVidx, vCurrIdx);
				/*
				 * Oi parakatw 7 grammes douleoun apla epelexa ton parapanw tropo
				 * gia na moiazei o algori8mos perissotero me th HW ylopoihsh
				 */
//				while(mIdx < nextRowElemsLimit)
//				{
//					resultVec[rVidx] += A[mIdx]*X[vCurrIdx];
//					mIdx++;
//					vCurrIdx++;
//				}
//				memAddr+=16;


			}
		}
	}
	else if(Uplo == 'L' or Uplo == 'l')
	{
//		mIdx = 0;
//		limit = 1;
//		vCurrIdx=0;
//		rVidx=0;
//		limitIncr = 2;
//		while(mIdx < packedMatrixSize)
//		{
//			resultVec[rVidx] += A[mIdx]*X[vCurrIdx];
//
//			if(mIdx<limit-1)
//			{
//				vCurrIdx++;
//			}
//			else
//			{
//				limit = limit + limitIncr;
//				limitIncr++;
//				rVidx++;
//				vCurrIdx = 0;
//			}
//			mIdx++;
//		}
	}
	else
	{
		std::cout<<"Define Uplo"<<std::endl;
	}

}

inline uint32_t calc_FPGA_packed_matrix_usefull_data(uint32_t N, uint32_t VDATA_SIZE)
{
	uint32_t new_size=0;
	uint32_t i = 0;
	for ( i=0; i<N; i++)
	{
		if( (i+1)%VDATA_SIZE==0)
			new_size += (i+1);
		else
			new_size += ((i+1)/VDATA_SIZE + 1) * VDATA_SIZE;
	}
	return new_size;
}

inline uint32_t cmpt_hyperpacked_triang_mtrx_blks(uint32_t N, uint32_t VDATA_SIZE ){
	uint32_t first_term = ( N%VDATA_SIZE ) * ( N/VDATA_SIZE + 1 );
//	printf("first term = %u\n",first_term);
	uint32_t sec_term = (VDATA_SIZE * (( N/VDATA_SIZE ) * (1 + (N/VDATA_SIZE) ))/2);
//	printf("sec term = %u\n",sec_term);
	uint32_t res = (first_term + sec_term );
	return res;
}



//inline void convert_packed_matrix_to_Hyperpacked(const char Uplo, float *Apacked, uint32_t packedSize, float *FPGApacked, uint32_t fpgaPackedSize, uint32_t N, uint32_t VDATA_SIZE)
//{
//	uint32_t limit=N;
//
//	uint32_t fp_idx = 0;
//	uint32_t ap_idx = 0;
//	uint32_t usefulRowElems = N;
//
//
//
//	if ((Uplo == 'U') || (Uplo == 'u')){
//		 printf("Mphka uplo U\n");
//		while (ap_idx < packedSize )
//		{
//			if ( (usefulRowElems%VDATA_SIZE) != 0)
//			{
//				while( (fp_idx%VDATA_SIZE) < VDATA_SIZE - (usefulRowElems%VDATA_SIZE)  )
//				{
////					 printf("Case 1 fp_idx = %u \n", fp_idx);
//					FPGApacked[fp_idx] = 0 ;
//					fp_idx++;
//
//				}
//			}
//
//			while(ap_idx < limit )
//			{
////				 printf("Case 2 fp_idx = %u, ap_idx = %u, Apacked[%u] = %f \n", fp_idx, ap_idx, ap_idx, Apacked[ap_idx]);
//				 FPGApacked[fp_idx] = Apacked[ap_idx];
////				FPGApacked[fp_idx] = ap_idx+1;
//				fp_idx++;
//				ap_idx++;
//
//			}
//			limit = limit + usefulRowElems - 1;
//			usefulRowElems--;
////			 printf("limit = %u usefulRowElems = %u \n ",limit,usefulRowElems);
//		}
//	}
//	else if ((Uplo == 'L') || (Uplo == 'l')){
//       printf("Mphka uplo L\n");
//    }
//    else {
//		printf("Define Uplo\n");
//    }
//
//}


 void print_Hyperpacked(float *fpgaPacked, uint32_t N, uint32_t VDATA_SIZE)
{
	uint32_t fpgaPackedSize = calc_FPGA_packed_matrix_usefull_data(N,VDATA_SIZE);
	uint32_t i = 0;
	uint32_t usefulRowElems = N;
	uint32_t usefulRowElems4Print = 0;

	printf("!@#$ fpgaPackedSize = %u \n", fpgaPackedSize);

	if (N%VDATA_SIZE==0)
	{
		usefulRowElems4Print = N;
		printf("1.\n");
	}
	else
	{
		usefulRowElems4Print = ( (N/VDATA_SIZE) + 1 ) * VDATA_SIZE ;
		printf("2.\n");
	}

	while (i < fpgaPackedSize)
	{
		printf(". i=%u %u\n",i,fpgaPackedSize);
		if( ((i%usefulRowElems4Print) == 0) && (i!=0))
		{
			printf("\n");
			usefulRowElems--;
		}
		if(usefulRowElems%VDATA_SIZE==0)
		{
			usefulRowElems4Print-=VDATA_SIZE;
		}
		printf(" %f ",fpgaPacked[i]);
		i++;

	}

}


#endif

