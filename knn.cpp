#include "knn.h"

knn::knn(int k, vector<double>feature)
{
	numneighbor = k;
	v = new vector<double>[k];
	dis.resize(k,DBL_MAX);
	center = feature;
	names.resize(k);
	return;
}

void knn::add(vector<double>feature, int name)
{
	printf("#\n");
	double d = eud2(center, feature);
	for(int i=0;i<numneighbor;i++)
	{
		if(d<dis[i])
		{
			double tempd = dis[i];
			dis[i] = d;
			d = tempd;
			int tempi = names[i];
			names[i] = name;
			name = tempi;
			vector<double>tempv = v[i];
			v[i] = feature;
			feature = tempv;
		}
	}


	return;
}
int knn::getk()
{
	return numneighbor;
}

vector<double> knn::getfeature(int i)
{
	return v[i];
}

int knn::getname(int i)
{
	return names[i];
}

double knn::getdis(int i)
{
	return dis[i];
}

double knn::eud2(vector<double> &f1, vector<double> &f2)
{
	double dis=0;

	for(int i=0;i<f1.size();i++)
	{
		double tempd = f1[i]-f2[i];
		dis += tempd*tempd;
	}

	return dis;
}

vector<double> knn::getcenter()
{
	return center;
}

double knn::getentry(int i)
{
	return center[i];
}