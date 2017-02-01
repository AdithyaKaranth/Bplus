#include<stdio.h>
#include<stdlib.h>
struct NODE
{
	int a[4],count,leaf,display;
	struct NODE *link[4];
	struct NODE *parent,*next;
};
typedef struct NODE *node;
node getnode(int);
void sort(int [],int);
node search(node,int);
node insert(node,node,int);
static int gName=0,display=0;
FILE *outFile;
void dotDump(node,FILE *);
void preorderDotDump(node,FILE *); 
void main(int argc,char **argv)
{
	FILE *pipe;
	node root=NULL,x;
	int val,n,i;
	scanf("%d",&val);
	for(i=0;i<val;i++)
	{
		scanf("%d",&n);
		x=search(root,n);
		root=insert(root,x,n);
	}
	outFile=fopen("bplus.dot","w");
	fclose(outFile);
	outFile=fopen("bplus.dot","a");
	if(outFile!=NULL)
		dotDump(root,outFile);
	fclose(outFile);
	pipe=popen("dot -Tps bplus.dot -o bplus.ps","w");
	pclose(pipe);
	pipe=popen("evince bplus.ps","r");
	pclose(pipe);
}
void preorderDotDump(node root,FILE *outFile)
{
	int i;
	if(root!=NULL)
	{
		fprintf(outFile,"node%d[label=\"<f0>%d",root->display,root->a[0]);
		i=1;
		while(root->a[i]!=0)
		{
			fprintf(outFile,"|<f%d>%d",i,root->a[i]);
			i++;
		}
		fprintf(outFile,"\"];\n");
		i=0;
		while(root->link[i]!=NULL && i<4)
		{
			fprintf(outFile,"\"node%d\":f%d->\"node%d\":f%d;\n",root->display,0,root->link[i]->display,0);
			i++;
		}
		i=0;
		while(root->link[i]!=NULL && i<4)
		{
			preorderDotDump(root->link[i],outFile);
			i++;
		}
	}
}
void dotDump(node root,FILE *outFile)
{
	gName++;
	fprintf(outFile,"digraph tree{\nnode[shape=record];\n",gName);
	preorderDotDump(root,outFile);
	fprintf(outFile,"}\n");
}
node getnode(int n)
{
	int i;
	node x=(node)malloc(sizeof(struct NODE));
	for(i=0;i<4;i++)
	{
		x->a[i]=0;
		x->link[i]=NULL;
	}
	x->count=0;
	x->leaf=n;
	x->parent=x->next=NULL;
	x->display=display++;
	return x;
}
void sort(int a[],int count)
{
	int i,j,temp;
	for(i=0;i<count-1;i++)
	{
		for(j=0;j<count-i-1;j++)
		{
			if(a[j]>a[j+1])
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
}
node search(node root,int x)
{
	int i,flag=0;
	if(root==NULL)
		return NULL;
	while(root->leaf==0)
	{
		flag=0;
		for(i=0;i<root->count;i++)
		{
			if(x<root->a[i])
			{
				root=root->link[i];
				flag=1;
				break;
			}
		}
		if(flag==1)
			continue;
		root=root->link[i];
	}
	return root;
}
node insert(node root,node x,int key)
{
	int i,flag=0,j;
	node y=NULL,z=NULL;
	if(x==NULL)
	{
		x=getnode(1);
		root=x;
	}
	x->a[(x->count)++]=key;
	sort(x->a,x->count);
	if(x->count==4)
	{
		y=getnode(x->leaf);
		if(x==root)
		{
			node z=getnode(0);
			root=z;
			x->parent=y->parent=root;
			root->link[root->count]=x;//initially 0.assigning the parent to x.
		}
		root=insert(root,x->parent,x->a[1]);//insertion to parent
		y->parent=x->parent;
		for(i=0;i<y->parent->count;i++)
			if(y->parent->a[i]==x->a[1])
			{
				flag=1;
				break;
			}
		if(flag)
		{
			for(j=y->parent->count;j>i+1;j--)
				y->parent->link[j]=y->parent->link[j-1];
			y->parent->link[i+1]=y;
		}
		else
			y->parent->link[i]=y;
		for(i=1;i<4;i++)
		{
			y->a[i-1]=x->a[i];
			x->a[i]=0;
		}
		for(i=2;i<x->count;i++)
		{
			y->link[i-2]=x->link[i];
			x->link[i]=NULL;
		}	
		x->count=1;
		y->count=3;
		if(x->leaf==0)//if x is an internal node
		{
			if(x->link[0]->count==4)
			{
				for(i=y->count;i>0;i--)
					y->link[i]=y->link[i-1];
				y->link[0]=x->link[1];
				x->link[1]=x->link[0];
				x->link[1]->parent=x;
			}
		
			//x->link[1]=NULL;
			for(i=0;i<3;i++)
			{
				y->a[i]=y->a[i+1];
				//y->link[i]=y->link[i+1];
				if(y->link[i]!=NULL)
					y->link[i]->parent=y;
			}
			y->a[3]=0;
			(y->count)--;
			return root;
		}	
		y->next=x->next;
		x->next=y;
	}
	return root;
}	
