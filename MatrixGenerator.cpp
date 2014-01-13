#include "MatrixGenerator.h"
#include "timeSurvey.h"
//#include <sys/time.h>

int MatrixGenerator::randomIndex(int mminIndex, int maxIndex){
   return minIndex + (rand() % (int)(maxIndex - minIndex + 1));
}

double MatrixGenerator::randomValue(double minValue, double maxValue){
  double factor = (double)rand() / RAND_MAX;
  return minValue + factor * (maxValue - minValue);
}

MatrixGenerator::MatrixGenerator(int n, int k) {
  this->m = n;
  this->n = n;
  this->k = k;
  this->w = 0.05*n;
  minIndex = 0;
  maxIndex = n -1;
  minDiagValue = 1;
  maxDiagValue = 2;
  minValue = 0;
  maxValue = 1; 
  notNullElementsCount = 0;
  srand(time(NULL));
  numThreads = 4;

  cout << "NotNullElements: " << (int)((double)n*(double)n*(double)k/100.0) << endl;
  
  initializeMatrixAndVector();
  cout << endl;
  // cout << "MultiVector: " << endl;
  // for (int i=0 ; i<n ; i++)
  //   cout << multiVector[i] << " ";
  // cout << endl;

  executeComputing();
}

void MatrixGenerator::executeComputing() {
  maxIndex = n -1;
  generateMatrixA(true);
  //printMatrix();
  saveAsCRS("CRS_A.txt");
  //saveAsCCS("CCS_A.txt");
  multiplyMatrixVectorCRS("CRS_A.txt");
  clearResultVector();
  //multiplyMatrixVectorCCS("CCS_A.txt");
  //clearMatrixAndResultVector();
  cout << endl;

  //maxIndex = n -1;
  //generateMatrixB(true);
  //printMatrix();
  //saveAsCRS("CRS_B.txt");
  //saveAsCCS("CCS_B.txt");
  //multiplyMatrixVectorCRS("CRS_B.txt");
  //clearResultVector();
  //multiplyMatrixVectorCCS("CCS_B.txt");
  //clearMatrixAndResultVector();
  //cout << endl;

  //maxIndex = m -1;
  //generateMatrixC(true);
  //printMatrix();
  //saveAsCRS("CRS_C.txt");
  //saveAsCCS("CCS_C.txt");
  //multiplyMatrixVectorCRS("CRS_C.txt");
  //clearResultVector();
  //multiplyMatrixVectorCCS("CCS_C.txt");
  //clearMatrixAndResultVector();
  //cout << endl;

  //maxIndex = m -1;
  //generateMatrixD(true);
  //printMatrix();
  //saveAsCRS("CRS_D.txt");
  //saveAsCCS("CCS_D.txt");
  //multiplyMatrixVectorCRS("CRS_D.txt");
  //clearResultVector();
  //multiplyMatrixVectorCCS("CCS_D.txt");
  //clearMatrixAndResultVector();  
}

void MatrixGenerator::initializeMatrixAndVector() {
  cout << "Initializing matrix and vector... " << flush;
  matrix = new double*[m];
  for (int i=0 ; i<m ; i++)
    matrix[i] = new double[n];
  clearMatrix();

  multiVector = new double[n];
  for (int i=0 ; i<n ; i++)
    multiVector[i] = randomValue(1, 2);

  resultVector = new double[n];
  clearResultVector();
  cout << "done" << endl;
}

void MatrixGenerator::clearMatrixAndResultVector() {
  clearMatrix();
  clearResultVector();
  notNullElementsCount = 0;

  // for (int i=0 ; i<m ; i++)
  //   delete [] matrix[i];
  // delete matrix;
  // delete resultVector;

  //initializeMatrixAndVector();
}

void MatrixGenerator::clearResultVector() {
  for (int i=0 ; i<n ; i++)
    resultVector[i] = 0;
}

void MatrixGenerator::clearMatrix() {
  for (int i=0 ; i<m ; i++){
    for (int j=0 ; j<n ; j++){
      matrix[i][j] = 0;
    }
  }
}

