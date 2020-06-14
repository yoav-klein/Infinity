#include "polygon.h"
#include <iostream>

#define RUN_TEST(x) if(1 == x()) { std::cout << "TEST OK!\n"; } \
							else { std::cout << "TEST FAILED!\n";  }


int TestPolygonBasic()
{
	int flag = 1;
	Polygon poly(5);
	Polygon poly2(5);
	Polygon poly3(5);
	
	poly.Add(3, 4);
	poly.Add(2, 1);
	poly.Add(4, 5);
	poly.Add(1, 2);
	
	poly2.Add(3, 4);
	poly2.Add(2, 1);
	poly2.Add(4, 5);
	poly2.Add(1, 2);
	
	poly3.Add(3, 4);
	poly3.Add(2, 1);
	poly3.Add(4, 5);
	poly3.Add(2, 2);
	
	if(false == poly.IsEqual(poly2))
	{
		flag = 0;
	}
	
	if(true == poly.IsEqual(poly3))
	{
		flag = 0;
	}
	
	return flag;
}

int main()
{
	RUN_TEST(TestPolygonBasic);	
		
	return 0;
}
