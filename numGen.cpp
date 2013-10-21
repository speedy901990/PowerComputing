#include "MatrixGenerator.cpp"
#include <sstream>
using namespace std;

int main(int argc, char *argv[]){
  if (argc != 8) {
    cout << "USAGE: ./program.o [a|b|c|d] [m] [n] [k] [w] [crs|ccs] [filename]" << endl;
    return 0;
  }

  ostringstream oss;
  oss << argv[1];
  string matrixType = oss.str();
  oss.str("");
  oss << argv[6];
  string compression = oss.str();
  oss.str("");
  oss << argv[7];
  string filename = oss.str();
  int m = atoi(argv[2]);
  int n = atoi(argv[3]);
  int k = atoi(argv[4]);
  int w = atoi(argv[5]);

  MatrixGenerator *gen = new MatrixGenerator(matrixType, m, n, k, w, compression, filename);

  cout << endl;
  return 0;
}

// ./program.o [a|b|c|d] [m] [n] [k] [w] [crs|ccs] [filename]