void MatrixGenerator::generateMatrixA(bool isPercent) {
  cout << "Generating matrixA... " << flush;
  int numbersNotNull = k;
  if (isPercent)
    numbersNotNull = (int)((double)m*((double)numbersNotNull/100.0));

  for (int i=0 ; i<m ; i++) {
    matrix[i][i] = randomValue(minDiagValue, maxDiagValue);
    notNullElementsCount++;
    int numbersLeft = numbersNotNull - 1;
    while (numbersLeft) {
      int index = randomIndex(maxIndex, maxIndex);
      if (matrix[i][index] == 0) {
        matrix[i][index] = randomValue(minValue, maxValue);
        notNullElementsCount++;
        numbersLeft--;
      }
    }
  }
  cout << "done" << endl;
  cout << "MatrixA test: " << ((testMatrixA(numbersNotNull) == true)?"passed":"failed") << endl;
}

void MatrixGenerator::generateMatrixB(bool isPercent) {
  cout << "Generating matrixB... " << flush;
  int UpDown = 0;
  for (int i=0 ; i<m ; i++) {
    for (int j=i-w+UpDown ; j<=i+w+UpDown ; j++) {
      if (j < 0 || j >= n)
        continue;
      if (i == j) {
        matrix[i][j] = randomValue(minDiagValue, maxDiagValue);
    }
      else {
        matrix[i][j] = randomValue(minValue, maxValue);
    }
    notNullElementsCount++;
    }
  }
  cout << "done" << endl;
  cout << "MatrixB test: passed" << endl;
}

void MatrixGenerator::generateMatrixC(bool isPercent) {
  cout << "Generating matrixC... " << flush;
  int numbersNotNull = k;
  if (isPercent)
    numbersNotNull = (int)((double)n*((double)numbersNotNull/100.0));

  for (int j=0 ; j<n ; j++) {
    matrix[j][j] = randomValue(minDiagValue, maxDiagValue); 
    notNullElementsCount++;
    int numbersLeft = numbersNotNull - 1;
    while (numbersLeft) {
      int index = randomIndex(minIndex, maxIndex);
      if (matrix[index][j] == 0) {
        matrix[index][j] = randomValue(minValue, maxValue);
        notNullElementsCount++;
        numbersLeft--;
      }
    }
  }
  cout << "done" << endl;
  cout << "MatrixC test: " << ((testMatrixC(numbersNotNull) == true)?"passed":"failed") << endl;
}

void MatrixGenerator::generateMatrixD(bool isPercent) {
  cout << "Generating matrixD... " << flush;
  int UpDown = 0;
  for (int i=0 ; i<m ; i++) {
    for (int j=i-w+UpDown ; j<=i+w+UpDown ; j++) {
      if (j < 0 || j >= n)
        continue;
      if (i == j) {
        matrix[i][j] = randomValue(minDiagValue, maxDiagValue);
      }
      else {
        matrix[i][j] = randomValue(minValue, maxValue);
    }
    notNullElementsCount++;
    }
  }
  cout << "done" << endl;
  cout << "MatrixD test: passed" << endl;
}

