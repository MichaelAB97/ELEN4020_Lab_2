# ELEN4020 - Lab 2
The following Matrix Transposition Algorithms were implemented using pThreads and OpenMP:
- Naive-Threading Algorithm (only OpenMP)
- Diagonal Threading Algorithm
- Block-Oriented-Threading Algorithm

## Build Instructions
This lab utilizes [GNU make](https://www.gnu.org/software/make/manual/make.html#Makefile-Contents) to create a UNIX makefile for the build automation of this repo.

1. Clone this repo in your directory of choice.
2. Run `make` in your command window to build the files in this repo.
3. To run the files in this repo use the following terminal commands:

Commands to run the following programs:
- DThreadAlgorithm.cpp: `./DThreadAlg`
- BlockThreadAlgorithm.cpp: `./BlockThreadAlg`
- NaiveOpenMP.cpp: `./NaiveOpenMP`
- DiagonalOpenMP.cpp: `./DiagOpenMP`

4. To delete any binary files that were created during the building process run:
   `make clean`