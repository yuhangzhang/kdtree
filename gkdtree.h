
#include <vector>
#include <cmath>
#include "../poly/poly.h"
#include "heap.h"




class gkdtree 
{
	
private:	
	int size;
	int oldsize;
	
	double eud2(vector<double> &f1, vector<double> &f2);

	struct leaf
	{
		int label;
		vector<double> feature;
	};	
	struct parent
	{
		int axis;
		double value;
		parent * left;
		parent * right;
		leaf * son;
	};

	//parent *root;
	void createnode(int axis, double value,parent*& n);
	int fdim;
	void postorder(parent* n, vector<leaf*> &list);
	void pssavetree(FILE *fp,parent *node);
public:
	parent *root;
	gkdtree(int fd);
	int getsize(){return size;}
	void checkandadd(vector<double> &feature,int label);	
	void search(vector<double> &feature);

	void plottree(parent *n);	
	void savekdtree(char* fname);
	
	void rebuild();
	void kdtree(vector<leaf*> &list, int axis, parent* &n);
	
};	