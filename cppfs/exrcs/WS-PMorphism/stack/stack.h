#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */

template <typename T>
class Stack
{
public:
	~Stack();
	explicit Stack();
	void Push(const T& data);
	void Pop();
	T Top() const;
	bool IsEmpty()	const;
	size_t Count() const;
	
private:
	class Node 
	{
		public:
		explicit Node(T t1, const Node *head);
		const T m_value;
		const Node* m_next;
		T GetData() const;
		bool HasNext() const;
		const Node *GetNext() const;
	};
	const Node *m_head;
};


#endif
