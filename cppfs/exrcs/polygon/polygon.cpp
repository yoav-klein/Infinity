#include <iostream>
#include "polygon.h"

Polygon::Polygon(size_t num_of_points) : m_index(0), m_max_points(num_of_points)
{
	m_points = new Point[num_of_points];
}

int Polygon::Add(int x, int y)
{
	if(m_index <= m_max_points)
	{
		Point pnt(x, y);
		m_points[m_index] = pnt;
		++m_index;
		
		return 0;
	}
	return 1;
}

Polygon::~Polygon()
{
	delete[] m_points;
}

void Polygon::Print()
{
	size_t i = 0;
	for(i = 0; i < m_index; ++i)
	{
		m_points[i].Print();
		std::cout << " --> ";
	}
}

bool Polygon::IsEqual(const Polygon& poly)
{
	if(poly.m_index != m_index)
	{
		return false;
	}
	
	size_t i = 0;	
	for(i = 0; i < m_index; ++i)
	{
		if(!m_points[i].IsEqual(poly.m_points[i]))
		{
			return false;
		}		
	}
	
	return true;
}
