#include "MatrixGenerator.h"

int MatrixGenerator::randomIndex(int mminIndex, int maxIndex){
   return minIndex + (rand() % (int)(maxIndex - minIndex + 1));
}

double MatrixGenerator::randomValue(double minValue, double maxValue){
  double factor = (double)rand() / RAND_MAX;
  return minValue + factor * (maxValue - minValue);
}

MatrixGenerator::MatrixGenerator(string matrixType, int m, int n, int k, int w, string compression, string filename) {
  this->m = m;
  this->n = n;
  this->k = k;
  this->w = w;
  this->filename = filename;
  minIndex = 0;
  maxIndex = m -1;
  minDiagValue = 1;
  maxDiagValue = 2;
  minValue = 0;
  maxValue = 1; 
  srand(time(NULL));

  initializeMatrixAndVector();

  if (matrixType == "a" || matrixType == "A"){
    maxIndex = n -1;
    generateMatrixA(true);
  }
  else if (matrixType == "b" || matrixType == "B") {
    maxIndex = n -1;
    generateMatrixB(true);
  }
  else if (matrixType == "c" || matrixType == "C") {
    maxIndex = m -1;
    generateMatrixC(true);
  }
  else if (matrixType == "d" || matrixType == "D") {
    maxIndex = m -1;
    generateMatrixD(true);
  }

  if (compression == "crs" || compression == "CRS") {
    saveAsCRS();
    //loadCRS(filename);    
  }
  else if (compression == "ccs" || compression == "CCS") {
    saveAsCCS();
    //loadCCS(filename);
  }

  multiplyMatrixVectorCRS(filename);
}

void MatrixGenerator::initializeMatrixAndVector() {
  matrix = new double*[m];
  for (int i=0 ; i<m ; i++)
    matrix[i] = new double[n];

  for (int i=0 ; i<m ; i++){
    for (int j=0 ; j<n ; j++){
      matrix[i][j] = 0;
    }
  }

  multiVector = new double[n];
  for (int i=0 ; i<n ; i++)
    multiVector[i] = randomValue(1, 2);

  resultVector = new double[n];
  for (int i=0 ; i<n ; i++)
    resultVector[i] = 0;
}

void MatrixGenerator::generateMatrixA(bool isPercent) {
  int numbersNotNull = k;
  if (isPercent)
    numbersNotNull = (int)(m*((double)numbersNotNull/100.0));

  for (int i=0 ; i<m ; i++) {
    int numbersLeft = numbersNotNull;

    while (numbersLeft) {
      int index = randomIndex(maxIndex, maxIndex);
      if (matrix[i][index] == 0) {
        if (i == index)
          matrix[i][index] = randomValue(minDiagValue, maxDiagValue);
        else
          matrix[i][index] = randomValue(minValue, maxValue);
        numbersLeft--;
      }
    }
  }

  printMatrix();
  cout << "Matrix test: " << ((testMatrixA(numbersNotNull) == true)?"passed":"failed");
}

void MatrixGenerator::generateMatrixB(bool isPercent) {
  int UpDown = 0;
  for (int i=0 ; i<m ; i++) {
    for (int j=i-w+UpDown ; j<=i+w+UpDown ; j++) {
      if (j < 0 || j > n)
        continue;
      if (i == j)
        matrix[i][j] = randomValue(minDiagValue, maxDiagValue);
      else
        matrix[i][j] = randomValue(minValue, maxValue);
    }
  }
  printMatrix();
  cout << "Matrix test: passed";
}

void MatrixGenerator::generateMatrixC(bool isPercent) {
  int numbersNotNull = k;
  if (isPercent)
    numbersNotNull = (int)(n*((double)numbersNotNull/100.0));

  for (int j=0 ; j<n ; j++) {
    int numbersLeft = numbersNotNull;

    while (numbersLeft) {
      int index = randomIndex(minIndex, maxIndex);
      if (matrix[index][j] == 0) {
        if (j == index)
          matrix[index][j] = randomValue(minDiagValue, maxDiagValue);
        else
          matrix[index][j] = randomValue(minValue, maxValue);
        numbersLeft--;
      }
    }
  }

  printMatrix();
  cout << "Matrix test: " << ((testMatrixC(numbersNotNull) == true)?"passed":"failed");
}

void MatrixGenerator::generateMatrixD(bool isPercent) {
  int UpDown = 0;
  for (int i=0 ; i<m ; i++) {
    for (int j=i-w+UpDown ; j<=i+w+UpDown ; j++) {
      if (j < 0 || j > n)
        continue;
      if (i == j)
        matrix[i][j] = randomValue(minDiagValue, maxDiagValue);
      else
        matrix[i][j] = randomValue(minValue, maxValue);
    }
  }
  printMatrix();
  cout << "Matrix test: passed";
}

