#include "MatrixGenerator.cpp"
#include <sstream>
using namespace std;

int main(int argc, char *argv[]){
  if (argc != 3) {
    cout << "USAGE: ./program.o [n] [k] (k: 0\%-100\%)" << endl;
    return 0;
  }

  int n = atoi(argv[1]);
  int k = atoi(argv[2]);
  if ((int)(n*n*((double)k/100.0) < n)) {
    cout << "[k]\% must be bigger" << endl;
    cout << "USAGE: ./program.o [a|b|c|d] [n] [k] [w] [crs|ccs] (k: 0\%-100\%)" << endl;
    return 0;
  }
  else if (k > 100) {
    cout << "[k]\% must be lower than 100\%" << endl;
    cout << "USAGE: ./program.o [a|b|c|d] [n] [k] [w] [crs|ccs] (k: 0\%-100\%)" << endl;
    return 0;
  }

  MatrixGenerator *gen = new MatrixGenerator(n, k);

  cout << endl;
  return 0;
}

// ./program.o [a|b|c|d] [m] [n] [k] [w] [crs|ccs] [filename]