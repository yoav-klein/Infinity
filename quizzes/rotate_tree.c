#include <stdio.h>
#include <stdlib.h>

#define HEIGHT (4)

int g_counter = 0;
enum children { LEFT, RIGHT, NUM_CHILDREN };

typedef struct node
{
	int value;
	struct node *children[NUM_CHILDREN];
} node_t;

node_t *CreateNode(int value)
{
	node_t *root = NULL;	
	
	root = malloc(sizeof(node_t));
	if(NULL == root)
	{
		return NULL;
	}
	
	root->value = value;
	root->children[LEFT] = NULL;
	root->children[RIGHT] = NULL;
	
	return root;
}


node_t *RecCreateTree(node_t *root, int height)
{
	if(height == HEIGHT)
	{
		return NULL;
	}
	
	if(root == NULL)
	{
		root = CreateNode(++g_counter);
	}
	
	++height;
	
	root->children[LEFT] = RecCreateTree(root->children[LEFT], height);
	root->children[RIGHT] = RecCreateTree(root->children[RIGHT], height);
	
	return root;
}

node_t *CreateTree()
{
	node_t *root = NULL;
	root = RecCreateTree(root, 0);
	
	return root;
}


void PrintTreeStraight(node_t *root)
{
	if(NULL == root)
	{
		return;
	}
	
	printf("%d\n", root->value);
	PrintTreeStraight(root->children[LEFT]);
	PrintTreeStraight(root->children[RIGHT]);
}

void RotateTree(node_t *root)
{
	node_t *temp = NULL;
	
	if(NULL == root)
	{
		return;
	}
	
	temp = root->children[LEFT];
	root->children[LEFT] = root->children[RIGHT];
	root->children[RIGHT] = temp;
	
	RotateTree(root->children[LEFT]);
	RotateTree(root->children[RIGHT]);
}

void PrintTreeRotated(node_t *root)
{
	if(NULL == root)
	{
		return;
	}	
	
	printf("%d\n", root->value);
	PrintTreeRotated(root->children[RIGHT]);
	PrintTreeRotated(root->children[LEFT]);
}

void Test()
{
	node_t *root = NULL;
	
	root = CreateTree();
	
	PrintTreeStraight(root);
	
	RotateTree(root);
	
	
	PrintTreeRotated(root);
	
}

int main()
{
	Test();
	
	return 0;
}