bool MatrixGenerator::testMatrixA(int numbersNotNull) {
  cout << endl;
  for (int i=0 ; i<m ; i++){
    int notNull = 0;
    for (int j=0 ; j<n ; j++){
      if (matrix[i][j] != 0)
        notNull++;
    }
    if (notNull != numbersNotNull)
      return false;
  }
  return true;
}

bool MatrixGenerator::testMatrixC(int numbersNotNull) {
  cout << endl;
  for (int j=0 ; j<n ; j++){
    int notNull = 0;
    for (int i=0 ; i<m ; i++){
      if (matrix[i][j] != 0)
        notNull++;
    }
    if (notNull != numbersNotNull)
      return false;
  }
  return true;
}

void MatrixGenerator::printMatrix() {
  for (int i=0 ; i<m ; i++)
    for (int j=0 ; j<n ; j++){
      cout << setprecision(2) << matrix[i][j] << "\t";
      if (j == n - 1)
        cout << endl;
    }
}

void MatrixGenerator::saveAsCRS(){
  CRS crs;
  int lastRow = 0;
  bool allNull = true;
  for (int i=0 ; i<m ; i++){
    allNull = true;
    for (int j=0 ; j<n ; j++){
      if (matrix[i][j] != 0){        
        crs.val.push_back(matrix[i][j]);
        crs.colId.push_back(j);
        if (lastRow < i || crs.val.size() == 1)
          crs.rowPtr.push_back(crs.val.size()-1);
        lastRow = i;
        allNull = false;
      }
    }
    if (allNull)
      crs.rowPtr.push_back(-1);
  }

  saveCRSToFile(crs); 
}

void MatrixGenerator::saveCRSToFile(CRS crs) {
  ofstream file;
  file.open(filename.c_str());
  file << m << " " << n << "\n";
  file << crs.val.size()<< "\n";
  for (int i=0 ; i<crs.val.size() ; i++)
    file << crs.val[i] << " ";
  file << "\n";
  file << crs.colId.size()<< "\n";
  for (int i=0 ; i<crs.colId.size() ; i++)
    file << crs.colId[i] << " ";
  file << "\n";
  file << crs.rowPtr.size()<< "\n";
  for (int i=0 ; i<crs.rowPtr.size() ; i++)
    file << crs.rowPtr[i] << " ";
  file << "\n";
  file.close();
}

CRS MatrixGenerator::loadCRS(string filename) {
  int m, n;
  int valSize;
  int colIdSize;
  int rowPtrSize;
  CRS crs;
  ifstream file;
  file.open(filename.c_str());
  file >> m;
  file >> n;
  file >> valSize;
  for (int i=0 ; i<valSize ; i++) {
    double x;
    file >> x;
    crs.val.push_back(x);
  }
  cout << endl;
  file >> colIdSize;
  for (int i=0 ; i<colIdSize ; i++) {
    int x;
    file >> x;
    crs.colId.push_back(x);
  }
  cout << endl;
  file >> rowPtrSize;
  for (int i=0 ; i<rowPtrSize ; i++) {
    int x;
    file >> x;
    crs.rowPtr.push_back(x);
  }
  cout << endl;
  file.close();

  double **matrixTest;
  matrixTest = new double*[m];
  for (int i=0 ; i<m ; i++)
    matrixTest[i] = new double[n];

  for (int i=0 ; i<m ; i++)
    for (int j=0 ; j<n ; j++)
      matrixTest[i][j] = 0;

  int j=0;
  for (int i=0 ; i<m-1; i++) {
    int step = 0;
    if (i == m - 1)
      step = valSize - crs.rowPtr[i];
    else if (crs.rowPtr[i] == -1)
      continue;
    else {
      if (crs.rowPtr[i+1] == -1){
        if (i+1 == m -1)
          step = valSize - crs.rowPtr[i];
        else{
          int tmp = i+1;
          while (crs.rowPtr[tmp] == -1) {tmp++;}
          if (tmp == crs.rowPtr.size())
            step = valSize - crs.rowPtr[i];
          else
            step = crs.rowPtr[tmp] - crs.rowPtr[i];
        }
      }
      else
        step = crs.rowPtr[i+1] - crs.rowPtr[i];
    }
    while (step) { 
      matrixTest[i][crs.colId[j]] = crs.val[j];
      j++;
      step--;
    }

    return crs;
  }

  cout << "Matrix test decompression CRS result: " << endl;
  for (int i=0 ; i<m ; i++)
    for (int j=0 ; j<n ; j++){
      cout << matrix[i][j] << "\t";
      if (j == n -1)
        cout << endl;
    }
}