bool MatrixGenerator::testMatrixA(int numbersNotNull) {
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
  for (int j=0 ; j<n ; j++) {
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

void MatrixGenerator::saveAsCRS(string filename){
  cout << "Saving CRS... " << flush;
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
    // if (allNull)
    //   crs.rowPtr.push_back(-1);
  }
  crs.rowPtr.push_back(notNullElementsCount);

  saveCRSToFile(crs, filename); 
  cout << "done" << endl;
}

void MatrixGenerator::saveCRSToFile(CRS crs, string filename) {
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

CRS *MatrixGenerator::loadCRS(string filename) {
  cout << "Loading CRS... " << flush;
  int m, n;
  int valSize;
  int colIdSize;
  int rowPtrSize;
  CRS *crs = new CRS();
  ifstream file;
  file.open(filename.c_str());
  file >> m;
  file >> n;
  file >> valSize;
  for (int i=0 ; i<valSize ; i++) {
    double x;
    file >> x;
    crs->val.push_back(x);
  }
  file >> colIdSize;
  for (int i=0 ; i<colIdSize ; i++) {
    int x;
    file >> x;
    crs->colId.push_back(x);
  }
  file >> rowPtrSize;
  for (int i=0 ; i<rowPtrSize ; i++) {
    int x;
    file >> x;
    crs->rowPtr.push_back(x);
  }
  file.close();

  cout << "done" << endl;
  return crs;
}

void MatrixGenerator::decompressCRS(CRS crs) {
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
      step = crs.val.size() - crs.rowPtr[i];
    else if (crs.rowPtr[i] == -1)
      continue;
    else {
      if (crs.rowPtr[i+1] == -1){
        if (i+1 == m -1)
          step = crs.val.size() - crs.rowPtr[i];
        else{
          int tmp = i+1;
          while (crs.rowPtr[tmp] == -1) {tmp++;}
          if (tmp == crs.rowPtr.size())
            step = crs.val.size() - crs.rowPtr[i];
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
  }

  cout << "Matrix test decompression CRS result: " << endl;
  for (int i=0 ; i<m ; i++)
    for (int j=0 ; j<n ; j++){
      cout << matrix[i][j] << "\t";
      if (j == n -1)
        cout << endl;
    }
}

void MatrixGenerator::saveAsCCS(string filename){
 cout << "Saving CCS... " << flush; 
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
    // if (allNull)
    //   ccs.colPtr.push_back(-1);
  }
  ccs.colPtr.push_back(notNullElementsCount);

  saveCCSToFile(ccs, filename);
  cout << "done" << endl;
}

void MatrixGenerator::saveCCSToFile(CCS ccs, string filename) {
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

CCS *MatrixGenerator::loadCCS(string filename) {
  cout << "Loading CCS... " << flush;
  int m, n;
  int valSize;
  int rowIdSize;
  int colPtrSize;
  CCS *ccs = new CCS();
  ifstream file;
  file.open(filename.c_str());
  file >> m;
  file >> n;
  file >> valSize;
  for (int i=0 ; i<valSize ; i++) {
    double x;
    file >> x;
    ccs->val.push_back(x);
  }
  file >> rowIdSize;
  for (int i=0 ; i<rowIdSize ; i++) {
    int x;
    file >> x;
    ccs->rowId.push_back(x);
  }
  file >> colPtrSize;
  for (int i=0 ; i<colPtrSize ; i++) {
    int x;
    file >> x;
    ccs->colPtr.push_back(x);
  }
  file.close();

  cout << "done" << endl;
  return ccs;
}

void MatrixGenerator::decompressCCS(CCS ccs) {
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
      step = ccs.val.size() - ccs.colPtr[j];
    else if (ccs.colPtr[j] == -1)
      continue;
    else {
      if (ccs.colPtr[j+1] == -1){
        if (j+1 == n -1)
          step = ccs.val.size() - ccs.colPtr[j];
        else{
          int tmp = j+1;
          while (ccs.colPtr[tmp] == -1){tmp++;}
          if (tmp == ccs.colPtr.size())
            step = ccs.val.size() - ccs.colPtr[j];
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
}

//-----------------CRS Algorithms----------------------------------//

void MatrixGenerator::multiplyMatrixVectorCRS(string filename) {
  CRS *crs = loadCRS(filename);
  algorithmOneCRS(crs);
  clearResultVector();
  algorithmTwoCRS(crs);
  clearResultVector();
  parallelOpenMP_CRS(crs);
  clearResultVector();
  parallelPthreads_CRS(crs);
  clearResultVector();
  parallelMPI_CRS(crs);
}

void MatrixGenerator::algorithmOneCRS(CRS *crs) {
  cout << "Computing CRS... " << flush;
  // timeval start, stop;
  // gettimeofday(&start, 0);
  initializeTime();

  for (int i=0 ; i<m ; i++){
    for (int j=crs->rowPtr[i] ; j<crs->rowPtr[i+1] ; j++) {
      resultVector[i] += crs->val[j] * multiVector[crs->colId[j]];
    }
  }

  // gettimeofday(&stop, 0);
  cout << "done ver. 1" << endl;
  // long seconds = stop.tv_sec - start.tv_sec;
  // long useconds = stop.tv_usec - start.tv_usec;
  // double elapsedTime = (seconds * 1000 + useconds/1000.0) + 0.5;
  cout << "->Time elapsed [CRS]: " << endl;// setprecision(6) << elapsedTime << endl;
  printTime();
  //printResultVector("CRS");
}

void MatrixGenerator::algorithmTwoCRS(CRS *crs) {
  cout << "Computing CRS... " << flush;
  // timeval start, stop;
  // gettimeofday(&start, 0);
  initializeTime();

  for (int i=0 ; i<m ; i+=10){
    for (int j=crs->rowPtr[i] ; j<crs->rowPtr[i+1] ; j++) {
      resultVector[i] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+1] ; j<crs->rowPtr[i+2] ; j++) {
      resultVector[i+1] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+2] ; j<crs->rowPtr[i+3] ; j++) {
      resultVector[i+2] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+3] ; j<crs->rowPtr[i+4] ; j++) {
      resultVector[i+3] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+4] ; j<crs->rowPtr[i+5] ; j++) {
      resultVector[i+4] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+5] ; j<crs->rowPtr[i+6] ; j++) {
      resultVector[i+5] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+6] ; j<crs->rowPtr[i+7] ; j++) {
      resultVector[i+6] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+7] ; j<crs->rowPtr[i+8] ; j++) {
      resultVector[i+7] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+8] ; j<crs->rowPtr[i+9] ; j++) {
      resultVector[i+8] += crs->val[j] * multiVector[crs->colId[j]];
    }
	for (int j=crs->rowPtr[i+9] ; j<crs->rowPtr[i+10] ; j++) {
      resultVector[i+9] += crs->val[j] * multiVector[crs->colId[j]];
    }

  }

  // gettimeofday(&stop, 0);
  cout << "done ver. 2" << endl;
  // long seconds = stop.tv_sec - start.tv_sec;
  // long useconds = stop.tv_usec - start.tv_usec;
  // double elapsedTime = (seconds * 1000 + useconds/1000.0) + 0.5;
  cout << "->Time elapsed [CRS]: " << endl; //setprecision(6) << elapsedTime << endl;
  printTime();
  //printResultVector("CRS");
}

