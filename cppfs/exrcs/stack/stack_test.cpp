
#include <iostream>
#include "stack.h"


#define RUN_TEST(x) if(1 == x()) { std::cout << "TEST OK!\n"; } \
							else { std::cout << "TEST FAILED!\n";  } 

#define CHECK(x, ex) if(x != ex) { flag = 0;  }

int TestStackBasic()
{	
	int flag = 1;

	Stack stack;

	// Check IsEmpty and Count
	CHECK(1, stack.IsEmpty())
	
	CHECK(0, stack.Count())
	
	// Push a few numbers, check IsEmpty and Count
	stack.Push(5);
	
	CHECK(0, stack.IsEmpty())
	
	CHECK(1, stack.Count())
	
	// check Top
	CHECK(5, stack.Top())
	
	stack.Push(10);
	CHECK(10, stack.Top())
	CHECK(2, stack.Count())
	CHECK(0, stack.IsEmpty())
	
	stack.Pop();
	
	CHECK(1, stack.Count())
	CHECK(5, stack.Top())
	
	stack.Pop();
	
	CHECK(0, stack.Count())
	CHECK(1, stack.IsEmpty())
	
	stack.Push(43);
	stack.Push(1);
	
	return flag;
}

int main()
{
	RUN_TEST(TestStackBasic);	
	
	return 0;
}
