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
