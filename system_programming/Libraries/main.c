#include <dlfcn.h>
#include <stdio.h>

void Foo();

void Bar1();

typedef void (*func)(void);

int main()
{	
	
	void *handle = NULL;
	void *func_void = NULL;
	
	Foo();
	
	Bar1();
	
	handle = dlopen("libso2.so", RTLD_LAZY);
	func_void = dlsym(handle, "Bar2");
	
	(*(func*)&func_void)();
	
		
	return 0;
}
