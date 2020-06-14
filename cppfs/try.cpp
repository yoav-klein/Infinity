
#include <iostream>

#include <cstdio>
#include <iostream>

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, y) if(x != y) { flag = 0; }


class Base
{
public:
	virtual void Foo();
};

void Base::Foo()
{
	std::cout << "Base Foo" << std::endl;
}

class Derived : public Base
{
public:
	virtual void Foo();
};

void Derived::Foo()
{
	std::cout << "Derived Foo" << std::endl;
}

int Test()
{
	int flag = 1;	
	
	
}		

int main()
{
	Base* b = new Derived();
	return 0;
}
