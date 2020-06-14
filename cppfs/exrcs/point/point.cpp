#include <iostream>
#include <stdio.h>
#include "point.h"
#include <math.h>

using namespace std;

float Point::g_total_length = 0;

void Point::UpdateLength()
{
	g_total_length -= length;
	length = sqrt((m_x* m_x) + (m_y * m_y));
	g_total_length += length;
}	

Point &Point::AdjustBy(const Point& pnt)
{
	return AdjustBy(pnt.m_x, pnt.m_y);
}

Point &Point::AdjustBy(int dx, int dy)
{
	m_x+= dx;
	m_y += dy;
	
	UpdateLength();
	
	return *this;
}

Point::~Point()
{
	m_x = 0;
	m_y = 0;
	UpdateLength();
}

Point::Point(const Point& other) : m_x(other.m_x), m_y(other.m_y)
{
	UpdateLength();
}

Point& Point::operator=(const Point& other_)
{
	m_x = other_.m_x;
	m_y = other_.m_y;
	
	UpdateLength();
		
	return *this;
}

Point::Point(int x, int y) 
: m_x(0), m_y(0), length(0) 
{
	AdjustBy(x, y);
}

bool Point::IsEqual(const Point pnt) const
{
	return ((m_x == pnt.m_x) && (m_y == pnt.m_y));
}

void Point::Print(enum paranthesis paran) const
{
	char arr[NUM_OF_PARANS * 2] = { '(', ')', '[', ']', '{', '}', '<', '>' };
	Print(arr[paran * 2], arr[paran * 2 + 1]);
}

float Point::Length() const
{	
	return length;
}

void Point::Print(char open, char close) const
{
	cout << open << m_x<< " , " << m_y << close;
}

float GetTotalLength()
{
	return Point::g_total_length;
}

Point Add(Point pnt_a, Point pnt_b)
{
	pnt_b.m_x += pnt_a.m_x;
	pnt_b.m_y += pnt_a.m_y;
	
	return pnt_b;
}

