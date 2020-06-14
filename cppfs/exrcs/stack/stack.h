#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */

struct Stack
{
	~Stack();
	explicit Stack();
	
	private:
	struct Node 
	{
		explicit Node(int data, const Node *head);
		const int m_value;
		const Node* m_next;
		int GetData() const;
		bool HasNext() const;
		const Node *GetNext() const;
	};
	const Node *m_head;
	
	public:
	void Push(int data);
	void Pop();
	int Top() const;
	bool IsEmpty()	const;
	size_t Count() const;
};


#endif
