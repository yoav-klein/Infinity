#include <cstdio>
#include <iostream>
#include "fraction.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, y) if(x != y) { flag = 0; }

using namespace ilrd;

int TestFraction()
{
	int flag = 1;
	
	Fraction f;
	Fraction f2(3, 4);
	
	// test Getvals;
	
	CHECK(f.GetNumerator(), 0);
	CHECK(f.GetDenomenator(), 1);
	
	//check add_to_self with int
	f.AddToSelf(2);
	f.Print();
	CHECK(f.GetNumerator(), 2)
	CHECK(f.GetDenomenator(), 1)
	
	//check add_to_self with another fraction
	f.AddToSelf(f2);
	f.Print();
	CHECK(f.GetNumerator(), 11);
	CHECK(f.GetDenomenator(), 4);
	
	//add 1 to numerator, check
	f.AddToSelf(Fraction(1, 4));
	f.Print();
	
	//check IsEqual
	f2.SetNumerator(9);
	f2.SetDenomenator(3);
	f.Print();
	f2.Print();
	
	//check Undefined
	Fraction f3(1, 0);
	CHECK(true, f3.IsUndefined())
	f3.AddToSelf(f2);
	CHECK(true, f3.IsUndefined())
	f2.AddToSelf(f3);
	CHECK(true, f2.IsUndefined())
	
	return flag;
}

int TestOperators()
{
	int flag = 1;
	
	Fraction f1(1, 2);
	Fraction f2(3, 4);
	Fraction f3 = f1;
	Fraction f4 = f2;
	
	Fraction res;
	
	res = +f1 + -(++f2);
	res.Print();
	CHECK(-5, res.GetNumerator());
	CHECK(4, res.GetDenomenator());
	
	res = (f3++) -+(f4--);
	CHECK(-1, res.GetNumerator());
	CHECK(4, res.GetDenomenator());
	res.Print();
	
	return flag;
}

int TestConsts()
{
	int flag = 1;
	
	const Fraction f1(1, 4);
	const Fraction f2(3, 4);
	Fraction res;
	
	res = f1 + f2;
	CHECK(1, res.GetNumerator());
	CHECK(1, res.GetDenomenator());
	CHECK(false, f1.IsEqual(f2));

	return flag;
}

int TestCountInstances()
{
	int flag = 1;
	CHECK(0, Fraction::CountFractions());
	
	Fraction f1;
	CHECK(1, Fraction::CountFractions());
	std::cout << f1;
	
	Fraction f2;
	CHECK(2, Fraction::CountFractions());
	std::cout << f2;
	
	Fraction f3;
	CHECK(3, Fraction::CountFractions());
	std::cout << f3;
	return flag;
}

int TestConstants()
{
	int flag = 1;
	CHECK(Fraction::ZERO, 0);
	CHECK(Fraction::ONE, 1);
	CHECK(Fraction::HALF, 0.5);

	return flag;
}

int TestConversions()
{
	int flag = 1;
	
	Fraction f1(4, 1);
	int res = f1;
	CHECK(res, 4);
	
	Fraction f2(10, 8);
	res = f2;
	CHECK(res, 1);
	
	float f;
	f = f2;
	CHECK(f, 1.25);
	
	f1 = 5;
	CHECK(1, f1.IsEqual(5));
	
	Fraction r1(1, 2);
	Fraction r2(3);
	Fraction f3 = r1 + r2;
	
	r1 = r1 + 3;
	r2 = 3 + r2;
	CHECK(r1.IsEqual(Fraction(7, 2)), 1);
	CHECK(r2.IsEqual(Fraction(6, 1)), 1);
	
	Fraction r3(1, 2);
	Fraction r4(3);
	Fraction r_res;
	
	r_res = r3 - r4;
	CHECK(r_res.IsEqual(Fraction(-5, 2)), 1);
	
	r_res = r3 - 3;
	CHECK(r_res.IsEqual(Fraction(-5, 2)), 1);
	
	r_res = 3 - r3;
	CHECK(r_res.IsEqual(Fraction(5, 2)), 1);
	
	return flag;
}

int main()
{
	RUN_TEST(TestFraction);
	RUN_TEST(TestOperators);
	RUN_TEST(TestConsts);
	RUN_TEST(TestCountInstances);
	RUN_TEST(TestConstants);
	RUN_TEST(TestConversions);
	
	
	return 0;
}
