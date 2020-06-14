#include <cstdio> // printf
#include "shared_ptr.h"
using namespace ilrd;

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, exp) if((x) != (exp)) { flag = 0;  }

class X
{
public:
	X(int i = 0) : m_a(i) { }
	
	int Get()
	{
		return m_a;
	}
	
private:
	int m_a;
};

class Base
{
public:
	int m_a;
};

class Derived : public Base
{
public:
	int m_b;
};

SharedPtr<X> Bar()
{
	SharedPtr<X> xptr(new X(24));
	
	return xptr;
}

int BasicTest()
{
	int flag = 1;
	
	SharedPtr<int> sp_int1(new int(5));
	SharedPtr<int> sp_int2(new int(10));
	SharedPtr<int> sp_int3(sp_int2); // copy ctor
	
	// check copy ctor
	CHECK(10, *sp_int3);
	
	SharedPtr<X> xptr1(new X(8));
	
	//check * operator
	CHECK(8, (*xptr1).Get())
	
	*sp_int1 = 30;
	CHECK(30, *sp_int1);
	
	//check assignment operator
	sp_int2 = sp_int1;
	CHECK(*sp_int2, *sp_int1);
	
	// check creation in another function
	SharedPtr<X> xptr2 = Bar(); 
	CHECK((*xptr2).Get(), 24);
	
	// check -> operator
	CHECK(24, xptr2->Get());
	
	// check default ctor and ! operator
	SharedPtr<int> empty;
	CHECK(!empty, true);
	
	return flag;
}

int Inheritance()
{	
	int flag = 1;
	
	SharedPtr<Derived> dp(new Derived());
	SharedPtr<Base> bp(new Base());
	SharedPtr<Base> bp2(dp);
	bp->m_a = 10;
	dp->m_a = 20;
	
	bp = dp; // assign derived to base pointer
	
	//dp = bp;  // invalid conversion, uncomment and it won't compile
	
	CHECK(bp2->m_a, dp->m_a)
	
	return flag;
}

int TestConst()
{
	int flag = 1;
	

	return flag;
}

int main() 
{
	RUN_TEST(BasicTest);
	RUN_TEST(Inheritance);	
			
	return 0;
}
