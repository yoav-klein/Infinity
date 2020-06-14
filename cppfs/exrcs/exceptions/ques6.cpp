#include <iostream>
#include <typeinfo>
#include <exception>


struct NoThrow
{
	NoThrow() { std::cerr << "NoThrow Ctor" << std::endl; }
	//NoThrow(int) { std::cerr << "NoThrow Ctor int" << std::endl; }
	~NoThrow() { std::cerr << "NoThrow Dtor" << std::endl; }
};

struct ThrowAtDefCtor
{
	ThrowAtDefCtor()
	{
		std::cerr << "ThrowAtDefCtor Ctor" << std::endl;
		throw std::bad_cast();
	}
	
	ThrowAtDefCtor(int) { std::cerr << "ThrowAtDefCtor Ctor(int)" << std::endl; }
	~ThrowAtDefCtor() { std::cerr << "ThrowAtDefCtor Dtor" << std::endl; }
};


class X
{
public:
	X() : m_dtc(2) { std::cerr << "X Ctor" << std::endl; }
	~X() { std::cerr << "X Dtor" << std::endl; }
private:
	ThrowAtDefCtor m_dtc;
	NoThrow m_nt;
};

class Y
{
public:
	Y() { std::cerr << "Y Ctor" << std::cerr; }
	~Y() { std::cerr << "Y Dtor" << std::endl; }
private:
	ThrowAtDefCtor m_dtc;
	NoThrow m_nt;
};

class Z
{
public:
	Z() { std::cerr << "Z Ctor" << std::cerr; }
	~Z() { std::cerr << "Z Dtor" << std::endl; }
private:
	NoThrow m_nt;
	ThrowAtDefCtor m_dtc;
};

class YPtrs
{
public:
	YPtrs() : m_dtc(new ThrowAtDefCtor), m_nt(new NoThrow)
	{
		std::cerr << "YPtrsCtor" << std::endl;
	}
	
	~YPtrs()
	{
		std::cerr << "YPtrs Dtor" << std::endl;
		delete m_dtc;
		delete m_nt;
	}
	
private:
	ThrowAtDefCtor *m_dtc;
	NoThrow *m_nt;
};

class ZPtrs
{
public:
	ZPtrs()
	{
		std::cerr << "ZPtrs Ctor" << std::endl;
		m_nt = new NoThrow;
		m_dtc = new ThrowAtDefCtor;
	}
	~ZPtrs()
	{
		std::cerr << "ZPtrs Dtor" << std::endl;
		delete m_dtc;
		delete m_nt;
	}

private:
	NoThrow *m_nt;
	ThrowAtDefCtor *m_dtc;
};

int main()
{
	try
	{
		//X var1;
		//Z var2;
		//Y y1;
		//YPtrs yptr1;
		
		ZPtrs var1;
	}
	catch(std::exception &e)
	{
		std::cerr << "excecption cout. what:" << e.what() << std::endl;
	}

}







