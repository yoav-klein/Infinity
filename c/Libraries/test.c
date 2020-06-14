#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

typedef void (*func_ptr)(void);
int g_a = 5;

void Foo();

void Bla()
{
	printf("Bla from main %d\n", g_a);
}

void explicit()
{
	func_ptr which_func;
	
	void *handle = dlopen("./libbla.so", RTLD_LAZY | RTLD_DEEPBIND);
	void *func;		
	
	if(NULL == handle)
	{
		printf("%s\n", dlerror());
		exit(1);
	}
	
	func =  dlsym(handle, "Bla");
	printf("%p\n", handle);
	
	g_a = 10;
	*(void**)&which_func = func;
	
	
	printf("%p\n", which_func);
	which_func();
	
	Bla();
	
	printf("from main: %p\n", g_a);
	
}

void implicit()
{
	/*Foo();*/
	
}

int main()
{
	explicit();
	return 0;
}
