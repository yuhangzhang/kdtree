#include "heap.h"

heap::heap()
{
	root = NULL;
	size = 0;
	return;
}

void heap::add(int id, double feature)
{
	if(root==NULL)
	{
		root = new node;
		root->id = id;
		root->feature = feature;
		root->left = NULL;
		root->right = NULL;
	}
	else
	{
		node *tempn = root;
		node *tempn2;

		while(tempn!=NULL)
		{		
			tempn2 = tempn;
			if(feature<tempn->feature)
			{
				tempn = tempn->left;
			}
			else
			{
				tempn = tempn->right;
			}
		}

		tempn = new node;
		tempn->id = id;
		tempn->feature = feature;
		tempn->left = NULL;
		tempn->right = NULL;
		
		if(feature<tempn2->feature)
		{
			tempn2->left=tempn;
		}
		else
		{
			tempn2->right=tempn;
		}

	}

	size++;

	return;
}

void heap::lfr(node* cnode, int &count, int &target, node* mid)
{
	//if(count>target){printf("count>target\n");getchar();}
	//else printf("in lfr %d %d\n",count,target);
	if(cnode==NULL)
	{
		mid = NULL;
		return;
	}	
	else
	{
		//printf("at %d %f\n",cnode->id,cnode->feature);
		lfr(cnode->left,count,target,mid);
		if(mid!=NULL)
		{//printf("A\n");
			return ;
		}
		else if(count==target)
		{//printf("B id=%d\n",cnode->id);
			order.push_back(cnode->id);
			mid=cnode;
			return;
		}
		else
		{//printf("C\n");
			order.push_back(cnode->id);
			count++;
			//printf("count=%d size=%d\n",count,size);
			lfr(cnode->right,count,target,mid);
			return;
		}


	}
}

void heap::medium(int &id, double &feature)
{
	int half = size/2;
	int count= 0;
	//printf("in medium count=%d target=%d\n",count,half);
	node *tempn;
	order.clear();
	
	lfr(root,count,half,tempn);

	id = tempn->id;
	feature=tempn->feature;

	//printf("in medium %d %d\n",id,size);

	return;
}
void heap::sort()
{

	//printf("in medium count=%d target=%d\n",count,half);
	node *tempn=NULL;
	order.clear();
	
	int count=0;
	int target=size;
	lfr(root,count,target,tempn);



	//printf("in medium %d %d\n",id,size);

	return;
}

void heap::destroy()
{
	destroy_lrf(root);
	//order.clear();

	return;
}

void heap::destroy_lrf(node* cnode)
{
	if(cnode==NULL) 
	{
		return;
	}
	else
	{
		destroy_lrf(cnode->left);
		destroy_lrf(cnode->right);
		delete cnode;
		return;
	}
}