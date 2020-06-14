#include <vector>
#include <iostream>
#include <list>
#include <tr1/memory>
#include <memory>
class Ref
{
public:
	Ref(int a, int b) : m_a(a), m_b(b) { }
	
	int m_a;
	int m_b;
	
	static bool ToRemove(const Ref& ref)
{
	return ref.m_b == 4;
}	
};



class Interface
{
public:
	Interface(int a)
		: m_a(a) { }
	
	virtual void GetMem() = 0;
	
protected:
	int m_a;
};

class Regular : public Interface
{
public:
	Regular(int a)
		: Interface(a) { }
	void GetMem()
	{
		std::cout << "From regular" << m_a << std::endl;
	}
};

class Special : public Interface
{
public:
	Special(int a)
		: Interface(a) { }
	void GetMem()
	{
		std::cout << "From special" << m_a << std::endl;
	}
};

class Recursive
{
public:
	Recursive() 
		: m_a(1) { }
	
	void Rec(int n)
	{
		if(n < 100)
		{
			++n;
			std::cout << "Calling rec: n:" << n << std::endl;
			Rec(n);
		}
	}
	
private:
	int m_a;

};

enum Color { WHITE } ;

struct IsSame
{
	IsSame(Ref r)
		: m_r(r) { }
	bool operator()(const Ref& ref)
	{
		std::cout << "address if predicator" << " " << &ref.m_a << std::endl;
		return ((ref.m_b == m_r.m_b) && (ref.m_a == m_r.m_a));
	}
	
	Ref m_r;
};

void RemoveIf()
{
	Ref ref(4, 5);
	
	std::list<Ref> list;
	list.push_back(Ref(10, 12));
	list.push_back(Ref(1, 11));
	list.push_back(Ref(50, 30));
	list.push_back(Ref(12, 11));
	
	list.remove_if(IsSame(Ref(10, 11)));
	
	std::list<Ref>::iterator iter = list.begin();
	
	for(iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << "address in main: " << &iter->m_b << std::endl;
		std::cout << iter->m_a << " " << iter->m_b << std::endl;
	}

}

void Addresses()
{
	std::list<int> list;
	
	int a = 4;
	std::cout << &a << std::endl;
	
	list.push_back(a);
	list.push_back(3);
	list.push_back(5);
	list.push_back(9);
	
	std::list<int>::iterator iter = list.begin();
	
	for(iter = list.begin(); iter != list.end(); ++iter)
	{
		std::cout << &*iter << std::endl;
	}
	
	
}

class Bar
{
public:
	Bar(int& a, int b) : m_a(a), m_b(b)
	{ 
		std::cout << "Ctor" << std::endl;	
	}
	
	Bar(const Bar& other)
		: m_a(other.m_a), m_b(other.m_b)
	{
		std::cout << "CCtor" << std::endl;
	}
	
	int& m_a;
	int m_b;
};

void Foo(Bar* b)
{
	std::cout << "Foo: " << &b->m_a << std::endl;
	std::cout << "Foo: " << reinterpret_cast<void*>(b) << std::endl;
}

void List()
{
	int i = 3;
	int i1 = 4;
	Bar bar(i, i1);
	std::list<Bar> list;
	
	
	std::cout << "Empty" << list.empty();
	list.push_back(bar);
	
	std::cout << "Empty" << list.empty();
}

class HoldsInt
{
public:
	HoldsInt() 
		: m_holder(new int(2))
	{
	
	}
	
	void Print()
	{
		std::cout << *m_holder << std::endl;
	}
	
	void Switch()
	{
		m_holder.reset(new int(11));
	}
	
	
	
private:
	std::tr1::shared_ptr<int> m_holder;
};

void SmartPointer()
{
	HoldsInt i;
	i.Print();
	i.Switch();
	i.Print();
	//std::shared_ptr<int> sp_11(new int(22));
	
}

std::vector<std::tr1::shared_ptr<int> > Producer()
{
	std::vector<std::tr1::shared_ptr<int> > vec;
	
	vec.push_back(std::tr1::shared_ptr<int>(new int(11)));
	

	
	return vec;
}

void PrintInt(std::tr1::shared_)
{

}

void DoubleSharedPointer()
{
	
	

}

int main()
{

	//SmartPointer();
	//RemoveIf();
	
	//ArrayOfSmartPointers();
	
	//Addresses();
	//List();
	
	DoubleSharedPointer();
	/*
	std::vector<int> vec;
	
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(5);
	vec.push_back(9);
	
	for(int i = 0; i < vec.size(); ++i)
	{
		std::cout << vec[i] << " " << &vec[i] << std::endl;
	}
	
	vec.erase(vec.begin());
	
	for(int i = 0; i < vec.size(); ++i)
	{
		std::cout << vec[i] << " " << &vec[i] << std::endl;
	}
	
	//std::vector<int>& ref = vec;
	//vec.erase(std::vector<int>::iterator(&vec[2]));
	//std::vector<Ref>& ref = vec;
*/
	
}
