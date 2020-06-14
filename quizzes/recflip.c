typedef struct Node
{
	void *data;
	struct Node *next;
}node_t;

void FlipListRec(node_t *node)
{
	static node_t *prev = node;
	static node_t *current = node->next;
	static node_t *next = current->next;
	
	current->next = prev;
	prev =current;
	current = next;
	next = current->next;
	
	if(next != NULL)
	{
		FlipListRec(node);
	}	
}

int main()
{
	node_t first, second, third, fourth;
	first.next = second;
	second.next = third;
	third.next = fourth;
	fourth.next = NULL;
	
	FlipListRec(&first);

}
