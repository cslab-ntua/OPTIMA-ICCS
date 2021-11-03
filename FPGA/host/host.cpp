#include "oops.hpp"
const int N=50, incX=5, incY=3;
#define ROT 11

void main_copy(){
    float* X;
    float* Y;
    X= (float *)malloc(sizeof(float)*N*incX);
    Y= (float *)malloc(sizeof(float)*N*incY);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
	OOPS_copy( N, X, incX, Y, incY);

}

void main_ddot(){
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

    result=OOPS_ddot( N, alpha, X,  incX, Y, incY);
	std::cout<<result<<"  from main with love"<<std::endl;
}

void main_dot(){
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

    result=OOPS_dot( N, X,  incX, Y, incY);
	std::cout<<result<<"  from main with love"<<std::endl;

}

void main_rot(){
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

    OOPS_rot( N, X, incX, Y, incY, c, s);

}

void main_rotm(){
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


    OOPS_rotm( N, X, incX, Y, incY, P);
}

void main_scal(){
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
    float alpha=((float) rand()) / (float) RAND_MAX;

    OOPS_scal(  N,  alpha, X,  incX);
}

void main_swap(){
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

    OOPS_swap( N, X, incX, Y, incY);
}

void main_axpy(){
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

    OOPS_axpy( N, alpha, X, incX, Y, incY);

}

void main_xpay(){
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

    OOPS_axpy( N, alpha, X, incX, Y, incY);

}

void main_asum(){
	float result;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }

    result=OOPS_asum( N, X, incX);
	std::cout<<result<<"  from main with love"<<std::endl;
}

void main_iamax(){
	int result;
    float* X;
    X= (float *)malloc(sizeof(float)*N*incX);
    int x_index=0;
    for (int i = 0; i < N; i++) {
        X[x_index] = ((float) rand()) / (float) RAND_MAX;
        x_index=x_index + incX;
    }
    result=OOPS_iamax( N, X, incX);
	std::cout<<result<<"  from main with love"<<std::endl;
}



int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    //main_copy(); //ok

	//main_ddot(); //ok

	//main_dot(); //ok

	//main_asum(); //ok

	//main_swap();

	//main_iamax(); //ok

	//main_axpy();

	//main_xpay();

	//main_rot();

	main_rotm();

	//main_scal();


    return 0;

}
