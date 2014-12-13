#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
class heap
{
public:
	heap();
	void add(int id, double feature);
	void medium(int &id, double &feature);
	void sort();
	void destroy();
	vector<int> order;
	int index(int i){return order[i];}
private:
	struct node
	{
		int id;
		double feature;
		node* left;
		node* right;
	};

	node *root;
	void lfr(node* cnode, int &count, int &target, node* mid);
	void destroy_lrf(node* cnode);
	int size;
};