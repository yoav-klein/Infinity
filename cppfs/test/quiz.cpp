#include <iostream>

class Integer
{
public:
	Integer(int i)
		: m_i(i)
	{
	}
	
	const Integer& operator++(int)
	{
		Integer temp(*this);
		++m_i;
		return temp;
	}

	int m_i;
};

int main()
{
	Integer i(10);
	
	Integer b(2);
	b = i++;
	
	std::cout << b.m_i;
	return 0;
}
