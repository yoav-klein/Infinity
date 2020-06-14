/*********************************************************************
*	Author: Yoav klein												  *
*	Date: 12.5.19													  * 	
**********************************************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h>  /*  malloc  */

int SoldiersSecond(int *arr, int num)
{
	int kill = 0;
	int jump = 2;
	int first_living = 0;

	while(first_living + (jump / 2) <= num-1)
	{
		kill = first_living + jump/2;
	
		while(kill <= (num - 1))
		{
			arr[kill] = 0;
			kill = kill + jump;
		}
	
	/* Checks weather the first living in the array 
	should be killed the next iteration, if does, defines the first 
	living as the next one after him */
		if( ((kill - jump + kill) / 2) <= num - 1) 
		{
			arr[first_living] = 0;
			first_living = first_living + jump;
		}
	
		jump = jump * 2;
	}

		return (first_living + 1);
}

int SoldiersFirst(int *arr, int num)
{
	int last_soldier = 0;
	int count_living = 0;
	int i = 0;
	int kill_flag = 0;

	do
	{
		count_living = 0;
		i = 0;
		while(i < num)
		{
			if(1 == arr[i])
			{
				++count_living;
				if(1 == kill_flag)
				{
					arr[i] = 0;
					kill_flag = 0;
					--count_living;
				}
				else
				{
					kill_flag = 1;
				}
			}
			
			i++;
		}
	} 
	while(count_living > 1);

	for(i = 0; i < num; i++)
	{
		if(arr[i] == 1)
		{
			last_soldier = i + 1;
		}
	}
	
	return (last_soldier);
}

int main()
{
	int i = 0;
	int number = 30;
	int last_soldier = 0;
	
	int *arr = (int *)malloc(sizeof(int) * number); 

	for(i = 0; i < number; i++)
	{
		*(arr + i) = 1;
	}

	
	last_soldier = SoldiersSecond(arr, number);
	printf("N: %d\n", last_soldier);
	last_soldier = SoldiersFirst(arr, number);
	printf("NLogN: %d\n", last_soldier);
	free(arr);

	return 0;
}
