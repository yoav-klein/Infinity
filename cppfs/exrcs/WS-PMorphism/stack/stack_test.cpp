#include <cstring>
#include <iostream>
#include "stack.h"
#include "stack.tpp"
#include "string.h"

#define RUN_TEST(x) if(1 == x()) { std::cout << "TEST OK!\n"; } \
							else { std::cout << "TEST FAILED!\n";  } 

#define CHECK(x, ex) if(x != ex) { flag = 0;  }

int TestStackBasic()
{	
	int flag = 1;
	
	Stack<int> stack;
	
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

int TestStackDouble()
{
	int flag = 1;
	Stack<double> stack;
	
	
	// Check IsEmpty and Count
	CHECK(1, stack.IsEmpty())
	
	CHECK(0, stack.Count())
	
	// Push a few numbers, check IsEmpty and Count
	stack.Push(5.8);
	
	CHECK(0, stack.IsEmpty())
	CHECK(1, stack.Count())
	
	CHECK(5.8, stack.Top())
	
	std::cout<<stack.Top() << std::endl;
	
	stack.Push(10.1);
	
	std::cout<<stack.Top() << std::endl;
	CHECK(10.1, stack.Top())
	CHECK(2, stack.Count())
	CHECK(0, stack.IsEmpty())
	
	return flag;
}

int TestString()
{
	int flag = 1;
	
	String s1 = "Jerusalem";
	String s2 = "Shalom";
	String s3 = "Tamar";
	
	Stack<String> stack;
	// Check IsEmpty and Count
	CHECK(1, stack.IsEmpty())
	CHECK(0, stack.Count())
	
	stack.Push(s1);
	stack.Push(s2);
	stack.Push(s3);
	
	s3 = "Tamari";
	// notice that now s3 contains "Tamari" but the 
	// string inside the stack stays "Tamar"
	CHECK(3, stack.Count());
	
	CHECK(0, strcmp(stack.Top().ToCStr(), "Tamar"));
	stack.Pop();
	
	CHECK(0, strcmp(stack.Top().ToCStr(), "Shalom"));
	stack.Pop();
	
	CHECK(0, strcmp(stack.Top().ToCStr(), "Jerusalem"));
	
	return flag;
}

int main()
{
	//RUN_TEST(TestStackBasic);
	//RUN_TEST(TestStackDouble);
	RUN_TEST(TestString);
	
	return 0;
}
