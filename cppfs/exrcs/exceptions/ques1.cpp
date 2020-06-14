#include <iostream>
#include <cstdlib>
using namespace std;

class Prints
{
public:
	Prints(int x) : m_x(x) { cout << "Ctor" << endl; }
	~Prints() { cout << "Dtor " << m_x << endl; }
	
	int m_x;
};

struct BadDog : public runtime_error
{
	BadDog(const std::string& s = "This is a bad dog")
		: runtime_error(s) { }
};

void Fifi() 
{
	Prints p(1);
	throw 1;
	throw BadDog("bad pup\n"); 
	std::cerr << "Fifi() after throw" << std::endl;
}

void Foo()
{
	Prints p(2);
	Fifi();
	std::cerr << "Foo() after Fifi()" << std::endl;
}

void Bar()
{
	Foo();
	std::cerr << "Bar() after Foo()" << std::endl;
}


int main() 
{	
	try
	{
		Bar();
	}
	catch(bad_alloc)
	{
		std::cerr << "out of memory! exiting"; 
		exit(2);
	}
	catch(BadDog& b)
	{
		std::cerr << "Bad dog excpetion: " << b.what(); 
		//exit(3);
	}
	catch(exception r)
	{
		std::cerr << "unknown exception" << r.what();
		exit(4);
	}
	//catch(...)
	//{
	//	cerr << "Some exception" << endl;
	//}

	
	std::cout << "End of prog" <<std::endl;
	
	return 0;
}
