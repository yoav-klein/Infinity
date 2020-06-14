#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "../include/scheduler.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 

struct note
{
	time_t tm;
	int func_num;
};

struct note result[20];
size_t res_count;


long Func1(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 1;
	result[res_count].tm = time(NULL);
	
	++res_count;
	
	printf("Func 1\n");
	
	return 0;
}

long Func2(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 2;
	
	++res_count;
	
	printf("Func 2\n");
	
	return 0;
}

long Func3(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 3;
	
	++res_count;
	printf("Func 3\n");
	
	return -1;
}

long Func4(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 4;
	
	++res_count;
	
	printf("Func 4\n");
	
	return 10;
}

long Func5(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 5;
	
	++res_count;
	
	printf("Func 5\n");
	
	return 10;
}

long Func6(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 6;
	
	++res_count;
	
	printf("Func 6\n");
	
	return 0;
}

long Func7(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 7;
	
	++res_count;
	
	printf("Func 7\n");
	
	return 0;
}

long Func8(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 8;
	
	++res_count;
	
	printf("Func 8\n");
	
	return 0;
}

long Func9(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 9;
	
	++res_count;
	
	printf("Func 9\n");
	
	return -1;
}

long Func10(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 10;
	
	++res_count;
	
	printf("Func 10\n");
	
	return -1;
}

long Func11(void *params)
{
	(void)params;
	
	result[res_count].tm = time(NULL);
	result[res_count].func_num = 11;
	
	++res_count;
	
	printf("Func 11\n");
	
	return -1;
}

long RemoveFunc(void *params)
{
	ScdRemove(*(scd_t**)params, **((unid_t**)params + 1));
	
	return -1;
}

long StopFunc(void *params)
{	
	ScdStop((scd_t*)params);

	printf("Stop\n");

	return 7;
}

long AddTask(void *params)
{
	ScdAdd((scd_t*)params, 5, Func1, NULL);
	
	return 0;
}

int FlowTest1()
{
	int cmp_res[20];
	int i = 0;
	int flag = 1;
	int res = 0;
	
	scd_t *scd = ScdCreate();
	unid_t uid;
	
	/* Test Plan:
	
	1 and 2: each 5 seconds.
	3: after 5 seconds, then stop
	4: after 5 seconds, then change to 10 
	
	run 20 seconds. then stop, and remove 2. run again for another 7 seconds.
	*/
	
	ScdAdd(scd, 5, Func1, NULL);
	uid = ScdAdd(scd, 5, Func2, NULL);
	ScdAdd(scd, 5, Func3, NULL);
	ScdAdd(scd, 5, Func4, NULL);
	ScdAdd(scd, 21, StopFunc, scd);
	
	/* check size */
	res = ScdSize(scd);
	if(5 != res)
	{
		flag = 0;
	}	
	
	ScdRun(scd);
	
	/* check size again */
	res = ScdSize(scd);
	if(4 != res)
	{
		flag = 0;
	}
	
	ScdRemove(scd, uid);
	res = ScdSize(scd);
	if(3 != res)
	{
		flag = 0;
	}
	
	ScdAdd(scd, 2, Func5, NULL);
	res = ScdSize(scd);
	if(4 != res)
	{
		flag = 0;
	}
	
	ScdRun(scd);
	
	cmp_res[0] = 1;
	cmp_res[1] = 2;
	cmp_res[2] = 3;
	cmp_res[3] = 4;
	
	cmp_res[4] = 1;
	cmp_res[5] = 2;
	
	cmp_res[6] = 4;
	cmp_res[7] = 1;
	cmp_res[8] = 2;
	
	cmp_res[9] = 1;
	cmp_res[10] = 2;
	
	cmp_res[11] = 5;
	cmp_res[12] = 4;
	cmp_res[13] = 1;
	
	for(i = 0; i < 13; ++i)
	{
		if(cmp_res[i] != result[i].func_num)
		{
			flag = 0;
		}	
	}
	
	ScdDestroy(scd);
	
	return flag;
}


