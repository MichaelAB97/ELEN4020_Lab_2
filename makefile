all: DThreadAlg BlockThreadAlg NaiveOpenMP DiagOpenMP BlockOpenMP

# Diagonal Matrix Transposition using PThreads
DThreadAlg: DThreadAlgorithm.o
	g++ DThreadAlgorithm.o -o DThreadAlg -lpthread

DThreadAlgorithm.o: DThreadAlgorithm.cpp
	g++ -c DThreadAlgorithm.cpp


# Block-Orientated Matrix Transposition using PThreads
BlockThreadAlg: BlockThreadAlgorithm.o
	g++ BlockThreadAlgorithm.o -o BlockThreadAlg -lpthread

BlockThreadAlgorithm.o: BlockThreadAlgorithm.cpp
	g++ -c BlockThreadAlgorithm.cpp


# Naive Matrix Transposition using OpenMP
NaiveOpenMP: NaiveOpenMP.o
	g++ -fopenmp -lgomp NaiveOpenMP.o -o NaiveOpenMP

NaiveOpenMP.o: NaiveOpenMP.cpp
	g++ -c NaiveOpenMP.cpp


# Diagonal Matrix Transposition using OpenMP
DiagOpenMP: DiagonalOpenMP.o
	g++ -fopenmp -lgomp DiagonalOpenMP.o -o DiagOpenMP

DiagonalOpenMP.o: DiagonalOpenMP.cpp
	g++ -c DiagonalOpenMP.cpp


# Block-Orientated Matrix Transposition using PThreads
BlockOpenMP: BlockOpenMP.o
	g++ -fopenmp -lgomp BlockOpenMP.o -o BlockOpenMP

BlockOpenMP.o: BlockOpenMP.cpp
	g++ -c BlockOpenMP.cpp


# Removing All Binary Files
clean:
	rm *.o DThreadAlg BlockThreadAlg NaiveOpenMP DiagOpenMP BlockOpenMP