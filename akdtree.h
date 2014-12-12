
#include <vector>
#include <cmath>
#include "../poly/poly.h"
#include "knn.h"
#include "heap.h"


//typedef unsigned __int64 LINT;
typedef int LINT;

class akdtree //incremental kd tree
{
public:
	akdtree(int featured);


	int getsize(){return featureset.size();}
	void checkandadd(vector<double> &feature,char label);	
	void search(vector<double> &feature);
	int getnode(int i);
	int getfeature(int i);
	void plottree(LINT root);	
	void savekdtree(char* fname);
	LINT size();
	void rebuild();
	void kdtree(vector<int> &active, int axis, int root);
private:	
	vector<vector<double> > featureset;
	vector<char> labelset;
	int featuredim;
	int oldtreesize;
	poly<LINT> tree;

	double eud2(vector<double> &f1, vector<double> &f2);

};	