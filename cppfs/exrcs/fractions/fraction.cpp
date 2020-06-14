#include <iostream>
#include "fraction.h"
#include <algorithm>
#include <cmath>

namespace ilrd
{

const int Fraction::ZERO = 0;
const float Fraction::HALF = 0.5;
const int Fraction::ONE = 1;
size_t Fraction::counter = 0;

std::ostream& operator<<(std::ostream& os, const Fraction& fraction)
{
	os << fraction.m_num << " / " << fraction.m_den;
	
	return os;
}

size_t Fraction::CountFractions()
{
	return counter;
}

Fraction::Fraction(int num, int den) 
	: m_num(num), m_den(den)
{
	++counter;
}

Fraction::Fraction(const Fraction& other)
	: m_num(other.m_num), m_den(other.m_den)
{
	++counter;
}
Fraction::~Fraction()
{
	--counter;
}

Fraction::operator int() const
{
	return (m_num / m_den); 
}

Fraction::operator float() const
{
	return ((float)m_num / m_den);
}

Fraction& Fraction::operator=(int num)
{
	*this = Fraction(num);
	
	return *this;
}

Fraction& Fraction::operator=(const Fraction& other)
{
	m_num = other.m_num;
	m_den = other.m_den;
	
	return *this;
}


Fraction Fraction::operator++(int) // x++
{
	Fraction temp = *this;
	
	AddToSelf(1);
	
	return temp;
}

Fraction& Fraction::operator++() // ++x
{
	return AddToSelf(1);
}
  
Fraction Fraction::operator--(int)
{
	Fraction temp = *this;
	
	AddToSelf(-1);
	
	return temp;
}

Fraction& Fraction::operator--()
{
	return AddToSelf(-1);
}

Fraction Fraction::operator+(const Fraction& f1) const
{
	Fraction f = *this;
	f.AddToSelf(f1);
	
	return f;
}

Fraction Fraction::operator-(const Fraction& f1) const
{
	Fraction f = *this;
	f.AddToSelf(-f1);
	
	return f;
}

Fraction Fraction::operator+(int num) const
{
	return Fraction(num) + *this;
}

Fraction Fraction::operator-(int num) const
{
	return *this - Fraction(num);
}

Fraction operator+(int num, const Fraction& f)
{
	return f + num;
}

Fraction operator-(int num, const Fraction& f)
{
	return -(f - num);
}

int Fraction::GCD(int a, int b) const
{
	return b ? GCD(b, a % b) : a;  
}

Fraction Fraction::Minimize() const
{
	int gcd = abs(GCD(m_num, m_den));
	
	if(0 == m_num || 0 == m_den) 
	{
		return *this;
	}
	
	return Fraction(m_num / gcd, m_den / gcd);
}

void Fraction::Minimize()
{
	*this = static_cast<const Fraction>(*this).Minimize();
}

bool Fraction::IsUndefined() const
{
	return (m_den == 0);
}

Fraction& Fraction::AddToSelf(const Fraction& other)
{
	Fraction f = other.Minimize();
	Minimize();
	
	m_num *= f.m_den;
	f.m_num *= m_den;
	
	m_den *= f.m_den;
	m_num += f.m_num;
	
	Minimize();
	
	return *this;
}

Fraction& Fraction::AddToSelf(int num)
{
	return AddToSelf(Fraction(num));
}

void Fraction::Print() const
{
	std::cout << "(" << m_num << " / " << m_den << ")" << std::endl; 
}

bool Fraction::IsEqual(const Fraction& other) const
{
	return (other.Minimize().m_num == Minimize().m_num &&
			other.Minimize().m_den == Minimize().m_den);
}

bool Fraction::IsEqual(int num) const
{
	return IsEqual(Fraction(num));
}

void Fraction::SetNumerator(int num)
{
	m_num = num;
}

void Fraction::SetDenomenator(int num)
{
	m_den = num;
}

int Fraction::GetNumerator() const
{
	return m_num;
}

int Fraction::GetDenomenator() const 
{
	return m_den;
}

} // namespace ilrd

