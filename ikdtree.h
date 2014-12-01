
#include <vector>
#include <cmath>
#include "../poly/poly.h"
#include "knn.h"

class ikdtree //incremental kd tree
{
public:
	ikdtree(int featured);

	void addnode(vector<double> &feature,int label);
	void rebuild();
	void search(int k, vector<double> feature);
	int getnode(int i);
	int getfeature(int i);



	
	vector<vector<double> > featureset;
	vector<int> labelset;

//private:
	int featuredim;

	poly<int> tree;

	knn *neighbors;

	void kdtree(vector<int> &active, int axis, int root);

	double eud2(vector<double> &f1, vector<double> &f2);
	void sort(vector<double> &feature, vector<int> &index);//sort an sequence of scalars
	void printtree(int root);
	void dftraversal(int root, vector<double> adis);//depth first traversal
};	