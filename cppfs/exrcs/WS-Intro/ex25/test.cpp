#include <cstddef>
#include <cstdio>

extern int g_a;
void Foo();

namespace math
{
	long Fibo(int n) { printf("math::Fibo\n"); return 0; }
	double Sin(double d) { printf("math::Sin\n");  return 0;}
	double Cos(double d) { printf("math::Cos\n"); return 0;}
}

namespace fibonaci
{
	long lookupTable[256];
	long Fibo(size_t n) { printf("fibonaci::Fibo\n"); return 0;}
	
	namespace altversion
	{
		long Fibo(size_t n) { printf("fibonaci::altversion::Fibo\n"); return 0;}
	}
}


namespace math
{
	double Tang(double d) { printf("math::Tang\n"); return 0;}
}

void A()
{
}

	using namespace math;
	using namespace fibonaci;
	namespace alt=fibonaci::altversion;
	

void B()
{
	size_t n = 3;
	alt::Fibo(n);
	Fibo(n);
}


int main()
{
	A();
	B();
	
	g_a = 2;
	Foo();
	return 0;
}
