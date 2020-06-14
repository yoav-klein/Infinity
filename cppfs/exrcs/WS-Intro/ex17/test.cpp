#include <iostream>

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
	float *f = new float(12.6);
	delete f;
	
	f = new float[15];
	delete[] f;
	
	
	
	
	return 0;   
}
