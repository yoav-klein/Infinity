#include <iostream>


template<typename T, typename Q>
class Holder
{
public:
	Holder()
		: m_data_t(2), m_data_q(4) { }
	
	void print()
	{
		std::cout << m_data_t << " " << m_data_q << std::endl;
	}

private:
	T m_data_t;
	Q m_data_q;
};