int TaskInPast()
{
	int cmp_res[20];
	int i = 0;
	int flag = 1;
	
	scd_t *scd = ScdCreate();
	
	/* Add 4 tasks: 
	func6: each 3 seconds
	func7: each 5 seconds
	func8: each 10 seconds
	stop: after 24 seconds.
	sleep 8 seconds before running.   */	
	ScdAdd(scd, 3, Func6, NULL);
	ScdAdd(scd, 5, Func7, NULL);
	ScdAdd(scd, 10, Func8, NULL);
	ScdAdd(scd, 24, StopFunc, scd);
	
	sleep(8);
	
	ScdRun(scd);

	ScdDestroy(scd);
	
	cmp_res[0] = 8;
	cmp_res[1] = 6;
	cmp_res[2] = 7;
	
	for(i = 0; i < 3; ++i)
	{
		if(result[i].func_num != cmp_res[i])
		{
			flag = 0;
		}
	}		
	
	return flag;
}

int EmptyScheduler()
{
	int cmp_res[20];
	int i = 0;
	int flag = 1;
	
	scd_t *scd = ScdCreate();
	
	ScdAdd(scd, 3, Func9, NULL);
	ScdAdd(scd, 5, Func10, NULL);
	ScdAdd(scd, 10, Func11, NULL);

	sleep(8);
	
	ScdRun(scd);
	
	ScdDestroy(scd);
	
	cmp_res[0] = 11;
	cmp_res[1] = 9;
	cmp_res[2] = 10;
	
	for(i = 0; i < 3; ++i)
	{
		if(result[i].func_num != cmp_res[i])
		{
			flag = 0;
		}
	}		
	
	return flag;

}

int AddTaskFromTask()
{

 	/*  
 	 	Explanation: i add a task with an interval of 3 secs
 	 	that adds Func1 with an interval of 5 secs.
 	 	i stop after 20 secs.
 	 	Func1 writes to the global array result[count].tm 
 	 	the current time of running. i expect it to be
 	 	correspondent with the values listed below in the array
 	 	cmp_res.
 	
 	*/
	time_t cmp_res[20];
	int i = 0;
	int flag = 1;
	time_t start = time(NULL);
	
	scd_t *scd = ScdCreate();
	
	ScdAdd(scd, 3, AddTask, scd);
	ScdAdd(scd, 20, StopFunc, scd);
	
	ScdRun(scd);
	
	cmp_res[0] = 8;
	cmp_res[1] = 11;
	cmp_res[2] = 13;
	cmp_res[3] = 14;
	cmp_res[4] = 16;
	cmp_res[5] = 17;
	cmp_res[6] = 18;
	cmp_res[7] = 19;
	
	for(i = 0; i < 8; ++i)
	{
		if(cmp_res[i] + start != result[i].tm)
		{
			flag = 0;
		}
	}
	
	ScdDestroy(scd);
	
	return flag;
}

int RemoveTaskFromTask()
{
	/* 
		Explanation: i add Func1 with recurrence of 3 secs,
		Func2 with recurrence of 5 secs. i add RemoveFunc
		that removes Func1 after 10 secs. i expect that 
		after 10 secs Func1 won't be executed.
	*/
	
	int cmp_res[20];
	int i = 0;
	int flag = 1;
	void *arr[2];
	unid_t uid = bad_uid;
	
	scd_t *scd = ScdCreate();
	
	arr[0] = (void*)scd;
	arr[1] = (void*)&uid;
	
	uid = ScdAdd(scd, 3, Func1, NULL);
	ScdAdd(scd, 5, Func2, NULL);
	ScdAdd(scd, 10, RemoveFunc, arr);
	ScdAdd(scd, 17, StopFunc, scd);
	
	ScdRun(scd);
	
	cmp_res[0] = 1;
	cmp_res[1] = 2;
	cmp_res[2] = 1;
	cmp_res[3] = 1;
	cmp_res[4] = 2;
	cmp_res[5] = 2;
	
	for(i = 0; i < 6; ++i)
	{
		if(cmp_res[i] != result[i].func_num)
		{
			flag = 0;
		}
	}
	
	return flag;
}

int main()
{
/*	 RUN_TEST(FlowTest1); */
/*	RUN_TEST(TaskInPast);*/
/*	RUN_TEST(EmptyScheduler);*/
/*	RUN_TEST(AddTaskFromTask)*/
	RUN_TEST(RemoveTaskFromTask)
	
	return 1;	
}
