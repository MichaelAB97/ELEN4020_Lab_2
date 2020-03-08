// Diagonal Matrix Transposition Algorithm using Threads
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

/* This function assigns each thread a diagonal transposition operation based on the operation index. 
   A mutex lock is used to ensure that the threads wait for the thread that is 
   currently accessing data to complete its operation before the next operation is executed.*/
void *DiagonalThreadTransposition(void *arg)
{
    ThreadDataStruct* diagonalThread = (ThreadDataStruct*)arg;
    int matrix_size = diagonalThread->N;
    int d_Index = diagonalThread->diagonal_index;

        for (int i = d_Index+1; i < matrix_size; ++i)
        {
            int element_coords[2] = {diagonalThread->diagonal_index, i};
            int rowElement = getElementPosition(element_coords,matrix_size);
            element_coords[0] = i;
            element_coords[1] = diagonalThread->diagonal_index;
            int colElement = getElementPosition(element_coords,matrix_size);
            transposeElement(diagonalThread->matrix+rowElement, diagonalThread->matrix+colElement);
        }
        
        pthread_mutex_lock(&diagonal_Lock);
        if(next_diagonal < matrix_size -1)
        {
            diagonalThread->diagonal_index = next_diagonal;
            ++next_diagonal;
        }
        else diagonalThread->diagonal_index = matrix_size - 1;
        pthread_mutex_unlock(&diagonal_Lock);

        if(diagonalThread->diagonal_index == matrix_size-1) EXIT_SUCCESS;
    
    pthread_exit((void*)0);
} 

/* This function creates threads and a struct of threads based on the number of threads defined.*/
void DiagonalThreadManager(int* matrix, int N, int num_threads)
{
    int thread_check;
    pthread_t threads[num_threads];
    ThreadDataStruct threads_traits[num_threads]; 

    /* Updates the operation index for the threads so that if a thread has completed it's
       assigned operation, it will go and execute the next available operation */ 
    next_diagonal = num_threads; 

    // Initializing the information in the struct for each thread and each thread is assigned an operation
    for(int i=0; i < num_threads; ++i)
    {
        
        threads_traits[i].thread_ID = i;
        threads_traits[i].diagonal_index = i;
        threads_traits[i].matrix = matrix;
        threads_traits[i].N = N;

        thread_check = pthread_create(&threads[i], NULL, DiagonalThreadTransposition, &threads_traits[i]);
        
        //Error condition: If the pthreads are created successfully, 0 is returned
        //If threads are not created successfully, an error message will be output and the program will terminate 
        if(thread_check)
        {
            cout<< "Threads were not created successfully"<<endl;
            EXIT_FAILURE;
        }
    }

    //scheduling the joining of the threads
    for(int i = 0; i< num_threads; i++) 
    {
        pthread_join(threads[i],NULL);
    }
}

int main()
{
    int N = 4096; //Size of the Matrix
    int num_threads = 8; // Number of threads
    int* matrix = GenerateMatrix(N);

    //Uncomment the following two lines to view the original NxN Matrix output
    /*cout << "Original Matrix";
    DisplayMatrix(matrix, N);*/

    //Start the steady clock
    std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
    startClock = std::chrono::steady_clock::now();
    
    DiagonalThreadManager(matrix, N, num_threads);
    
    //Pause the steady clock
    endClock = std::chrono::steady_clock::now();
    std::chrono::duration<double>elapsedTime = duration_cast<duration<double>>(endClock - startClock);

    //Uncomment the following two lines to view the NxN matrix transposition output
    /*cout << "\nTransposed Matrix";
    DisplayMatrix(matrix, N);*/

    cout << "Number of Threads: " << num_threads << endl;
    cout << "Matrix Size: " << N << " by " << N << " matrix" << endl;
    cout << "Diagonal Transposition Elapsed Time in Seconds: " << elapsedTime.count() << endl;

    return 0;
}