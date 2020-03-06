all: DThreadAlg BlockThreadAlg

DThreadAlg: DThreadAlgorithm.o
	g++ DThreadAlgorithm.o -o DThreadAlg -lpthread

DThreadAlgorithm.o: DThreadAlgorithm.cpp
	g++ -c DThreadAlgorithm.cpp

BlockThreadAlg: BlockThreadAlgorithm.o
	g++ BlockThreadAlgorithm.o -o BlockThreadAlg -lpthread

BlockThreadAlgorithm.o: BlockThreadAlgorithm.cpp
	g++ -c BlockThreadAlgorithm.cpp

clean:
	rm *.o DThreadAlg BlockThreadAlg