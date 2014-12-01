#include "ikdtree.h"


//exe k f1 f2
int main(int argc, char *argv[])//test sort
{
	ikdtree testtree(2);
	vector<int> index;
	vector<double> feature(2);

	for(int i=0;i<10;i++)
	{
		feature[0] = i;
		for(int j=0;j<10;j++)
		{
			//printf("ij=%d,%d\n",i,j);
			feature[1] = j;

			testtree.addnode(feature,(i+j)%2);
		}
	}
	//printf("\n\n\n");
	//testtree.rebuild();

	int k=atoi(argv[1]);
	vector<double> tempv(2);
	tempv[0] = atof(argv[2]);
	tempv[1] = atof(argv[3]);
	testtree.search(k,tempv);


	for(int i=0;i<k;i++)
	{
		printf("(%f, %f) %d\n",testtree.neighbors->getfeature(i)[0],testtree.neighbors->getfeature(i)[1], testtree.labelset[testtree.neighbors->getname(i)]);
	}


	return 0;
}