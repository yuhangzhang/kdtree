#include "ikdtree.h"


ikdtree::ikdtree(int featured)
{
	featuredim = featured;
	featureset.push_back(vector<double>(0));
	labelset.push_back(-1);
	neighbors=NULL;
	return;
}


void ikdtree::printtree(int root)
{	
	if(tree.getTerm(vector<int>(1,root))==0) return;

	printf("[%d]:",root);
	printf("( ");
	for(int i=0;i<featuredim;i++)
	{
		printf("%f ",featureset[tree.getTerm(vector<int>(1,root))][i]);
	}
	printf(") || ");

	printtree(root*2+1);
	printtree(root*2+2);

	return;
}

void ikdtree::rebuild()
{
	tree.destroy();

	vector<int> active(featureset.size()-1);

	for(int i=0;i<featureset.size()-1;i++)
	{
		active[i] = i+1;
	}

	kdtree(active, 0, 0);

	//printtree(0);

	//printf("type to continue\n");
	//getchar();


	return;
}


void ikdtree::dftraversal(int root, vector<double> adis)
{
	//printf("in\n");
	
	if(tree.getTerm(vector<int>(1,root))==0) return;

//printf("node_(%f,%f)\n",featureset[tree.getTerm(vector<int>(1,root))][0],featureset[tree.getTerm(vector<int>(1,root))][1]);
//printf("ad = %f %f\n",adis[0],adis[1]);
	int axis =  ((int)ceil(log(root+2.0)/log(2.0))-1)%featuredim;
	//printf("root=%d axis=%d\n",root,axis);
	//printf("%f\n",neighbors->getentry(axis));
	//printf("%f\n",featureset[tree.getTerm(vector<int>(1,root))][axis]);
	//exit(0);
	double tempd = neighbors->getentry(axis)-featureset[tree.getTerm(vector<int>(1,root))][axis];
	tempd *= tempd;

	if(neighbors->getentry(axis)<featureset[tree.getTerm(vector<int>(1,root))][axis])
	{
		//printf("left\n");
		dftraversal(root*2+1,adis);

		if(adis[axis]<tempd) adis[axis] = tempd;
		
		tempd = 0;
		for(int i=0;i<adis.size();i++)
		{
			tempd+=adis[i];
		}

		//printf("%f %f\n",tempd,neighbors->getdis(neighbors->getk()-1));

		if(tempd<neighbors->getdis(neighbors->getk()-1))
		{
			neighbors->add(featureset[tree.getTerm(vector<int>(1,root))],tree.getTerm(vector<int>(1,root)));
			
			dftraversal(root*2+2,adis);
		}
	}
	else
	{
		//printf("right\n");
		dftraversal(root*2+2,adis);

		if(adis[axis]<tempd) adis[axis] = tempd;
		
		tempd = 0;
		for(int i=0;i<adis.size();i++)
		{
			tempd+=adis[i];
		}

		//printf("%f %f\n",tempd,neighbors->getdis(neighbors->getk()-1));

		if(tempd<neighbors->getdis(neighbors->getk()-1))
		{
			neighbors->add(featureset[tree.getTerm(vector<int>(1,root))],tree.getTerm(vector<int>(1,root)));
			
			dftraversal(root*2+1,adis);
		}
	}


	return;
}

void ikdtree::search(int k, vector<double> feature)
{
	if(neighbors!=NULL) delete neighbors;

	neighbors = new knn(k,feature);

	vector<double> adis(featuredim,0);

	dftraversal(0, adis);

	return;
}


void ikdtree::addnode(vector<double> & feature, int label)
{
	featureset.push_back(feature);
	labelset.push_back(label);

	for(int i=0,j=0,k;;i++)//i-axis,j-current node, k-the feature on current node 
	{
		k=tree.getTerm(vector<int>(1,j));

		if(k==0)
		{
			tree.addTerm(vector<int>(1,j),featureset.size()-1);
			break;
		}
		else if(feature[i]<featureset[k][i])
		{
			j = 2*j+1;
		}
		else
		{
			j = 2*j+2;
		}
		//printf("axis=%d node=%d feature=%d\n",i,j,k);
	}

	//printf("lastvar=%d numterm=%d\n",tree.lastVar(),tree.numTerm());
	if(tree.lastVar()>tree.numTerm()*2)
	{
		rebuild();
	}

	return;
}

double ikdtree::eud2(vector<double> &f1, vector<double> &f2)
{
	double dis=0;

	for(int i=0;i<f1.size();i++)
	{
		double tempd = f1[i]-f2[i];
		dis += tempd*tempd;
	}

	return dis;
}

void ikdtree::sort(vector<double> &feature, vector<int> &index)
{
	index.resize(feature.size());

	if(feature.size()==0)
	{
		return;
	}

	vector<double> left;
	vector<double> right;

	for(int i=1;i<feature.size();i++)
	{
		if(feature[i]<feature[0])
		{
			left.push_back(feature[i]);
			index[i] = -left.size();
		}
		else
		{
			right.push_back(feature[i]);
			index[i] = right.size();
		}
	}

	vector<int> lindex;
	vector<int> rindex;
	
	sort(left,lindex);
	sort(right,rindex);

	index[0] = left.size();
	left.push_back(feature[0]);
	left.insert(left.end(),right.begin(),right.end());
	
	feature = left;

	for(int i=1;i<feature.size();i++)
	{
		if(index[i]<0)
		{
			index[i] = lindex[-index[i]-1];
		}
		else
		{
			index[i] = rindex[index[i]-1]+index[0]+1;
		}
	}
		
	return;
}


void ikdtree::kdtree(vector<int> &active, int axis, int root)
{
	if(active.size()==0)
	{
		return;
	}
	//else if(active.size()==1)
	//{
	//	tree.addTerm(vector<int>(1,root),active[0]);
	//	return;
	//}


	vector<double> feature;
	vector<int> index;

	for(int i=0;i<active.size();i++)
	{
		//printf("i=%d/%d active[i]=%d\n",i,active.size(),active[i]);
		feature.push_back(featureset[active[i]][axis]);	
	}

	sort(feature,index);

	//tree.addTerm(vector<int>(1,root),(feature[feature.size()/2]+feature[feature.size()/2+1])/2);

	vector<int> left, right;

	
	for(int i=0;i<active.size();i++)
	{
		if(index[i]<active.size()/2) 
		{
			left.push_back(active[i]);
		}
		else if(index[i]==active.size()/2)
		{
			tree.addTerm(vector<int>(1,root),active[i]);
		}
		else
		{
			right.push_back(active[i]);
		}	
	}
	
	vector<int> lindex, rindex;

	kdtree(left,(axis+1)%featuredim,root*2+1);

	kdtree(right,(axis+1)%featuredim,root*2+2);

	return;
}




