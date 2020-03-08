all: DThreadAlg BlockThreadAlg NaiveOpenMP DiagOpenMP

DThreadAlg: DThreadAlgorithm.o
	g++ DThreadAlgorithm.o -o DThreadAlg -lpthread

DThreadAlgorithm.o: DThreadAlgorithm.cpp
	g++ -c DThreadAlgorithm.cpp

BlockThreadAlg: BlockThreadAlgorithm.o
	g++ BlockThreadAlgorithm.o -o BlockThreadAlg -lpthread

BlockThreadAlgorithm.o: BlockThreadAlgorithm.cpp
	g++ -c BlockThreadAlgorithm.cpp

NaiveOpenMP: NaiveOpenMP.o
	g++ -fopenmp -lgomp NaiveOpenMP.o -o NaiveOpenMP

NaiveOpenMP.o: NaiveOpenMP.cpp
	g++ -c NaiveOpenMP.cpp

DiagOpenMP: DiagonalOpenMP.o
	g++ -fopenmp -lgomp DiagonalOpenMP.o -o DiagOpenMP

DiagonalOpenMP.o: DiagonalOpenMP.cpp
	g++ -c DiagonalOpenMP.cpp

clean:
	rm *.o DThreadAlg BlockThreadAlg NaiveOpenMP DiagOpenMP