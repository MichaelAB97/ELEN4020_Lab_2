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

//This Function displays the 2D matrix
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

/* This function makes use of an elements coordinates in the 2D matrix and returns the
   element's position in the 1D matrix that was initially created. */
int getElementPosition(int coords[2], int N)
{
	int row = coords[0];
	int col = coords[1];
    int position = ( row * N ) + col;

    return position;
}

/* This function returns a pointer to the first element of a block */
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

/* This function takes in two elements and assigns each variable the other variable's value
   by the use of a temp variable. For example: if a = 2 & b = 1, the result of the transposElement 
   function will be a = 1 and b = 2 */
void transposeElement(int* row_element, int* col_element)
{
	int temp = *row_element;
    *row_element = *col_element;
    *col_element = temp;
}

/* This function does matrix transposition on a single block */
void blockElementsTranspose(int *matrix, int *block_ij, int *block_ji)
{	
    for (int i = 0; i < 4; i++)
    {
        transposeElement(matrix+block_ij[i], matrix+block_ji[i]);
    }
}

/* This function transposes the first elements of the blocks needed to be transposed */
void blockTranspose(int *matrix, int *block_ptr)
{
	transposeElement(matrix+block_ptr[1], matrix+block_ptr[2]);
}

/* This function assigns each thread a transposition operation based on the operation index. 
   A mutex lock is used to ensure that the threads wait for the thread that is 
   currently accessing data to complete its operation before the next operation is executed.*/
void *BlockThreadTransposition(void *arg)
{
	BlockThreadStruct* block_thread = (BlockThreadStruct*)arg;
	int matrix_size = block_thread->N;
	int block_number = matrix_size/2;
	
	for(int i=block_thread->diagonal_index; i<block_number; ++i)
	{
		int element_coords[2] = {block_thread->diagonal_index,i};
		int block_1 = getElementPosition(element_coords, block_number);
		int *blockElements_1 = blockElement( block_1, matrix_size);
		blockTranspose(block_thread->matrix, blockElements_1);
			
		if(i !=block_thread->diagonal_index)
		{
			element_coords[0] = i;
			element_coords[1] = block_thread->diagonal_index;		

			int block_2 = getElementPosition(element_coords, block_number);
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

/* This function creates threads and a struct of threads based on the number of threads defined.*/
void BlockThreadManager(int* matrix, int N, int num_threads)
{
	int thread_check;
	
	pthread_t threads[num_threads];
	BlockThreadStruct threads_traits[num_threads];
	
    /* Updates the operation index for the threads so that if a thread has completed it's
       assigned operation, it will go and execute the next available operation */ 
	next_block_diagonal = num_threads;
	
    // Initializing the information in the struct for each thread and each thread is assigned an operation
	for(int i=0; i < num_threads; ++i)
	{
		threads_traits[i].thread_ID = i;
		threads_traits[i].diagonal_index = i;
		threads_traits[i].matrix = matrix;
		threads_traits[i].N = N;
		threads_traits[i].block_ID = i;
		
		thread_check = pthread_create(&threads[i], NULL, BlockThreadTransposition, &threads_traits[i]);
		
        //Error condition: If the pthreads are created successfully, 0 is returned
        //If threads are not created successfully, an error message will be output and the program will terminate 
		if(thread_check)
		{
			cout << "Threads were not created successfully";
			EXIT_FAILURE;
		}
	}
	//scheduling the joining of the threads
	for(int j = 0; j < num_threads; ++j) pthread_join(threads[j], NULL);
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

    BlockThreadManager(matrix, N, num_threads);

    //Pause the steady clock
    endClock = std::chrono::steady_clock::now();
    std::chrono::duration<double>elapsedTime = duration_cast<duration<double>>(endClock - startClock);

    //Uncomment the following two lines to view the NxN matrix transposition output
    /*cout << "\nTransposed Matrix";
    DisplayMatrix(matrix, N);*/

    cout << "Number of Threads: " << num_threads << endl;
    cout << "Matrix Size: " << N << " by " << N << " matrix" << endl;
    cout << "Block Transposition Elapsed Time in Seconds: " << elapsedTime.count() << endl;

    return 0;
}