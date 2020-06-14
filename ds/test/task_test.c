#include <time.h>
#include <stdio.h>

#include "../include/task.h"
#include "../include/uid.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

long PleaseDo(void *params)
{
	if((long)params == 1)
	{
		return 20;
	}
	
	if((long)params == 2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int Test()
{
	int flag = 1;
	int res = 0;
	time_t tm = 0;
		
	task_t *task = TaskCreate(50, PleaseDo, (void*)1);
	task_t *task2 = TaskCreate(50, PleaseDo, (void*)2);
	task_t *task3 = TaskCreate(30, PleaseDo, (void*)3); 
	
	/* check getuid */
	if(TaskGetUID(task).pid != TaskGetUID(task2).pid)
	{
		printf("1\n");
		flag  = 0;
	}
	
	if(TaskGetUID(task).counter != TaskGetUID(task2).counter - 1)
	{
		printf("2\n");
		flag = 0;
	}
	
	/*check gettime */
	tm = TaskGetTime(task);
	if(tm != (time(NULL) + 50))
	{
		printf("3\n");
		flag = 0;
	}
	
	res = TaskExecute(task);
	/* execute of task, should return 1 */
	if(1 != res)
	{
		printf("4\n");
		flag = 0;
	}
	
	/* check if time interval has changed */
	tm = TaskGetTime(task);
	if(tm != time(NULL) + 20)
	{
		printf("5\n");
		flag = 0;
	}
	
	res = TaskExecute(task3);
	/*  should return 0 */
	if(0 != res)
	{
		flag = 0;
	}
	
	/* time should stay the same */
	tm = TaskGetTime(task3);
	if(tm != time(NULL) + 30)
	{
		flag = 0;
	}
	
	res =TaskExecute(task2);
	if(1 != res)
	{
		flag = 0;
	}
	
	return flag;
}

int main()
{
	RUN_TEST(Test);

	return 1;		
}
