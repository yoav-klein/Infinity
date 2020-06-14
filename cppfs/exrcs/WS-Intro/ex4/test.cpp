#include <stdio.h>
#include <iostream>
#include <dlfcn.h>

#define RUN_TEST(x) if(1 == x()) { cout << "TEST OK!\n"; } \
							else { cout << "TEST FAILED!\n";  } 
								
using namespace std;

void Foo();
void Bar();

typedef void (*func_t)(void);

extern "C"
{
	void CFoo();
	void CBar();
}

void ExplicitLinkToCLib()
{
	void *handle;
	void *sym;
	func_t my_func;
	
	handle = dlopen("./libmyc.so", RTLD_LAZY);
	sym = dlsym(handle, "CFoo");
	
	cout << handle << endl;
	cout << sym << endl;
	
	*(void**)&my_func = sym;
	
	my_func();
}

/*
void ImplicitLinkToCLib()
{
	CFoo();
	CBar();
}*/

void ExplicitLink()
{
	void *handle;
	void *sym;
	void (*my_func)(void);
	
	handle = dlopen("./libmy.so", RTLD_LAZY);
	sym = dlsym(handle, "Foo");
	
	cout << handle << endl;
	cout << sym << endl;
	
	my_func = *(void(**)(void))&sym;
	
	my_func();
	
}

/*
void ImplicitLink()
{
	Foo();
	Bar();
}*/

int main()
{
	ExplicitLinkToCLib();
	
	// ImplicitLink();
	// ImplicitLinkToCLib();
	
	return 0;
}