void MatrixGenerator::parallelOpenMP_CRS(CRS* crs) {
  cout << "Computing CRS... " << flush;
  initializeTime();

  int ckey = 0, 
	  j = 0,
	  chunk = 1000;

  #pragma omp for private(ckey,j) schedule(static,chunk)
  //#pragma omp parallel for
  for (int i=0 ; i<m ; i++){
    for (ckey=crs->rowPtr[i] ; ckey<crs->rowPtr[i+1] ; ckey++) {
		j = crs->colId[ckey];
      resultVector[i] += crs->val[ckey] * multiVector[j];
    }
  }

  cout << "done parallel OpenMP" << endl;
  cout << "->Time elapsed [CRS]: " << endl; //setprecision(6) << elapsedTime << endl;
  printTime();
}

class PthreadData {
public:
	CRS * crs;
	double * resultVector;
	double * multiVector;
	int m;
	int numThreads;
	int threadID;
};

void * algorithmForThread(void * x) {
  PthreadData * data = (PthreadData *) x;
  for (int i = (data->threadID) * data->m / data->numThreads ; i<((data->threadID) + 1) * data->m / data->numThreads ; i++) {
    for (int ckey=data->crs->rowPtr[i] ; ckey<data->crs->rowPtr[i+1] ; ckey++) {
      data->resultVector[i] += data->crs->val[ckey] * data->multiVector[data->crs->colId[ckey]];
    }
  }
}

void MatrixGenerator::parallelPthreads_CRS(CRS* crs) {
  cout << "Computing CRS... " << flush;

  pthread_t * threads = new pthread_t[numThreads];
  
  PthreadData * data = new PthreadData();
  data->crs = crs;
  data->resultVector = resultVector;
  data->multiVector = multiVector;
  data->m = m;

  initializeTime();
  for (int i=0 ; i<numThreads ; i++) {
	  data->threadID = i;
	  pthread_create( &threads[i], NULL, algorithmForThread, &data);
  }

  cout << "done parallel Pthreads" << endl;
  cout << "->Time elapsed [CRS]: " << endl; //setprecision(6) << elapsedTime << endl;
  printTime();
}

