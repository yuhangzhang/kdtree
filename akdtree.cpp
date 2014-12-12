#include "akdtree.h"

#define MONI 126


akdtree::akdtree(int featured)
{
	featuredim = featured;
	featureset.push_back(vector<double>(0));
	labelset.push_back(char(NULL));
	oldtreesize=0;
	return;
}



void akdtree::plottree(LINT root)
{	
	printf("(%I64u|%.0f,%.0f)\n",root,featureset[tree.getTerm(vector<LINT>(1,root))][0],featureset[tree.getTerm(vector<LINT>(1,root))][1]);

	vector<LINT> next(2);
	next[0] = root*2+1;
	next[1] = root*2+2;

	while(1)
	{
		vector<LINT> temp(next.size()*2);
		for(int i=0;i<next.size();i++)
		{
			//printf("i=%d\n",i);
			if(tree.getTerm(vector<LINT>(1,next[i]))!=0)
			printf("(%I64u|%.0f,%.0f) ",next[i],featureset[tree.getTerm(vector<LINT>(1,next[i]))][0],featureset[tree.getTerm(vector<LINT>(1,next[i]))][1]);		
			temp[i*2] = next[i]*2+1;
			temp[i*2+1] = next[i]*2+2;
		}		
		printf("\n");
		
		next = temp;
		if(next[0]>tree.lastVar()) break;
	}



	return;
}

void akdtree::search(vector<double> &feature)
{


	return;
}



void akdtree::checkandadd(vector<double> & feature, char label)
{
	//printf("in ca\n");
	if(tree.numTerm()==0) 
	{
		featureset.push_back(feature);
		labelset.push_back(label);
		tree.addTerm(vector<LINT>(1,0),featureset.size()-1);
	}
	else
	{
		LINT node=0;
		LINT k=tree.getTerm(vector<LINT>(1,node));
		//if(k<0) {printf("k<0\n");getchar();}
		//else printf("k==%d\n",k);
		for(int i=0;;i++)
		{	
			//printf("featuredim i=%d k=%d\n",i,k);
			if(i==featuredim)
			{ 
				i = 0;
				//printf("to zero k=%d %d node=%d\n",k,featureset.size(),node);
				
			}

			if(feature[i]<featureset[k][i])
			{//printf("here0\n");
				node = node*2+1;
			}				
			else
			{//printf("here1\n");
				node = node*2+2;
			}


			LINT kk=tree.getTerm(vector<LINT>(1,node));
			//printf("node=%d\n kk=%d\n",node,kk);
			if(kk==0&&label==labelset[k])
			{//printf("here2\n");
				break;
			}
			else if(kk==0)
			{//printf("here3\n");
				featureset.push_back(feature);
				labelset.push_back(label);	

				if(feature[i]<featureset[k][i])
				{
					tree.addTerm(vector<LINT>(1,node),featureset.size()-1);
					tree.addTerm(vector<LINT>(1,node+1),k);
				}
				else
				{					
					tree.addTerm(vector<LINT>(1,node),featureset.size()-1);
					tree.addTerm(vector<LINT>(1,node-1),k);
					tree.editTerm(vector<LINT>(1,(node-2)/2),featureset.size()-1);
				}
				break;
			}
			
			k = kk;
		}
	}

	//printf("here\n");
	if(featureset.size()>oldtreesize*4||tree.lastVar()>INT_MAX/4)
	{
		printf("lastvar=%d numterm=%d\n",tree.lastVar(),tree.numTerm());

		rebuild();
		oldtreesize = tree.numTerm();
	}

	return;
}

LINT akdtree::size()
{
	return tree.numTerm();
}

double akdtree::eud2(vector<double> &f1, vector<double> &f2)
{
	double dis=0;

	for(int i=0;i<f1.size();i++)
	{
		double tempd = f1[i]-f2[i];
		dis += tempd*tempd;
	}

	return dis;
}


void akdtree::savekdtree(char *fname)
{
	FILE *fpw = fopen(fname,"w");

	LINT name = tree.getTerm(vector<LINT>(1,0));

	




	for(poly<LINT>::TERMS::iterator it=tree.firstTerm();it!=tree.lastTerm();it++)
	{
		fprintf(fpw,"%I64u %d ",(it->first)[0],labelset[it->second]);

		for(int i=0;i<featuredim;i++)
		{
			fprintf(fpw,"%f ",featureset[it->second][i]);
		}		

		fprintf(fpw,"\n");
	}

	


	fclose(fpw);

	return;
}

void akdtree::rebuild()
{
	printf("in rebuild %d %d %d\n",tree.lastVar(),tree.numTerm(),featureset.size()-1);
	tree.destroy();

	vector<int> active(featureset.size()-1);

	for(int i=0;i<active.size();i++)
	{
		active[i] = i+1;
	}


	kdtree(active, 0, 0);

	active.clear();

	printf("out rebulid lastvar=%d numterm=%d/%d\n",tree.lastVar(),tree.numTerm(),featureset.size()-1);



	return;
}



void akdtree::kdtree(vector<int> &active, int axis, int root)
{
	//if(featureset.size()>MONI) printf("size=%d axis=%d root=%d\n",active.size(),axis,root);
	if(active.size()==0)
	{
		return;
	}
	else if(active.size()==1)
	{
		tree.addTerm(vector<int>(1,root),active[0]);
		return;
	}

	//printf("active.size=%d\n",active.size());

	heap sorttree;

	//if(featureset.size()>MONI) printf("here\n");
	for(int i=0;i<active.size();i++)
	{
		sorttree.add(i,featureset[active[i]][axis]);
		//printf("sort add %d %f\n",active[i],featureset[active[i]][axis]);
	}

	vector<int> left, right;
	
	int mid;
	double mfeature;

	sorttree.medium(mid,mfeature);
	//printf("active.size=%d\n",active.size());
	//printf("rebuild tree %d %d %d\n",root,active[mid],mid);getchar();
	tree.addTerm(vector<int>(1,root),active[mid]);

	for(int i=0;i<active.size();i++)
	{
		if(featureset[active[i]][axis]<mfeature) 
		{
			left.push_back(active[i]);
		}
		else
		{
			right.push_back(active[i]);
		}	
	}

	
	kdtree(left,(axis+1)%featuredim,root*2+1);

	kdtree(right,(axis+1)%featuredim,root*2+2);

	sorttree.destroy();
	left.clear();
	right.clear();

	return;
}