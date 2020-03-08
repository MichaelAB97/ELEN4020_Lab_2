// Block-Orientated Transposition Algorithm using OpenMp
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

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


int getElementPosition(int coords[2], int N)
{
	int row = coords[0];
	int col = coords[1];
    int position = ( row * N ) + col;

    return position;
}


void transposeElement(int* row_element, int* col_element)
{
	int temp = *row_element;
    *row_element = *col_element;
    *col_element = temp;
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


void BlockTransposition(int* matrix, int N, int num_threads)
{
    int block_number = N/2;
    omp_set_num_threads(num_threads); //set number of threads

    #pragma omp parallel private(i,j)
    {
        #pragma omp for schedule(dynamic) nowait
        for (int i = 0; i < block_number; ++i)
        {
            for (int j = i; j < block_number; ++j)
            {
                int element_coords[2] = {i,j};
		        int block_1 = getElementPosition(element_coords, block_number);
		        int *blockElements_1 = blockElement( block_1, N);
		        blockTranspose(matrix, blockElements_1);
			
		        if(j != i)
		        {
			        element_coords[0] = j;
			        element_coords[1] = i;		

    			    int block_2 = getElementPosition(element_coords, block_number);
	    		    int *blockElements_2 = blockElement(block_2, N);
		    	    blockTranspose(matrix, blockElements_2);
			        blockElementsTranspose(matrix, blockElements_1, blockElements_2);
		        }
            }
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
    
    BlockTransposition(matrix, N, num_threads);
    
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