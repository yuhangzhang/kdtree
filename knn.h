#include <vector>

using namespace std;

class knn
{
public:
	knn(int k, vector<double>feature);
	void add(vector<double>feature, int name);
	vector<double> getfeature(int i);
	int getname(int i);
	vector<double> getcenter();
	double getentry(int i);
	double getdis(int i);
	int getk();

private:
	int numneighbor;
	vector<double> *v;
	vector<double> center;
	vector<double> dis;
	vector<int> names;
	double eud2(vector<double> &f1, vector<double> &f2);

};

