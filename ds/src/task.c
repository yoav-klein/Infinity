/*******************************************
*		Task
*		Author: Yoav klein
*		Reviewer: Yael Lotan
*		Date: 16.6.19
********************************************/

#include <stdlib.h> /* malloc */
#include <time.h> /* time */
#include <assert.h> /* assert */

#include "../include/task.h" /* TaskCreate */


struct task
{
	func_t func;
	void *params;
	unid_t uid;
	size_t interval;
	time_t time_to_run;
};

task_t *TaskCreate(size_t interval, func_t func, void *params)
{

	task_t *task = (task_t*)malloc(sizeof(task_t));
	
	assert(NULL != func);
	if(NULL == task)
	{
		return NULL;
	}
	
	task->func = func;
	task->params = params;
	task->uid = UIDCreate();
	task->interval = interval;
	task->time_to_run = interval + time(NULL);
	
	return task;
}

time_t TaskGetTime(const task_t *task)
{
	assert(NULL != task);
	
	return task->time_to_run;
}

int TaskExecute(task_t *task)
{
	long new_interval = 0;
	
	assert(NULL != task);
		
	if(task->time_to_run >= time(NULL))
	{
		new_interval = task->func(task->params);
		if(new_interval == -1)
		{
			return 0;
		}
		
		if(0 < new_interval)
		{
			task->interval = new_interval;
		}
	}
	
	task->time_to_run = task->interval + time(NULL);
	
	return 1;
}

unid_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

void TaskDestroy(task_t *task)
{
	free(task);
}



