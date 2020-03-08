#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int* generateMatrix(int N);
int getElementPosition(int coords[2], int N);
int getElement(int* matrix_ptr, int coords[2], int N);
void transposeElement(int* row_element, int* col_element);
void transposeMatrix(int N, int* matrix_ptr, int num_threads);
void DisplayMatrix(int *matrix, int N);

int main ()
{   
    int N = 4096; //Size of the Matrix
	int num_threads = 8; // Number of threads
    int* matrix = generateMatrix(N);

    //Uncomment the following two lines to view the NxN matrix original matrix output
    /*cout << "Original Matrix";
    DisplayMatrix(matrix, N);*/

    //Start the steady clock
    std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
    startClock = std::chrono::steady_clock::now();

    transposeMatrix(N, matrix, num_threads);

    //Pause the steady clock
    endClock = std::chrono::steady_clock::now();
    std::chrono::duration<double>elapsedTime = duration_cast<duration<double>>(endClock - startClock);

    //Uncomment the following two lines to view the NxN matrix transposition output
    /*cout << "Transposed Matrix";
    DisplayMatrix(matrix, N);*/

    cout << "\nNumber of Threads: " << num_threads << endl;
    cout << "Matrix Size: " << N << " by " << N << " matrix" << endl;
    cout << "Diagonal Transposition Elapsed Time in Seconds: " << elapsedTime.count() << endl; 

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

void transposeElement(int* row_element, int* col_element)
{
    int temp = *row_element;
    *row_element = *col_element;
    *col_element = temp;
}

void transposeMatrix(int N, int* matrix_ptr, int num_threads)
{   
    omp_set_num_threads(num_threads);
    #pragma omp parallel for
    for (int row=0; row<N; row++)
    {   
        for (int col=row; col<N; col++)
        {
            int oldCoords[2] = {row, col};
            int oldPosition = getElementPosition(oldCoords, N);
            int element = getElement(matrix_ptr, oldCoords, N);

            int newCoords[2] = {col, row};
            int newPosition = getElementPosition(newCoords, N);
            transposeElement(matrix_ptr+oldPosition, matrix_ptr+newPosition);
        }
    }
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