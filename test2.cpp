#include "akdtree.h"

#define SIZE 5
//exe k f1 f2
int main2(int argc, char *argv[])//test sort
{
	akdtree testtree(2);
	vector<int> index;
	vector<double> feature(2);

	for(int i=0;i<SIZE;i++)
	{
		feature[0] = i;
		for(int j=0;j<SIZE;j++)
		{
			printf("ij=%d,%d size=%d\n",i,j,testtree.size());
			feature[1] = j;

			if(i>j)
				testtree.checkandadd(feature,0);
			else
				testtree.checkandadd(feature,1);
			//testtree.plottree(0);
			//getchar();
		}
		//testtree.plottree(0);getchar();
	}

	testtree.plottree(0);
	testtree.savekdtree("test1.txt");
	testtree.rebuild();
	testtree.plottree(0);
	testtree.savekdtree("test2.txt");

	return 0;
}