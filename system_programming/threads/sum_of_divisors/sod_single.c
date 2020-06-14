#include <stdio.h>
#include <omp.h>

#define NUM (0xFFFFFF) 

size_t SumOfDivisors()
{
	size_t i = 0;
	size_t sum = 0;
	
	#pragma omp parallel for
	for(i = 1; i <= NUM; ++i)
	{
		if(0 == NUM % i)
		{
			sum += i;
		}
	}
	
	return sum;
	
}

int main()
{
	printf("Single: %lu\n", SumOfDivisors());
	
	return 0;
}
