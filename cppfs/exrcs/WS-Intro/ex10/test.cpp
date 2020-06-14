#include <iostream>
using namespace std;

int main() 
{
	enum E1 { AA, BB, CC, DD };
	enum E2 { TT, YY };
	int i;	
	E1 e = AA;
	E2 e2; 
	
	e = (E1)TT;
	
	i = e;
	
	return 0;	
}
