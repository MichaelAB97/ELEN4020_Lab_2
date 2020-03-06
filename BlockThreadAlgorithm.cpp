// Diagonal Matrix Transposition Algorithm using Threads

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
int next_block_diagonal  = 0;


// Mutex Lock
pthread_mutex_t BlockLock;


// Struct containing data the threads need in order to execute different operations
typedef struct{
    int diagonal_index;
    int thread_ID;
    int block_ID;
    int *matrix;
    int N;
}BlockThreadStruct;


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

int getElementCoords(int coords[2], int N)
{
	int row = coords[0];
	int col = coords[1];
    int position = ( row * N ) + col;

    return position;
}

int *blockElement(int blockIndex, int N)
{	
	int block_number = N/2;
	int *block_element_ptr = (int*)calloc(4,sizeof(int));
	
	*block_element_ptr =  (2*blockIndex)+ (blockIndex/block_number)*N;
	block_element_ptr[1] = *block_element_ptr+1; 
	block_element_ptr[2] = *block_element_ptr+N;
	block_element_ptr[3] = *block_element_ptr+(N+1);

	return block_element_ptr;
}

void transposeElement(int* row_element, int* col_element)
{
	int temp = *row_element;
    *row_element = *col_element;
    *col_element = temp;
}

void blockElementsTranspose(int *matrix, int *block_ij, int *block_ji)
{	
    for (int i = 0; i < 4; i++)
    {
        transposeElement(matrix+block_ij[i], matrix+block_ji[i]);
    }
}

void blockTranspose(int *matrix, int *block_ptr)
{
	transposeElement(matrix+block_ptr[1], matrix+block_ptr[2]);
}

void *BlockThreadTransposition(void *arg)
{
	BlockThreadStruct* block_thread = (BlockThreadStruct*)arg;
	int matrix_size = block_thread->N;
	int block_number = matrix_size/2;
	
	for(int i=block_thread->diagonal_index; i<block_number; ++i)
	{
		int element_coords[2] = {block_thread->diagonal_index,i};
		int block_1 = getElementCoords(element_coords, block_number);
		int *blockElements_1 = blockElement( block_1, matrix_size);
		blockTranspose(block_thread->matrix, blockElements_1);
			
		if(i !=block_thread->diagonal_index)
		{
			element_coords[0] = i;
			element_coords[1] = block_thread->diagonal_index;		

			int block_2 = getElementCoords(element_coords, block_number);
			int *blockElements_2 = blockElement( block_2, matrix_size);
			blockTranspose(block_thread->matrix, blockElements_2);
			blockElementsTranspose(block_thread->matrix, blockElements_1, blockElements_2);
		}
	}
	
	pthread_mutex_lock(&BlockLock);
	if(next_block_diagonal < block_number-1) 
	{
		block_thread->diagonal_index = next_block_diagonal;
		++next_block_diagonal;
	} else block_thread->diagonal_index = block_number-1;
	pthread_mutex_unlock(&BlockLock);
		
	if(block_thread->diagonal_index == block_number-1)EXIT_SUCCESS;
	
	pthread_exit((void *) 0);	
}

void BlockThreadManager(int* matrix, int N)
{
	int thread_check;
	
	pthread_t threads[num_threads];
	BlockThreadStruct threads_traits[num_threads];
	
	next_block_diagonal = num_threads;
	
	for(int i=0; i < num_threads; ++i)
	{
		threads_traits[i].thread_ID = i;
		threads_traits[i].diagonal_index = i;
		threads_traits[i].matrix = matrix;
		threads_traits[i].N = N;
		threads_traits[i].block_ID = i;
		
		thread_check = pthread_create(&threads[i], NULL, BlockThreadTransposition, &threads_traits[i]);
		
		if(thread_check)
		{
			cout << "Thread not Created";
			EXIT_FAILURE;
		}
	}
	
	for(int j = 0; j < num_threads; ++j) pthread_join(threads[j], NULL);
}

int main()
{
    int N = 4; //Size of the Matrix
    int* matrix = GenerateMatrix(N);

    cout << "Original Matrix";
    DisplayMatrix(matrix, N);

    //Start the steady clock
    std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
    startClock = std::chrono::steady_clock::now();

    BlockThreadManager(matrix, N);

    //Pause the steady clock
    endClock = std::chrono::steady_clock::now();
    std::chrono::duration<double>elapsedTime = duration_cast<duration<double>>(endClock - startClock);

    cout << "\nTransposed Matrix";
    DisplayMatrix(matrix, N);

    cout << "\nElapsed Time in Seconds: " << elapsedTime.count() << endl;

    return 0;
}