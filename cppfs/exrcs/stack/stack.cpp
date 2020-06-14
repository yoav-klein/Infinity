#include "stack.h"


Stack::Stack() : m_head(NULL)
{
	
}

Stack::Node::Node(int data, const Node *head) : m_value(data), m_next(head)
{
	
}

Stack::~Stack()
{
	const Node *temp = NULL;
	const Node *runner = m_head;
	while(NULL != runner)
	{
		temp = runner->GetNext();
		delete runner;
		runner = temp;
	}
}

void Stack::Pop()
{
	const Node *temp = m_head;
	m_head = m_head->GetNext();
	delete temp;
}

void Stack::Push(int data)
{
	Node *node = new Node(data, m_head);
	m_head = node;
}

int Stack::Top() const
{
	return m_head->GetData();
}

bool Stack::IsEmpty() const
{
	return (m_head == NULL);
}

size_t Stack::Count() const
{
	const Node *runner = m_head;
	size_t count = 0;
	while(NULL != runner)
	{
		++count;
		runner = runner->GetNext();
	}
	
	return count;
}

int Stack::Node::GetData() const
{
	return m_value;
}

bool Stack::Node::HasNext() const
{
	return (NULL != m_next);
}

const Stack::Node* Stack::Node::GetNext() const
{
	return m_next;
}


