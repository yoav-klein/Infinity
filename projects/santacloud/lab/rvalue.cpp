#include <iostream>
#include <memory>
#include <algorithm>

class Holder
{
public:
	Holder(std::size_t size)
		: arr(new int[size]), m_size(size)
	{
		std::cout << "Ctor" << this <<  std::endl;
	}
	
	Holder(const Holder& other)
	{
		std::cout << "CCtor" << std::endl;
		m_size = other.m_size;
		arr = new int[m_size];
		std::copy(other.arr, other.arr + m_size, arr);
		
	}
	Holder(Holder&& other)
	{
		std::cout << "MCtor" << this << std::endl;
		m_size = other.m_size;
		arr = other.arr;
		other.m_size = 0;
		other.arr = nullptr;
	}
	~Holder()
	{
		std::cout << "Dtor" << this << std::endl;
		delete[] arr;
	}

	void print()
	{
		for(std::size_t i = 0; i < m_size; ++i)
		{
			std::cout << *(arr + i) << " " << std::endl;
		}
	}
private:

	int* arr;
	std::size_t m_size;

};


Holder foo()
{
	Holder h(20);
	
	//h.print();
	return h;
}

void bar(Holder&& h)
{
	h.print();
}

void fifi(std::unique_ptr<int> uptr)
{

}

void unique_ptr()
{
	std::unique_ptr<int> uptr(new int(3));
	std::unique_ptr<int>&& rv = std::move(uptr);
	//std::unique_ptr<int> uptr2 = rv;
	//fifi(std::move(uptr));
	
	std::cout << *uptr;
}

void primitives()
{
	int a = 3;
	int b = std::move(a);
	
	std::cout << b;
	std::cout << a;
}

void holder()
{
	Holder h(10);
	//Holder h1 = std::move(h);
	
	std::cout << &h << std::endl;
	
	
	//h.print();
	//h1.print();
}

int main()
{
	holder();
		
	return 0;
}
