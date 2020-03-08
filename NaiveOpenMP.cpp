#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int* generateMatrix(int N);
int getElementPosition(int coords[2], int N);
int getElement(int* matrix_ptr, int coords[2], int N);
int* transposeMatrix(int N, int* matrix_ptr);
void DisplayMatrix(int *matrix, int N);

int main ()
{   
    int N = 5; //Size of the Matrix
	//int num_threads = 8; // Number of threads
    int* matrix = generateMatrix(N);

    cout << "Original Matrix";
    DisplayMatrix(matrix, N);

    int* transposedMatrix = transposeMatrix(N, matrix);

    cout << "Transposed Matrix";
    DisplayMatrix(transposedMatrix, N);

    return 0;
}

int* generateMatrix(int N)
{   
    int dimension = N*N;
    int seed = rand()%100;
    srand(seed);

    int *matrix =  (int* )calloc(dimension, sizeof(int));
    int *element_ptr = matrix;

    for (int i = 0; i < dimension; ++i)
    {
        *element_ptr = rand()%N;
        element_ptr++;
    }
    
    return matrix;
}

int getElementPosition(int coords[2], int N)
{
    int row = coords[0];
    int col = coords[1];
    int position = (row*N) + col;
    return position;
}

int getElement(int* matrix_ptr, int coords[2], int N)
{
    return *(matrix_ptr + getElementPosition(coords,N));
}

int* transposeMatrix(int N, int* matrix_ptr)
{   
    int dimension = N*N;
    int *transposedMatrix =  (int* )calloc(dimension, sizeof(int));
    int *transposedElementPtr = transposedMatrix;

     #pragma omp parallel num_threads(3)
    {   
        #pragma omp for
        for (int row=0; row<N; row++)
        {
            for (int col=0; col<N; col++)
            {
            int oldCoords[2] = {row, col};
            int oldPosition = getElementPosition(oldCoords, N);
            int element = getElement(matrix_ptr, oldCoords, N);

            int newCoords[2] = {col, row};
            int newPosition = getElementPosition(newCoords, N);
            *(transposedMatrix + newPosition) = element;
            }
        }
    }

    return transposedMatrix;
}

void DisplayMatrix(int *matrix, int N)
{
    int dimension = N*N;
	for(int i=0; i< dimension; ++i)
	{
		if( i%N == 0)cout << endl;

		cout << " " << *matrix;
		++matrix;
	}
	cout << endl;
}