#include <iostream>

using namespace std;

int g_a;

struct Counter
{
	Counter *IncPointer();
	Counter &IncRef();
	void Dec();
	
	int m_a;
	int m_b;
};

Counter &Counter::IncRef()
{
	++m_a;
	m_b += 2;
	
	return *this;
}

Counter *Counter::IncPointer()
{
	++m_a;
	m_b += 2;
	
	return this;
}

void Counter::Dec()
{
	--m_a;
	m_b /= 2;
}


int main()
{
	struct Counter x;
	
	x.m_a = 1;
	x.m_b = 2;
	
	x.IncPointer()->IncPointer()->IncPointer();
	x.IncRef().IncRef();
	
	cout << x.m_a << endl;
	
	return 0;
}


