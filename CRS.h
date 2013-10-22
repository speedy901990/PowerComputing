#ifndef CRS_H
#define CRS_H
#include <cstdlib>
#include <vector>
using namespace std;

class CRS{
	public:
		vector<double> val;
	  vector<int> colId;
		vector<int> rowPtr;

		CRS();
};
#endif
