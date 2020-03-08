all: DThreadAlg BlockThreadAlg NaiveAlg

DThreadAlg: DThreadAlgorithm.o
	g++ DThreadAlgorithm.o -o DThreadAlg -lpthread

DThreadAlgorithm.o: DThreadAlgorithm.cpp
	g++ -c DThreadAlgorithm.cpp

BlockThreadAlg: BlockThreadAlgorithm.o
	g++ BlockThreadAlgorithm.o -o BlockThreadAlg -lpthread

BlockThreadAlgorithm.o: BlockThreadAlgorithm.cpp
	g++ -c BlockThreadAlgorithm.cpp

NaiveAlg: NaiveOpenMP.o
	g++ -fopenmp -lgomp NaiveOpenMP.o -o NaiveAlg

NaiveOpenMP.o: NaiveOpenMP.cpp
	g++ -c NaiveOpenMP.cpp
clean:
	rm *.o DThreadAlg BlockThreadAlg NaiveAlg