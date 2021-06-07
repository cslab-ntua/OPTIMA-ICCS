#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void print(double **matrix, int matrixSize);
double** matrixAddition(double **matrix1, double **matrix2, int matrixSize);
double** matrixMultiply(double **matrix1, double **matrix2, int matrixSize);
double* vectorMultiply(double **matrix, double *vector, int matrixSize, int vectorSize);
double** transpose(double **matrix, int matrixSize);
double** initialize(int minValue, int maxValue, int matrixSize);
double** initMatrix(int size);
void transCopy(double ** source, double ** dest, int size);
void copyMatrix(double ** source, double ** dest, int size);

//Vector functions
void printVector(double *vector, int size);
double computeSumofAbsError( double* vector1 , double* vector2, int size);
