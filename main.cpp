#include "MatrixGenerator.h"
#include <sstream>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "USAGE: ./program.o [n] [k] (k: 0\%-100\%)" << endl;
    return 0;
  }

  int n = atoi(argv[1]);
  int k = atoi(argv[2]);

  if ((int)((double)n*(double)n*((double)k/100.0) < n)) {
    cout << "[k]\% must be bigger" << endl;
    cout << "USAGE: ./program.o [n] [k] (k: 0\%-100\%)" << endl;
    return 0;
  }
  else if (k > 100) {
    cout << "[k]\% must be lower than 100\%" << endl;
    cout << "USAGE: ./program.o [n] [k] (k: 0\%-100\%)" << endl;
    return 0;
  }

  MatrixGenerator *gen = new MatrixGenerator(n, k);

  cout << endl;
  return 0;
}
