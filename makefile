DThreadAlg: DThreadAlgorithm.o
	g++ DThreadAlgorithm.o -o DThreadAlg -lpthread

DThreadAlgorithm.o: DThreadAlgorithm.cpp
	g++ -c DThreadAlgorithm.cpp 

clean:
	rm *.o DThreadAlg