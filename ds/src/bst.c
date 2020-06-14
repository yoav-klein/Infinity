/***********************************************
*		Binary Search Tree
*		Author: Yoav Klein
*		Reviewer: Inbar Namdar
*		Date: 3.7.19
***************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/bst.h" /* BSTCreate */
#include "../include/sll.h" /* SLLCreate */

enum side{LEFT, RIGHT};

struct bst_node
{
	void *data;
	bst_it_t parent;
	bst_it_t children[2];
};

struct bst
{
	struct bst_node dummy;
	cmp_f compare;
	const void *params;
};

bst_t *BSTCreate(cmp_f compare, const void *params)
{
	bst_t *bst = (bst_t*)malloc(sizeof(bst_t));
	if(NULL == bst)
	{
		return NULL;
	}
	
	bst->compare = compare;
	bst->params = params;
	bst->dummy.data = (void*)0xDEADBEEF;
	bst->dummy.parent = NULL;
	bst->dummy.children[RIGHT] = NULL;
	bst->dummy.children[LEFT] = NULL;
	
	return bst;
}

void BSTDestroy(bst_t *bst)
{
	bst_it_t current = NULL; 
	bst_it_t parent = NULL;
	enum side freed_side = LEFT;
	
	assert(NULL != bst);
	
	current = bst->dummy.children[LEFT];
	parent = &bst->dummy;
	
	while(!BSTIsEmpty(bst))
	{
		if(NULL != current->children[LEFT])
		{
			parent = current;
			current = current->children[LEFT];
		}
		else if(NULL != current->children[RIGHT])
		{
			parent = current;
			current = current->children[RIGHT];
		}
		else
		{
			freed_side = current->parent->children[LEFT] == current ? LEFT : RIGHT;
			free(current);
			current = parent;
			current->children[freed_side] = NULL;
			parent = current->parent;
		}
	}
	
	free(bst);
}

bst_it_t BSTInsert(bst_t *bst, void *data)
{
	bst_it_t current = NULL;
	bst_it_t parent = NULL;
	int res = 0; 
	
	enum side insert_side = LEFT;
	struct bst_node *new_node = NULL;
	
	assert(NULL != bst && NULL != data);
	
	current = bst->dummy.children[0];
	parent = &bst->dummy;
	
	while(NULL != current)
	{
		res = bst->compare(current->data, data, bst->params);
		parent = current;
	
		if(0 < res)
		{
			current = current->children[LEFT];
			insert_side = LEFT;
		}
		else if(0 > res)
		{
			current = current->children[RIGHT];
			insert_side = RIGHT;
		} 
		else
		{ 
			return &bst->dummy;
		}
	}
	
	new_node = (struct bst_node*)malloc(sizeof(struct bst_node));
	if(NULL == new_node)
	{
		return &bst->dummy;
	}
	
	new_node->data = data;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;	
	new_node->parent = parent;
	parent->children[insert_side] = new_node;
	
	return new_node;
}

void BSTRemove(bst_it_t node)
{
	bst_it_t subs = NULL;
	enum side node_side = LEFT;
	
	assert(NULL != node);
	
	if(NULL != node->children[LEFT] && NULL != node->children[RIGHT])
	{
		subs = BSTNext(node);		
		node->data = subs->data;
		BSTRemove(subs);
	}
	else
	{
		node_side = node == node->parent->children[LEFT] ? LEFT : RIGHT;	
		subs = NULL != node->children[LEFT] ? node->children[LEFT] : node->children[RIGHT];
		node->parent->children[node_side] = subs;
		if(NULL != subs)
		{
			subs->parent = node->parent;
		}
		
		free(node);
	}
}

