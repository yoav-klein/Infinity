/********************************************
*		AVL
*		Author: Yoav Klein
*		Reviewer: Roi Kramer
*		Date: 11.7.19
**********************************************/

#include <stdlib.h> /* malloc */
#include <sys/types.h> /* ssize_t */

#include "../include/avl.h" /* AVLCreate */

enum side { LEFT, RIGHT };

typedef struct avl_node
{
	void *data;
	struct avl_node *children[2];
	ssize_t height;
} node_t;

struct avl
{
	node_t *root;
	avl_cmp_f compare;
	const void *params;
};

static int HowManyChildren(node_t *node)
{
	if(node->children[LEFT] != NULL && node->children[RIGHT] != NULL)
	{
		return 2;
	}
	else
	{
		return NULL == node->children[LEFT] ? NULL != node->children[RIGHT] : 1;  
	}
}

static node_t *FindSuccessorChild(node_t *node)
{
	return NULL == node->children[LEFT] ? node : 
	 FindSuccessorChild(node->children[LEFT]); 
}

static int WhereTo(avl_cmp_f compare, void *current, 
					const void *new, const void *params)
{
	int result = 0;
	
	result = compare(current, new, params);
	if(0 == result)
	{
		return -1;
	}
	
	return result > 0 ? LEFT : RIGHT;
}

static ssize_t StaticGetHeight(node_t *node)
{
	return NULL == node ? -1 : node->height;
}

static ssize_t BalanceFactor(node_t *node)
{
	return StaticGetHeight(node->children[LEFT]) - 
			StaticGetHeight(node->children[RIGHT]);
}

static ssize_t HighestChild(node_t *node)
{
	ssize_t left_height = StaticGetHeight(node->children[LEFT]);
	ssize_t right_height = StaticGetHeight(node->children[RIGHT]);
	
	return  left_height > right_height ? left_height : right_height;
}

static node_t *CreateNode(void *data)
{
	node_t *node = (node_t*)malloc(sizeof(node_t));
	if(NULL == node)
	{
		return NULL;
	}
	
	node->data = data;
	node->children[RIGHT] = NULL;
	node->children[LEFT] = NULL;
	node->height = 0;
	
	return node;
}

static node_t *Rotate(node_t *root, enum side side)
{	
	enum side opp_side = side == LEFT ? RIGHT : LEFT;
	node_t *pivot = root->children[opp_side];
	
	root->children[opp_side] = pivot->children[side];
	pivot->children[side] = root;
	
	root->height = HighestChild(root) + 1;
	pivot->height = HighestChild(pivot) + 1;
	
	return pivot;
}

static node_t *Balance(node_t *node)
{
	if(BalanceFactor(node) >= 2)
	{
		if(BalanceFactor(node->children[LEFT]) < 0)
		{
			node->children[LEFT] = Rotate(node->children[LEFT], LEFT);
		}
		
		return Rotate(node, RIGHT);
	}
	
	if(BalanceFactor(node) <= -2)
	{
		if(BalanceFactor(node->children[RIGHT]) > 0)
		{
			node->children[RIGHT] = Rotate(node->children[RIGHT], RIGHT);
		}
		
		return Rotate(node, LEFT);
	}
	
	return node;
}

static node_t *InsertRec(avl_t *avl, node_t *node, void *data, int *status)
{
	node_t *new = NULL;
	int direction = 0;
	
	if(NULL == node)
	{
		new = CreateNode(data);
		if(NULL == new)
		{
			*status = 1;
			return NULL;
		}
		*status = 0;
		return new;
	}
	
	direction = WhereTo(avl->compare, node->data, data, avl->params);
	
	if(direction == -1) /* duplicate */
	{
		return node;	
	}
	
	node->children[direction] = InsertRec(avl, node->children[direction], 
															data, status);
	node = Balance(node); 
	node->height = HighestChild(node) + 1;
	
	return node;
}

int AVLInsert(avl_t *avl, void *data)
{
	int status = 0;
	
	avl->root = InsertRec(avl, avl->root, data, &status); 
	
	return status;
}

static void DeleteNodes(node_t *node)
{
	if(NULL == node)
	{
		return;
	}
	DeleteNodes(node->children[RIGHT]);
	DeleteNodes(node->children[LEFT]);
	free(node);
}

static node_t *RemoveRec(avl_t *avl, node_t *node, const void *key)
{
	node_t *ret_node = NULL;
	node_t *successor = NULL;
	int direction = WhereTo(avl->compare, node->data, key, avl->params);
	
	if(-1 == direction) /* -1 means match */
	{
		if(2 == HowManyChildren(node))
		{
			successor = FindSuccessorChild(node->children[RIGHT]);
			node->data = successor->data;
			node->children[RIGHT] = RemoveRec(avl, node->children[RIGHT], 
														successor->data);
		}
		else
		{
			ret_node = node->children[LEFT] == NULL ? node->children[RIGHT] :
														node->children[LEFT];
			free(node);

			return ret_node;
		}	
	}
	else
	{
		node->children[direction] = RemoveRec(avl, node->children[direction], key);
	}
	
	node = Balance(node);
	node->height = HighestChild(node) + 1;
	
	return node;
} 

avl_t *AVLCreate(avl_cmp_f compare, const void *params)
{
	avl_t *avl = (avl_t*)malloc(sizeof(avl_t));
	if(NULL == avl)
	{
		return NULL;
	}
	
	avl->root = NULL;
	avl->compare = compare;
	avl->params = params;
	
	return avl;
}

void AVLDestroy(avl_t *avl)
{
	DeleteNodes(avl->root);
	free(avl);
}

int ForERec(avl_t *avl, avl_act_f act, node_t *node, const void *args)
{
	int res = 0;
	
	if(NULL == node)
	{
		return 0;
	}
	
	/* once 1 returns from left, right or current call, we want to return 1 */
	if(!(res = ForERec(avl, act, node->children[LEFT], args)) && 
	   !(res = act(node->data, args)) && 
	   !(res = ForERec(avl, act, node->children[RIGHT], args)))
	{
		return 0;
	}
	
	return res;
}

int AVLForEach(avl_t *avl, avl_act_f act, const void *args)
{
	return ForERec(avl, act, avl->root, args);
}

void AVLRemove(avl_t *avl, const void *key)
{
	avl->root = RemoveRec(avl, avl->root, key);
}

int AVLIsEmpty(const avl_t *avl)
{
	return  NULL == avl->root ? 1 : 0; 
}

static node_t *FindRec(avl_t *avl, node_t *node, const void *key)
{
	int direction = WhereTo(avl->compare, node->data, key, avl->params);
	/* -1 means match */
	return direction == -1 ? node->data : FindRec(avl, node->children[direction], 
																		  key);
}

void *AVLFind(const avl_t *avl, const void *key)
{
	return FindRec((avl_t*)avl, (node_t*)avl->root, key);
}

static size_t SizeRec(node_t *node)
{
	if(NULL == node)
	{
		return 0;
	}
	
	return 1 + SizeRec(node->children[LEFT]) + SizeRec(node->children[RIGHT]);
}

size_t AVLGetSize(const avl_t *avl)
{
	return SizeRec(avl->root);
}

size_t AVLGetHeight(const avl_t *avl)
{
	return (size_t)avl->root->height;
}

