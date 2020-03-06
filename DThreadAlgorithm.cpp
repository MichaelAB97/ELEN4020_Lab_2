//------------------------------------------------------------------------//
// Diagonal Matrix Transposition Algorithm using Threads
//------------------------------------------------------------------------//

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <time.h> 
#include <chrono>

using namespace std;
using namespace std::chrono;

// Defining the number threads to be used in the program
#define num_threads 8


// Operation Counter
int next_diagonal  = 0;


// Mutex Lock
pthread_mutex_t diagonal_Lock;


// Struct containing data the threads need in order to execute different operations
typedef struct{
    int diagonal_index;
    int thread_ID;
    int *matrix;
    int N;
}ThreadDataStruct;


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

int getElementPosition2D(int coords[2], int N)
{
    int row = coords[0];
    int col = coords[1];
    int position = (row*N) + col;
    return position;
}


int getElement(int* matrix_ptr, int coords[2], int N)
{
    return *(matrix_ptr + getElementPosition2D(coords,N));
}

void transposeElement(int* row_element, int* col_element)
{
    int temp = *row_element;
    *row_element = *col_element;
    *col_element = temp;
}


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



int* allocateMatrix(int N)
{
    int dimension = N*N;
    int* result = (int*)calloc(dimension, sizeof(int));
    return result;
}



void *DiagonalThreadTransposition(void *arg)
{
    ThreadDataStruct* diagonalThread = (ThreadDataStruct*)arg;
    int matrix_size = diagonalThread->N;
    int d_Index = diagonalThread->diagonal_index;

        for (int i = d_Index+1; i < matrix_size; ++i)
        {
            int element_coords[2] = {diagonalThread->diagonal_index, i};
            int rowElement = getElementPosition2D(element_coords,matrix_size);
            element_coords[0] = i;
            element_coords[1] = diagonalThread->diagonal_index;
            int colElement = getElementPosition2D(element_coords,matrix_size);
            transposeElement(diagonalThread->matrix+rowElement, diagonalThread->matrix+colElement);
        }
        
        pthread_mutex_lock(&diagonal_Lock);
        if(next_diagonal < matrix_size -1)
        {
            diagonalThread->diagonal_index = next_diagonal;
            ++next_diagonal;
            cout << next_diagonal << endl;
        }
        else diagonalThread->diagonal_index = matrix_size - 1;
        pthread_mutex_unlock(&diagonal_Lock);

        if(diagonalThread->diagonal_index == matrix_size-1) exit(-1);
    
    pthread_exit((void*)0);
} 


void DiagonalThreadManager(int* matrix, int N)
{
    int thread_check;
    pthread_t thread_matrix[num_threads];
    ThreadDataStruct threads_traits[num_threads]; 



    next_diagonal = (int)num_threads; 



    for(int i=0; i < num_threads; ++i)
    {
        
        threads_traits[i].thread_ID = i;
        threads_traits[i].diagonal_index = i;
        threads_traits[i].matrix = matrix;
        threads_traits[i].N = N;



        thread_check = pthread_create(&thread_matrix[i], NULL, DiagonalThreadTransposition, &threads_traits[i]);
        


        if(thread_check)
        {
            cout<< "ERROR creating thread."<<endl;
            exit(-1);
        }
    }


    for(int i = 0; i< num_threads; i++) 
    {
        pthread_join(thread_matrix[i],NULL);
    }
}

int main()
{
    int N = 128; //Size of the Matrix
    int* matrix = GenerateMatrix(N);

    cout << "Original Matrix";
    DisplayMatrix(matrix, N);

    //Start the steady clock
    std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
    startClock = std::chrono::steady_clock::now();
    
    DiagonalThreadManager(matrix, N);
    
    //Pause the steady clock
    endClock = std::chrono::steady_clock::now();
    std::chrono::duration<double>elapsedTime = duration_cast<duration<double>>(endClock - startClock);

    cout << "\nTransposed Matrix";
    DisplayMatrix(matrix, N);

    cout << "\nElapsed Time in Seconds: " << elapsedTime.count() << endl;

    return 0;
}