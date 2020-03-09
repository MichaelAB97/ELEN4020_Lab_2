// Diagonal Transposition Algorithm using OpenMp
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

/* This function dynamically generates a pointer to the first element of the matrix and populates 
   the matrix with random numbers according to a randomly generated seed value between 0 and 100 */
int* GenerateMatrix(int N)
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

/* This function makes use of an elements coordinates in the 2D matrix and returns the
   element's position in the 1D matrix that was initially created. */
int getElementPosition(int coords[2], int N)
{
    int row = coords[0];
    int col = coords[1];
    int position = (row*N) + col;
    return position;
}

/* This function takes in two elements and assigns each variable the other variable's value
   by the use of a temp variable. For example: if a = 2 & b = 1, the result of the transposElement 
   function will be a = 1 and b = 2 */
void transposeElement(int* row_element, int* col_element)
{
    int temp = *row_element;
    *row_element = *col_element;
    *col_element = temp;
}

//This Function displays the 2D matrix
void DisplayMatrix(int* matrix, int N)
{
    int dimension = N*N;

    for (int i = 0; i < dimension; ++i)
    {
        if (i % N == 0)
        {
            cout << endl;
        }
        cout << *matrix << " ";
        ++matrix;
    }
    cout << endl;

}

/*This function does the Diagonal Transposition using OpenMP
  for a matrix that is passed by pointer*/
void DiagonalTransposition(int* matrix, int N, int num_threads)
{   
    omp_set_num_threads(num_threads); //set number of threads

    #pragma omp parallel private(i,j)
    {
        #pragma omp for schedule(dynamic) nowait
        for (int i = 0; i < N-1; ++i)
        {
            for (int j = i+1; j < N; ++j)
            {
                int element_coords[2] = {i,j};
                int element_position_1 = getElementPosition(element_coords,N);
                element_coords[0] = j;
                element_coords[1] = i;
                int element_position_2 = getElementPosition(element_coords,N);
                transposeElement(matrix+element_position_1, matrix+element_position_2);
            }
        } 
    }
}


int main()
{
    int N = 128;
    int num_threads = 8;
    int* matrix = GenerateMatrix(N);

    //Uncomment the following two lines to view the NxN matrix original matrix output
    /*cout << "Original Matrix";
    DisplayMatrix(matrix, N);*/

    //Start the steady clock
    std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
    startClock = std::chrono::steady_clock::now();
    
    DiagonalTransposition(matrix, N, num_threads);
    
    //Pause the steady clock
    endClock = std::chrono::steady_clock::now();
    std::chrono::duration<double>elapsedTime = duration_cast<duration<double>>(endClock - startClock);

    //Uncomment the following two lines to view the NxN matrix transposition output
    /*cout << "\nTransposed Matrix";
    DisplayMatrix(matrix, N);*/

    cout << "\nNumber of Threads: " << num_threads << endl;
    cout << "Matrix Size: " << N << " by " << N << " matrix" << endl;
    cout << "Diagonal Transposition Elapsed Time in Seconds: " << elapsedTime.count() << endl; 

    return 0; 
}