void MatrixGenerator::parallelMPI_CRS(CRS* crs) {
  cout << "Computing CRS... " << flush;
  initializeTime();

  // int  numTasks, rank, rc;
  // MPI_Status status;
  // MPI_Request req, req1;
  // rc = MPI_Init(&argc,&argv);
  // int tab[2010];
  // int recvbuf[1005];
  // int messageToSent = 10;
  // int messageToRespond = 20;
  
  // MPI_Comm_size(MPI_COMM_WORLD,&numTasks);
  // MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  // printf ("Process count = %d Process ID = %d\n", numTasks,rank);

  // int index = n/numTasks;
  
  // // odpowiednie rozesłanie danych
  // MPI_Bcast(crs,index,MPI_INT,recvbuf,index,MPI_INT,0,MPI_COMM_WORLD);
  
  for (int i=0 ; i<m ; i++){
    for (int j=crs->rowPtr[i] ; j<crs->rowPtr[i+1] ; j++) {
      resultVector[i] += crs->val[j] * multiVector[crs->colId[j]];
    }
  }
  
  // MPI_Gather(&recvbuf[0],1,MPI_INT,tab,1,MPI_INT,0,MPI_COMM_WORLD);
  
  
  // MPI_Barrier(MPI_COMM_WORLD);
  // MPI_Finalize ( );

  cout << "done parallel MPI" << endl;
  cout << "->Time elapsed [CRS]: " << endl; //setprecision(6) << elapsedTime << endl;
  printTime();
}


//-----------------CCS Algorithms----------------------------------//

void MatrixGenerator::multiplyMatrixVectorCCS(string filename) {
  CCS *ccs = loadCCS(filename);
  algorithmOneCCS(ccs);
  clearResultVector();
  algorithmTwoCCS(ccs);
}

void MatrixGenerator::algorithmOneCCS(CCS *ccs) {
  cout << "Computing CCS... " << flush;
  // timeval start, stop;
  // gettimeofday(&start, 0);
  initializeTime();

  for (int i=0 ; i<m ; i++){
    for (int j=ccs->colPtr[i] ; j<ccs->colPtr[i+1] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i];
    }
  }
  
  // gettimeofday(&stop, 0);
  cout << "done ver. 1" << endl;
  // long seconds = stop.tv_sec - start.tv_sec;
  // long useconds = stop.tv_usec - start.tv_usec;
  // double elapsedTime = (seconds * 1000 + useconds/1000.0) + 0.5;
  cout << "->Time elapsed [CCS]: " << endl;// setprecision(6) << elapsedTime << endl;
  printTime();
  //printResultVector("CCS");
}

void MatrixGenerator::algorithmTwoCCS(CCS *ccs) {
  cout << "Computing CCS... " << flush;
  // timeval start, stop;
  // gettimeofday(&start, 0);
  initializeTime();

  for (int i=0 ; i<m ; i+=10){
    for (int j=ccs->colPtr[i] ; j<ccs->colPtr[i+1] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i];
    }
    for (int j=ccs->colPtr[i+1] ; j<ccs->colPtr[i+2] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+1];
    }
    for (int j=ccs->colPtr[i+2] ; j<ccs->colPtr[i+3] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+2];
    }
    for (int j=ccs->colPtr[i+3] ; j<ccs->colPtr[i+4] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+3];
    }
    for (int j=ccs->colPtr[i+4] ; j<ccs->colPtr[i+5] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+4];
    }
    for (int j=ccs->colPtr[i+5] ; j<ccs->colPtr[i+6] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+5];
    }
    for (int j=ccs->colPtr[i+6] ; j<ccs->colPtr[i+7] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+6];
    }
    for (int j=ccs->colPtr[i+7] ; j<ccs->colPtr[i+8] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+7];
    }
    for (int j=ccs->colPtr[i+8] ; j<ccs->colPtr[i+9] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+8];
    }
    for (int j=ccs->colPtr[i+9] ; j<ccs->colPtr[i+10] ; j++) {
      resultVector[ccs->rowId[j]] += ccs->val[j] * multiVector[i+9];
    }
  }
  
  // gettimeofday(&stop, 0);
  cout << "done ver. 2" << endl;
  // long seconds = stop.tv_sec - start.tv_sec;
  // long useconds = stop.tv_usec - start.tv_usec;
  // double elapsedTime = (seconds * 1000 + useconds/1000.0) + 0.5;
  cout << "->Time elapsed [CCS]: " << endl;// setprecision(6) << elapsedTime << endl;
  printTime();
  //printResultVector("CCS");
}

void MatrixGenerator::printResultVector(string compression) {
  cout << "Result vector " << compression.c_str() << ": ";
  for (int i=0 ; i<m ; i++)
    cout << resultVector[i] << " ";
  cout << endl;
}
