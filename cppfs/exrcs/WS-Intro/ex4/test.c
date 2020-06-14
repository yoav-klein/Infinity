#include <stdio.h>
#include <dlfcn.h>

typedef void (*func_t)(void);

void ExplicitLink()
{
	void *handle;
	void *sym;
	func_t my_func;
	
	handle = dlopen("./libmy.so", RTLD_LAZY);
	sym = dlsym(handle, "_Z3Barv");
	
	
	*(void**)&my_func = sym;
	
	my_func();
	
}

int main()
{
	ExplicitLink();

	return 0;
}