bst_it_t BSTFind(const bst_t *bst, const void *data)
{
	bst_it_t current = NULL;
	int res = 0;
	
	assert(NULL != bst && NULL != data);
	
	 current = bst->dummy.children[LEFT];
	
	while(NULL != current)
	{
		res = bst->compare(current->data, data, bst->params);
		
		if(0 < res)
		{
			current = current->children[LEFT];
		}
		else if(0 > res)
		{
			current = current->children[RIGHT];
		}
		else
		{
			return current;
		}
	}
	
	return NULL == current ? (bst_it_t)&bst->dummy : current;
}

bst_it_t BSTNext(bst_it_t node)
{
	assert(NULL != node);

	if(NULL != node->children[RIGHT])
	{
		node = node->children[RIGHT];
		while(NULL != node->children[LEFT])
		{
			node = node->children[LEFT];
		}
		
		return node;
	}
	else
	{
		while(node != node->parent->children[LEFT])
		{
			node = node->parent;
		}
		
		return node->parent;
	}
}

bst_it_t BSTPrev(bst_it_t node)
{
	assert(NULL != node);
	
	if(NULL != node->children[LEFT])
	{
		node = node->children[LEFT];
		while(NULL != node->children[RIGHT])
		{
			node = node->children[RIGHT];
		}
		
		return node;
	}	
	else
	{
		while(NULL != node->parent && node != node->parent->children[RIGHT])
		{
			node = node->parent;
		}
		
		return NULL == node->parent ? NULL : node->parent;
	}
}

int BSTForEach(bst_it_t from, bst_it_t to, act_f action, const void *args)
{
	int status = 0;
	
	assert(NULL != from && NULL != to && NULL != action);
	
	while(from != to && !status)
	{
		status = action(from->data, args);
		from = BSTNext(from);
	}
	
	return status;
}

void *BSTGetData(bst_it_t node)
{
	assert(NULL != node);

	return node->data;
}

bst_it_t BSTBegin(const bst_t *bst)
{
	bst_it_t current = NULL; 
	bst_it_t parent = NULL;
	
	assert(NULL != bst);
	
	current = bst->dummy.children[LEFT];
	parent = (bst_it_t)&bst->dummy;
	
	while(NULL != current)
	{
		parent = current;
		current = current->children[LEFT];
	}
	
	return parent;
}

bst_it_t BSTEnd(const bst_t *bst)
{	
	bst_it_t current = NULL;
	bst_it_t parent = NULL;

	assert(NULL != bst);
	
	current = bst->dummy.children[LEFT];
	parent = (bst_it_t)&bst->dummy;

	while(NULL != current)
	{
		parent = current;
		current = current->children[RIGHT];
	} 
	
	return parent;
}

int BSTIsEmpty(const bst_t *bst)
{
	assert(NULL != bst);	
	
	return (NULL == bst->dummy.children[LEFT]);	
}

size_t BSTCount(const bst_t *bst)
{
	size_t count = 0;
	sll_t *sll = SLLCreate();
	
	bst_it_t current = NULL;
	int flag = 1;
	
	current = bst->dummy.children[LEFT];
	
	while(1 == flag)
	{
		if(NULL != current && (bst_it_t)SLLGetData(SLLBegin(sll)) != current)
		{
			SLLPushFront(sll, current);
			current = current->children[LEFT];		
		}
		else if(NULL == current)
		{
			current = (bst_it_t)SLLGetData(SLLBegin(sll));
			flag = SLLIsEmpty(sll) ? 0 : 1;
		}
		else
		{
			++count;
			current = current->children[RIGHT];
			SLLPopFront(sll);
		}
	}
	
	SLLDestroy(sll);
	
	return count;
}


/* Another version of count

size_t BSTCount(const bst_t *bst)
{
	size_t count = 0;
	bst_it_t current = BSTBegin(bst);
	
	while(current != BSTEnd(bst))
	{
		++count;
		current = BSTNext(current);
	}
	
	return count + 1 - BSTIsEmpty(bst); *//* adding one is because when current reaches
}											BSTEnd it won't count it. if it's empty, 
											i don't want this 1 so i subtract IsEmpty */


