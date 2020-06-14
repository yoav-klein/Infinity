#include <iostream>

typedef struct List
{
	int num;
} List;

typedef struct Person
{
	int age;
} Person;

void *ll_find(List* ll, int key)
{
	return NULL;
}

void Foo(List* ll, int key, int age)
{
	void* vv = ll_find(ll, key);
	Person* p = static_cast<Person*>(vv);
	p->age;
}

int main()
{
	void *ptr = new Person;
	Person* p = static_cast<Person*>(ptr);		
	
	return 0;
}
