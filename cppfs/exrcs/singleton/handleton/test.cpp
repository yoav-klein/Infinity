#include <iostream> // std::cout
#include <dlfcn.h> // open
#include <thread> // std::thread
#include <mutex> // std::mutex
#include <condition_variable> // std::condition_variable

#include "handleton.hpp"
#include "dog.hpp"
#include "holder.hpp"

int g_counter = 0;

extern "C"
Dog* foo();

using namespace ilrd;

const std::size_t NUM_THREADS = 20;

/*void implicit()
{
    Dog* dog = Singleton<Dog>::get_instance();
    int* num = Singleton<int>::get_instance();
	//std::cout << dog << std::endl;
	
	//Dog* libdog = foo();
	//std::cout << libdog << std::endl;
}*/

/*void expl()
{
	
	
	std::cout << "calling get_instance from main: " << std::endl;
	Dog* local = Handleton<Dog>::get_instance();
	std::cout << "Dog* from main: " << local << std::endl;
	
	std::cout << "calling foo() " << std::endl;
	Dog* ret = ((Dog* (*)(void))func)();
	
	std::cout << "returned from foo: " << ret << std::endl;
}*/

void* open_handle()
{
	void *handle = dlopen("./libdog.so", RTLD_LAZY);
	std::cout << "handle: " << handle << std::endl;
	void *func = dlsym(handle, "foo");
	
	return func;
}

void print_instance_address(std::mutex* mutex, std::condition_variable* cv,
							Dog*(*func)(void))
{	
	bool flag = false;
	Dog* dog;
	{
		std::unique_lock<std::mutex> lock(*mutex);
		++g_counter;
		if(g_counter % 2 == 0)
		{
			flag = true;
		}
		while(g_counter < NUM_THREADS - 1)
		{
			std::cout << "waiting..." << std::endl;
			(*cv).wait(lock);
		}
		(*cv).notify_all();
	}
	
	if(true == flag)
	{
		dog = Handleton<Dog>::get_instance();
	}
	else
	{
		dog = func();
	}
	mutex->lock();
	std::cout << "thread id: " << std::this_thread::get_id();
	std::cout << " dog*: " << dog << std::endl;
	mutex->unlock();
}

void threads_test(Dog*(*func)(void))
{
	std::mutex mutex;
	std::condition_variable cv;
	
	std::thread threads[NUM_THREADS];

	std::size_t i = 0;
	for(i = 0; i < NUM_THREADS; ++i)
	{
		threads[i] = std::thread(print_instance_address, &mutex, &cv, func);
	}	
	
	for(i = 0; i < NUM_THREADS; ++i)
	{
		threads[i].join();
	}

}

// checking if Handleton can receive a template class as a parameter, 
// and if i can use class template that receives more then one parameter.
// it's possible using a macro to define the type
// #define TYPE Holder<float, int> 
// and then instnatiating with InitHandleton(TYPE);

void templ_type()
{
	Holder<float, int>* holder = Handleton<Holder<float, int> >::get_instance();
	
	holder->print();
}

int main()
{	
	void* function = open_handle();
	//threads_test((Dog*(*)(void))function);
	//expl();
	//implicit();
	
	templ_type();
	
	return 0;
}
