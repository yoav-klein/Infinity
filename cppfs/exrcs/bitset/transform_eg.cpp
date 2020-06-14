#include <iostream>
#include <exception>

#include <iterator>
#include <vector>
#include <algorithm>

void Print(int current)
{
	std::cout << current << std::endl;
}

void Foo()
{
	throw std::out_of_range("G");

}

class Add 
{
public:
	int operator()(int &a, int &b)
	{
		std::cout << "a: " << a << " " << "b: " << b << std::endl;
		return a + b;
	}
};

void Eg1()
{
	int res[8];
	
	std::vector<int> vec1;
	vec1.push_back(4);
	vec1.push_back(5);
	vec1.push_back(7);
	vec1.push_back(8);
	
	std::vector<int> vec2;
	
	vec2.push_back(1);
	vec2.push_back(10);
	vec2.push_back(12);
	vec2.push_back(17);
	
	std::transform(vec1.begin(), vec1.end(), vec2.begin(), res, Add());
	
	for(int i = 0; i < 4; ++i)
	{
		std::cout << res[i] << std::endl;
	}
}

int Add(int n)
{
	int temp = n;
	
	return temp + 1;
}

int Shift(int &a, int &b)
{
	std::cout << "a: " << a << " " << "b: " << b << std::endl;
	int temp = b;
	b = 0;
	return temp;
}

int DoublePrint(int a, int b)
{
	std::cout << "a: " << a << " " << "b: " << b << std::endl;
	
	return a;
}

void Eg2()
{
	int arr[10];
	std::vector<int> vec1;
	vec1.push_back(4);
	vec1.push_back(5);
	vec1.push_back(7);
	vec1.push_back(8);
	vec1.push_back(10);
	vec1.push_back(13);
	std::cout << "rbegin: " << *(vec1.rbegin()) << "rend - 1: " << *(vec1.rend() - 1) << std::endl;
	
	std::transform(vec1.rbegin(), vec1.rend() - 1, vec1.rbegin() + 1, arr, DoublePrint);
	/*
	for(int i = 0; i < 6; ++i)
	{
		std::cout << vec1[i] << std::endl;
	}
	
	std::size_t chunks = 2;
	*/
	/*
	std::transform(vec1.rbegin() + chunks, vec1.rend(), vec1.rbegin() + chunks, vec1.rbegin(), Shift);
	
	for(int i = 0; i < 6; ++i)
	{
		std::cout << vec1[i] << std::endl;
	}*/
	
}

int Copy(int n)
{
	return n;
}

void CopyFromVecToVec()
{
	std::vector<int> vec1;
	std::vector<int> vec2;
	
	vec1.push_back(1);
	vec1.push_back(2);
	vec1.push_back(3);
	vec1.push_back(4);
	
	std::cout << "vec1: " << std::endl;
	
	std::for_each(vec1.begin(), vec1.end(), Print);
	
	vec2.resize(4);
	std::cout << "vec2: " << std::endl;
	std::for_each(vec2.begin(), vec2.end(), Print);
	std::transform(vec1.begin(), vec1.end(), vec2.begin(), Copy);

	
	std::cout << "vec2: " << std::endl;
	std::for_each(vec2.begin(), vec2.end(), Print);
}

int main() 
{
	 CopyFromVecToVec();
	
	
	return 0;
}
