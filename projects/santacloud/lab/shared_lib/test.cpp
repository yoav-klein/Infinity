
#include "handleton.hpp"
#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include "symbols.hpp"

//  Explicitly loading a shared library
//  twice, and dlclosing both handles.
//  this test is for the DllLoader module
// turns out that it's not a problem to dlclose more then once
// the same handle
void explicit_load()
{
	void* handle = dlopen("./libfoo.so", RTLD_LAZY);
	if(nullptr == handle)
	{
		perror("dlopen");
		exit(1);
	}
	std::cout << "first handle: " << handle << std::endl;
	
	void* handle_b = dlopen("./libfoo.so", RTLD_LAZY);
	if(nullptr == handle)
	{
		perror("dlopen");
		exit(1);
	}
	std::cout << "second handle: " << handle_b << std::endl;
	
	dlclose(handle);
	dlclose(handle_b);
	dlclose(handle_b);
}

// this function is to test what happens when you load twice the same
// library, if the ctor is called the second time or not.

// turns out that not only that the ctor is not called, but even if you changed
// the contents of foo between the different loads, it wasn't at all loaded.
void load_twice()
{
	char c = '0';
	while('q' != c)
	{	
		void* handle = dlopen("./libfoo.so", RTLD_LAZY);
		if(nullptr == handle)
		{
			perror("dlopen");
			exit(1);
		}
	
		void *sym = dlsym(handle, "foo");
		if(nullptr == sym)
		{
			perror("dlsym");
			exit(1);
		}
	
		void(*func)() = *(void(**)())&sym;
		func();
		
		dlclose(handle);
		std::cin >> c;
	}
}


void foobar()
{
	
	
	void* handle_foo = dlopen("./libfoo.so", RTLD_LAZY);
	if(nullptr == handle_foo)
	{
		perror("dlopen");
		exit(1);
	}
	
	void* handle_bar = dlopen("./libbar.so", RTLD_LAZY);
	if(nullptr == handle_bar)
	{
		perror("dlopen");
		exit(1);
	}
	
	dlclose(handle_foo);
	dlclose(handle_bar);
}

void catdog()
{
	
	void* handle_dog = dlopen("/home/student/git/projects/santacloud/lib/libdog.so", RTLD_LAZY);
	if(nullptr == handle_dog)
	{
		perror("dlopen");
		printf("%s\n", dlerror());
		exit(1);
	}
	
	void* handle_cat = dlopen("/home/student/git/projects/santacloud/lib/libcat.so", RTLD_LAZY);
	if(nullptr == handle_cat)
	{
		perror("dlopen");
		printf("%s\n", dlerror());
		exit(1);
	}

	dlclose(handle_cat);
	dlclose(handle_dog);
}

// Loading two shared libraries, both contain some std::unique_ptr<int> with the 
// same name. see what happens when you close the libraries.
void load_two_libs()
{
	catdog();
	//foobar();
}

int main()
{
	load_two_libs();
	
	std::cout << "main exit" << std::endl;

	return 0;
}

