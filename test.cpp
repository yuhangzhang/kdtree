#include "ikdtree.h"

int main()//test sort
{
	FILE *fpr = fopen("testinput.txt","r");
	
	vector<double> data;

	double tempd;

	while(fscanf(fpr,"%lf",&tempd)==1)
	{
		printf("%lf\n",tempd);
		data.push_back(tempd);
	}

	fclose(fpr);

	ikdtree testtree(1);
	vector<int> index;
	
	testtree.sort(data,index);

	for(int i=0;i<data.size();i++)
	{
		printf("%d %f %d\n",i,data[i],index[i]);
	}

	return 0;
}