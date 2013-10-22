#ifndef CCS_H
#define CCS_H
#include <cstdlib>
#include <vector>
using namespace std;

class CCS{
	public:
		vector<double> val;
	  vector<int> rowId;
		vector<int> colPtr;

		CCS();
};
#endif