void MatrixGenerator::saveAsCCS(){
  CCS ccs;
  int lastColumn = 0;
  bool allNull = true;
  for (int j=0 ; j<n ; j++) {
    allNull = true;
    for (int i=0 ; i<m ; i++) {
      if (matrix[i][j] != 0) {        
        ccs.val.push_back(matrix[i][j]);
        ccs.rowId.push_back(i);
        if (lastColumn < j || ccs.val.size() == 1)
          ccs.colPtr.push_back(ccs.val.size()-1);
        lastColumn = j;
        allNull = false;
      }
    }
    if (allNull)
      ccs.colPtr.push_back(-1);
  }
  saveCCSToFile(ccs);
}

void MatrixGenerator::saveCCSToFile(CCS ccs) {
  ofstream file;
  file.open(filename.c_str());
  file << m << " " << n << "\n";
  file << ccs.val.size()<< "\n";
  for (int i=0 ; i<ccs.val.size() ; i++)
    file << ccs.val[i] << " ";
  file << "\n";
  file << ccs.rowId.size() << "\n";
  for (int i=0 ; i<ccs.rowId.size() ; i++)
    file << ccs.rowId[i] << " ";
  file << "\n";
  file << ccs.colPtr.size() << "\n";
  for (int i=0 ; i<ccs.colPtr.size() ; i++)
    file << ccs.colPtr[i] << " ";
  file << "\n";
  file.close();
}

CCS MatrixGenerator::loadCCS(string filename) {
  int m, n;
  int valSize;
  int rowIdSize;
  int colPtrSize;
  CCS ccs;
  ifstream file;
  file.open(filename.c_str());
  file >> m;
  file >> n;
  file >> valSize;
  for (int i=0 ; i<valSize ; i++) {
    double x;
    file >> x;
    ccs.val.push_back(x);
  }
  cout << endl;
  file >> rowIdSize;
  for (int i=0 ; i<rowIdSize ; i++) {
    int x;
    file >> x;
    ccs.rowId.push_back(x);
  }
  cout << endl;
  file >> colPtrSize;
  for (int i=0 ; i<colPtrSize ; i++) {
    int x;
    file >> x;
    ccs.colPtr.push_back(x);
  }
  cout << endl;
  file.close();

  double **matrixTest;
  matrixTest = new double*[m];
  for (int i=0 ; i<m ; i++)
    matrixTest[i] = new double[n];

  for (int i=0 ; i<m ; i++)
    for (int j=0 ; j<n ; j++)
      matrixTest[i][j] = 0;

  int i = 0;
  for (int j=0 ; j<n-1; j++) {
    int step = 0;
    if (j == n - 1)
      step = valSize - ccs.colPtr[j];
    else if (ccs.colPtr[j] == -1)
      continue;
    else {
      if (ccs.colPtr[j+1] == -1){
        if (j+1 == n -1)
          step = valSize - ccs.colPtr[j];
        else{
          int tmp = j+1;
          while (ccs.colPtr[tmp] == -1){tmp++;}
          if (tmp == ccs.colPtr.size())
            step = valSize - ccs.colPtr[j];
          else
            step =  ccs.colPtr[tmp] - ccs.colPtr[j];
        }
      }
      else
        step = ccs.colPtr[j+1] - ccs.colPtr[j];
    }
    while (step) { 
      matrixTest[ccs.rowId[i]][j] = ccs.val[i];
      i++;
      step--;
    }
  }

  cout << "Matrix test decompression CSS result: " << endl;
  for (int i=0 ; i<m ; i++)
    for (int j=0 ; j<n ; j++){
      cout << matrixTest[i][j] << "\t";
      if (j == n -1)
        cout << endl;
    }

  return ccs;
}

void MatrixGenerator::multiplyMatrixVectorCRS(string filename) {
  CRS crs = loadCRS(filename);

  for (int i=0 ; i<n ; i++){
    for (int j=crs.rowPtr[i] ; j<crs.rowPtr[i+1] -1 ; j++) {
      resultVector[i] += crs.val[j] * multiVector[crs.colId[j]];
    }
  }

  for (int i=0 ; i<n ; i++)
    cout << resultVector[i] << " ";
  cout << endl;
}

void MatrixGenerator::multiplyMatrixVectorCCS(string filename) {
  CCS ccs = loadCCS(filename);

}
