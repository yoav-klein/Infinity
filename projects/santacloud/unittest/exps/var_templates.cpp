
#include <iostream>

int count = 0;

void Test(int a, int b, int c)
{
	std::cout << a << b << c << std::endl;
}

template<typename T, typename ... Types>
void print(T var1, Types... var2)
{
	Test(var1, var2...);
	
}

int main()
{
	print(1, 3, 5);

}
