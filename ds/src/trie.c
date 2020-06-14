/******************************************************
*		Trie
*		Author: Yoav Klein
*		Reviewer: Andrei
*		Date: 
******************************************************/

#include <stdlib.h> /* malloc */
#include <math.h> /* pow */

#include "trie.h"

#define NUM_CHILDREN (2)
#define OP_CHILD(x) ((x) == 1 ? 0 : 1)

enum children { LEFT = 0, RIGHT };

struct node
{
	int is_full;
	struct node *children[NUM_CHILDREN];
};

struct trie
{
	node_t *root;
	size_t num_of_bits;
};

static int IsBitOn(uint32_t num, int index);
static void DestroyRec(node_t *root);
static int AreChildrenFull(node_t *root);
static enum trie_status InsertRec(node_t *root, int count, uint32_t ip_req, uint32_t *res);
static int RemoveRec(node_t *root, int count, uint32_t address);
static size_t CountTakenRec(const node_t *root, int count);

static node_t *NodeCreate()
{
	node_t *node = malloc(sizeof(*node));
	if(NULL == node)
	{
		return NULL;
	}
	
	node->is_full = 0;
	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;
	
	return node;
}

trie_t *TrieCreate(size_t num_of_bits)
{
	trie_t *trie = malloc(sizeof(*trie));
	if (NULL == trie)
	{
		return NULL;
	}
	
	trie->root = NodeCreate();
	if(NULL == trie->root)
	{
		free(trie);
		
		trie = NULL;
		
		return NULL;
	}
	
	trie->num_of_bits = num_of_bits;
	
	return trie;
}

static void DestroyRec(node_t *root)
{
	if(NULL == root)
	{
		return;
	}
	
	DestroyRec(root->children[LEFT]);
	DestroyRec(root->children[RIGHT]);
	
	free(root);
	
	root = NULL;
}

void TrieDestroy(trie_t *trie)
{
	DestroyRec(trie->root);
	
	free(trie);
	
	trie = NULL;
}

static int AreChildrenFull(node_t *root)
{
	return
	NULL != root->children[LEFT] &&
	NULL != root->children[RIGHT] &&
	root->children[LEFT]->is_full &&
	root->children[RIGHT]->is_full;
}

static int IsBitOn(uint32_t num, int index)
{
	return 0 < (num & (0x1u << index));
}

static enum trie_status InsertRec(node_t *root, int count, uint32_t ip_req, uint32_t *res)
{
	enum trie_status status = 0; 
	int child = 0; 
	node_t *new_root = NULL;
	
	if(0 == count)
	{
		root->is_full = 1;

		return T_OK;
	}
	
	child = IsBitOn(ip_req, count - 1);
	
	if (NULL == root->children[child] || 0 == root->children[child]->is_full) 
	{
		*res = (*res << 1) + child;
		
		if(NULL == root->children[child])
		{
			root->children[child] = NodeCreate();
			if(NULL == root->children[child])
			{
				return T_E_ALLOC;
			}		
		}
		
		new_root = root->children[child];
	}
	else
	{
		*res = (*res << 1) + OP_CHILD(child);
		
		if(NULL == root->children[OP_CHILD(child)])
		{
			root->children[OP_CHILD(child)] = NodeCreate();
			if(NULL == root->children[OP_CHILD(child)])
			{
				return T_E_ALLOC;
			}
		}
		
		new_root = root->children[OP_CHILD(child)];
		ip_req = 0;
	}
	
	status = InsertRec(new_root, count - 1, ip_req, res);
	
	if (AreChildrenFull(root))
	{
		root->is_full = 1;	
	}
	
	return status;
}

enum trie_status TrieInsert(trie_t *trie, uint32_t address, uint32_t *res)
{
	enum trie_status status = 0;
	uint32_t ret_val = 0;
	
	if(1 == trie->root->is_full)
	{
		return T_NO_FREE_ADDR;
	}
	
	status = InsertRec(trie->root, trie->num_of_bits, address, &ret_val);
	
	*res = ret_val;
	
	if(T_E_ALLOC == status)
	{
		return status;
	}
	
	if(*res != address)
	{
		status = T_ADDR_TAKEN;
	}
	
	return status;
}

static int RemoveRec(node_t *root, int count, uint32_t address)
{
	uint32_t child = 0;
	
	if (NULL == root)
	{
		return 1;
	}
	
	if(0 == count)
	{
		if(0 == root->is_full)
		{
			return 1;
		}
		else
		{
			root->is_full = 0;
			
			return 0;
		}
	}
	
	child = IsBitOn(address, count - 1);
	
	root->is_full = 0;
	root = root->children[child];
	
	return RemoveRec(root, --count, address);
}

int TrieRemove(trie_t *trie, uint32_t key)
{
	return RemoveRec(trie->root, trie->num_of_bits, key);
}

static size_t CountTakenRec(const node_t *root, int count)
{
	if(NULL == root)
	{
		return 0;
	}
	
	if(root->is_full)
	{
		return pow(NUM_CHILDREN, count);
	}
	
	return CountTakenRec(root->children[LEFT], count - 1) + 
	       CountTakenRec(root->children[RIGHT], count - 1);
}

size_t TrieCount(const trie_t *trie)
{
	size_t total_size = pow(NUM_CHILDREN, trie->num_of_bits);
	
	return total_size - CountTakenRec(trie->root, trie->num_of_bits); 
}


