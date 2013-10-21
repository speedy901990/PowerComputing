#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "CRS.cpp"
#include "CCS.cpp"
using namespace std;

class MatrixGenerator {
//private:
public:
  int m, n; // matrix dim
  int k;  // not null %
  int w;  // strap width
  string filename;
  double ** matrix;
  double * multiVector;
  double * resultVector;
  int minIndex, maxIndex; //index random
  double minDiagValue, maxDiagValue, minValue, maxValue; //value random bounds

  int randomIndex(int, int);
  double randomValue(double, double);
  bool testMatrixA(int);
  bool testMatrixC(int);
  void printMatrix();

//public:
  MatrixGenerator(string, int, int, int, int, string, string);
  void initializeMatrixAndVector();
  void generateMatrixA(bool);
  void generateMatrixB(bool);
  void generateMatrixC(bool);
  void generateMatrixD(bool);
  void multiplyMatrixVectorCRS(string);
  void multiplyMatrixVectorCCS(string);
  void saveAsCRS();
  void saveAsCCS();
  void saveCRSToFile(CRS);
  void saveCCSToFile(CCS);
  CRS loadCRS(string);
  CCS loadCCS(string);
};