#include "gkdtree.h"


gkdtree::gkdtree(int fd)
{
	root=NULL;
	size=0;
	oldsize=0;
	fdim = fd;
	return;
}

void gkdtree::createnode(int axis, double value,parent*& n)
{
	n = new parent;

	n->axis = axis%fdim;
	n->value = value;
	n->left=NULL;
	n->right=NULL;
	n->son=NULL;

	return;
}

void gkdtree::checkandadd(vector<double> &feature,int label)
{
	if(root == NULL)
	{
		createnode(0,feature[0],root);
		root->son = new leaf;
		root->son->feature = feature;
		root->son->label = label;
		size++;
		return;
	}

	parent* tempn = root;


	while(tempn->son==NULL)
	{
		if(feature[tempn->axis]<tempn->value)
		{
			tempn = tempn->left;
		}
		else
		{
			tempn = tempn->right;
		}
	}

	if(tempn->son->label!=label)
	{
		createnode(tempn->axis+1,0,tempn->left);
		createnode(tempn->axis+1,0,tempn->right);
		size++;
		leaf *nleaf = new leaf;
		nleaf->label=label;
		nleaf->feature=feature;

		if(feature[tempn->axis]<tempn->value)	
		{
			tempn->left->son=nleaf;
			tempn->right->son = tempn->son;
			tempn->left->value = nleaf->feature[tempn->left->axis];
			tempn->right->value = tempn->son->feature[tempn->right->axis];
		}
		else 
		{
			tempn->left->son=tempn->son;
			tempn->right->son=nleaf;
			tempn->left->value = tempn->son->feature[tempn->right->axis]; 
			tempn->right->value = nleaf->feature[tempn->left->axis];
		}
		tempn->son = NULL;
		
	}

	if(size>4*oldsize) 
	{
		printf("size=%d %d ",oldsize,size);
		rebuild();
		printf("%d\n",size);
	}

	return;

}

void gkdtree::postorder(parent* n,vector<leaf*> &list)
{
	if(n==NULL) return;
	else
	{
		postorder(n->left,list);
		postorder(n->right,list);
		if(n->son!=NULL) list.push_back(n->son);
		delete n;
	}

	return;
}
void gkdtree::pssavetree(FILE *fp,parent *node)
{
	if(node==NULL) return;
	else
	{
		pssavetree(fp,node->left);
		pssavetree(fp,node->right);
		if(node->son!=NULL)
		{
			fprintf(fp,"%d ", node->son->label);
			for(int i=0;i<fdim;i++)
			{
				fprintf(fp,"%f ",node->son->feature[i]);
			}
			fprintf(fp,"\n");
		}
	}

	return;
}
void gkdtree::savekdtree(char* fname)
{
	printf("saving tree...\n");
	FILE *fp = fopen(fname,"w");
	pssavetree(fp,root);
	printf("done\n");
	fclose(fp);
	return;
}

void gkdtree::plottree(parent *n)
{
	if(n==NULL) return;
	if(n->son!=NULL)
	{
		printf("L(%f, %f)\n",n->son->feature[0],n->son->feature[1]);
	}
	else
	{
		printf("-(%d, %f)\n",n->axis,n->value);
	}
	
	plottree(n->left);
	plottree(n->right);


	return;
}


void gkdtree::rebuild()
{
	vector<leaf*> list;
	list.clear();
	size=0;
	oldsize=0;

	postorder(root,list);
	root = NULL;

	kdtree(list,0,root);

	oldsize=size;

	return;
}

void gkdtree::kdtree(vector<leaf*> &list, int axis, parent* &n)
{
	//printf("in as %d\n",list.size());
	axis = axis%fdim;

	if(list.size()==0)
	{
		return;
	}	
	else if(list.size()==1)
	{
		createnode(axis,list[0]->feature[axis],n);
		n->son = list[0];
		size++;
		return;
	}

	heap sorttree;
	int label = list[0]->label;

	for(int i=0;i<list.size();i++)
	{
		sorttree.add(i,list[i]->feature[axis]);
		if(list[i]->label!=list[0]->label) label = list[i]->label;
	}

	if(label==list[0]->label)
	{
		sorttree.destroy();
		for(int i=1;i<list.size();i++) delete list[i];
		createnode(axis,list[0]->feature[axis],n);
		n->son = list[0];
		size++;
		//printf("cut short\n");
		return;
	}
	else
	{
		sorttree.sort();

		vector<leaf*> left,right;

		

		for(int i=0;i<list.size();i++)
		{
			if(i<list.size()/2) 
			{
				left.push_back(list[sorttree.index(i)]);
			}
			else if(i==list.size()/2)
			{
				createnode(axis,list[sorttree.index(i)]->feature[axis],n);
				right.push_back(list[sorttree.index(i)]);
				//n->son = list[sorttree.index(i)];	
				//size++;
			}
			else
			{
				right.push_back(list[sorttree.index(i)]);
			}	
		}

		//printf("total-%d left-%d right-%d\n",list.size(),left.size(),right.size());
		//printf("left\n");
		kdtree(left,axis+1,n->left);
		//printf("right\n");
		kdtree(right,axis+1,n->right);

		sorttree.destroy();
		left.clear();
		right.clear();

		return;

	}



}