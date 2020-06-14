
struct node
{
	int value;
	struct node *next;
};

struct node *Flip(struct node *node)
{
	struct node *new_head = NULL;
	
	if(NULL == node->next)
	{
		return node;
	}
	
	new_head = Flip(node);

	node->next->next = node;
	node->next = NULL;
	
	return new_head;
}
