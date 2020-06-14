#include <iostream>
#include <cstdio>
#include <cstring>

#include "bitset.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); }


#define CHECK(x, y) if(x != y) { flag = 0; }

using namespace ilrd;

int Test()
{
	int flag = 1;
	
	// test init
	Bitset<250> first;
	Bitset<250> second;
	
	CHECK(true, (first == second));
	CHECK(false, (first != second));
	
	// test flip and flipall
	first.Flip(1);
	CHECK(false, (first == second));
	CHECK(true, (first != second));
	
	second.Flip(1);
	CHECK(true, (first == second));
	
	second.FlipAll();
	first.FlipAll();
	CHECK(true, (first == second));
	
	// test  setall
	first.SetAll(1);
	second.SetAll(1);
	CHECK(true, (first == second));
	first.SetAll(0);
	second.SetAll(0);
	CHECK(true, (first == second));
	
	// test or
	second.Flip(2);
	first |= second;
	CHECK(true, (first == second));

	// test []
	first[4] = second[2];
	CHECK(true, first[4]);
	
	second[2] = first[1];
	CHECK(false, second[2]);
	
	first[130] = first[4];
	CHECK(first[130], first[4]);
	
	// test count on
	CHECK(3, first.CountBitsOn());

	// test and
	first &= second;
	CHECK(false, first[4]);
	
	return flag;
}

int TestShift()
{
	int flag = 1;
	
	/*Bitset<250> bit;
	bit.Flip(1);
	bit.Flip(2);
	bit.Flip(4);
	bit.Flip(66);
	bit.Flip(130);
	
	 //check small shift
	CHECK((bit << 4)[5], true);
	CHECK((bit << 4)[6], true);
	CHECK((bit << 4)[134], true);
	CHECK((bit << 4)[130], false);
	
	// check big shift
	CHECK((bit << 130)[131], true);
	CHECK((bit << 130)[132], true);
	CHECK((bit << 130)[134], true);
	CHECK((bit << 130)[196], true);
	*/
	Bitset<250> bit2;
	bit2.FlipAll();
	
	std::cout << (bit2 << 244).ToString();
	
	return flag;
}


int main()
{
	//	RUN_TEST(Test);
	RUN_TEST(TestShift);
	
	return 0;
	
}
