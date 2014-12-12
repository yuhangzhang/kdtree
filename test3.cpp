#include "heap.h"


int main3()
{
	heap testheap;

	for(int i=0;i<2;i++)
	{
		printf("i=%d\n",i);
		testheap.add(i,double(i));
	}

	int id;
	double feature;

	

	testheap.medium(id,feature);

	printf("medi=%d %f\n",id,feature);

	return 0;
}