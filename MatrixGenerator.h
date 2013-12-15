#ifndef MATRIXGENERATOR_H
#define MATRIXGENERATOR_H
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "CRS.h"
#include "CCS.h"
using namespace std;

class MatrixGenerator {
//private:
public:
  int m, n; // matrix dim
  int k;  // not null %
  int w;  // strap width
  int notNullElementsCount;
  double ** matrix;
  double * multiVector;
  double * resultVector;
  int minIndex, maxIndex; //index random
  double minDiagValue, maxDiagValue, minValue, maxValue; //value random bounds
  int numThreads;

  int randomIndex(int, int);
  double randomValue(double, double);
  bool testMatrixA(int);
  bool testMatrixC(int);
  void printMatrix();

//public:
  MatrixGenerator(int, int);
  void executeComputing();
  void initializeMatrixAndVector();
  void clearMatrixAndResultVector();
  void clearResultVector();
  void clearMatrix();
  void generateMatrixA(bool);
  void generateMatrixB(bool);
  void generateMatrixC(bool);
  void generateMatrixD(bool);
  void multiplyMatrixVectorCRS(string);
  void multiplyMatrixVectorCCS(string);
  void saveAsCRS(string);
  void saveAsCCS(string);
  void saveCRSToFile(CRS, string);
  void saveCCSToFile(CCS, string);
  CRS *loadCRS(string);
  CCS *loadCCS(string);
  void decompressCRS(CRS);
  void decompressCCS(CCS);
  void printResultVector(string);
  void algorithmOneCRS(CRS*);
  void algorithmTwoCRS(CRS*);
  void algorithmOneCCS(CCS*);
  void algorithmTwoCCS(CCS*);
  void parallelOpenMP_CRS(CRS*);
  void parallelPthreads_CRS(CRS*);
  void parallelMPI_CRS(CRS*);
  //void *algorithmForThread(void*);
};
#endif
