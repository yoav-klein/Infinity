

#include <stdio.h>
#include <stdlib.h>


struct node
{
	int value;
	struct node *left;
	struct node *right;
};

int MaxPath(struct node *root, int n)
{
	int *arr = (int*)calloc(n, sizeof(int));
	
	return MaxPathRec(root, arr);
	
	free(arr);
}

int MaxPathRec(struct node *root, int *arr)
{
	int left_path = 0;
	int right_path = 0;
	int max_path_child = 0;
	
	if (NULL == root)
	{
		return 0;
	}
	
	if (0 == arr[root->value])
	{
		arr[root->value] = 1;
		left_path = MaxPathRec(root->left, arr);
		right_path = MaxPathRec(root->right, arr);
		
		max_path_child = left_path > right_path ? left_path : right_path;
		arr[root->value] = 0;
		
		return 1 + max_path_child;
	}
	
	else
	{
		left_path = MaxPathRec(root->left, arr);
		right_path = MaxPathRec(root->right, arr);
		return left_path > right_path ? left_path : right_path;
	}
}


int main()
{
	struct node root;
	struct node left;
	struct node right;
	struct node left_left;
	struct node left_right;
	struct node right_left;
	struct node right_right;
	
	root.left = &left;
	root.right = &right;
	root.value = 1;
	
	left.right = &left_right;
	left.left = &left_left;
	left.value = 2;
	
	right.right = &right_right;
	right.left = &right_left;
	right.value = 2;
	
	right_right.left = NULL;
	right_right.right = NULL;
	right_right.value = 3;
	
	left_left.right = NULL;
	left_left.left = NULL;
	left_left.value = 1;
	
	left_right.left = NULL;
	left_right.right = NULL;
	left_right.value = 1;
	
	right_left.right = NULL;
	right_left.left = NULL;
	right_left.value = 1;
	
	printf("%d\n", MaxPath(&root, 40));
}
