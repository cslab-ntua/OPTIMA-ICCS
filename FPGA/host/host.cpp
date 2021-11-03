#include "oops.h"
unsigned int DATA_SIZE;
const int N=50, incX=5, incY=3;

void main_copy(char* arg1){
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
	OOPS_copy( N, X, incX, Y, incY,arg1);

}

void main_ddot(char* arg1){
    float* X;
    float* Y;
    double result;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    result=OOPS_ddot( N, alpha, X,  incX, Y, incY,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;
}

void main_dot(char* arg1){
    float* X;
    float* Y;
    float result;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }

    result=OOPS_dot( N, X,  incX, Y, incY,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;

}

void main_rot(char* arg1){
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float c=((float) rand()) / (float) RAND_MAX,s=((float) rand()) / (float) RAND_MAX;

    OOPS_rot( N, X, incX, Y, incY, c, s,arg1);

}

void main_rotm(char* arg1){
    float* X;
    float* Y;
    float* P;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
	P= (float *)malloc(sizeof(float)*5);
    int x_index=0;
    int y_index=0;
	P[0]=((float)(1));
	//P[0]=((float)(0));
	//P[0]=((float)(-1));
	for (int i=1;i<5;i++){
		P[i]=((float) rand())/(float) RAND_MAX;

	}
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }


    OOPS_rotm( N, X, incX, Y, incY, P,arg1);
}

void main_scal(char* arg1){
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    OOPS_scal(  N,  alpha, X,  incX,arg1);
}

void main_swap(char* arg1){
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }

    OOPS_swap( N, X, incX, Y, incY,arg1);
}

void main_axpy(char* arg1){
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    OOPS_axpy( N, alpha, X, incX, Y, incY,arg1);

}

void main_xpay(char* arg1){
	float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    int y_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        Y[y_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
        y_index=y_index + incY;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    OOPS_axpy( N, alpha, X, incX, Y, incY,arg1);

}

void main_asum(char* arg1){
	float result;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }

    result=OOPS_asum( N, X, incX,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;
}

void main_iamax(char* arg1){
	int result;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
    result=OOPS_iamax( N, X, incX,arg1);
	std::cout<<result<<"  from main with love"<<std::endl;
}



int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    //main_copy(argv[1]); //ok

	//main_ddot(argv[1]);

	//main_dot(argv[1]); //ok

	//main_asum(argv[1]); //ok

	//main_swap(argv[1]);

	//main_iamax(argv[1]); //ok

	//main_axpy(argv[1]);

	//main_xpay(argv[1]);

	//main_rot(argv[1]);

	main_rotm(argv[1]);

	//main_scal(argv[1]);

    return 0;

}
