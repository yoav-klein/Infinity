#ifndef __FRACTION_H__
#define __FRACTION_H__

#include <cstddef> // size_t 


namespace ilrd
{
	struct Fraction;
	std::ostream& operator<<(std::ostream& os, const Fraction& fraction);
	
	struct Fraction
	{
	public:
		static const int ZERO;
		static const float HALF;
		static const int ONE;
		
		static size_t CountFractions();
		explicit Fraction(int num = 0, int den = 1);
		Fraction(const Fraction& other);
		
		~Fraction();
		Fraction& operator=(const Fraction& other);
		Fraction& operator=(int num);
		Fraction operator++(int); // x++
		Fraction& operator++();  // ++x
		Fraction operator--(int);
		Fraction& operator--();
		Fraction operator+(const Fraction& other) const;
		Fraction operator-(const Fraction& other) const;
		Fraction operator-() const;
		Fraction operator+() const;
		Fraction operator+(int num) const;
		Fraction operator-(int num) const;
		operator int() const;
		operator float() const;
		
		bool IsUndefined() const;
		Fraction& AddToSelf(const Fraction& other);
		Fraction& AddToSelf(int num);
		void Print() const;
		bool IsEqual(const Fraction& other) const;
		bool IsEqual(int other) const;
		void SetNumerator(int num);
		void SetDenomenator(int num);
		int GetNumerator() const;
		int GetDenomenator() const; 
	
	private:
		int m_num;
		int m_den;
		int GCD(int a, int b) const;
		Fraction Minimize() const;
		void Minimize();
		static size_t counter;
		friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction);
	
	};
	
	Fraction operator+(int num, const Fraction& f);
	Fraction operator-(int num, const Fraction& f);
	
	inline Fraction Fraction::operator+() const
	{
		return *this;
	}
	
	inline Fraction Fraction::operator-() const
	{
		return Fraction(-m_num, m_den);
	}
	
}


#endif
