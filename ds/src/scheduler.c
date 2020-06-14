/**********************************************
*	Scheduler
*	Author: Yoav Klein
*	Reviewer: Yael Lotan
*	Date: 16.6.19
**************************************************/


#include <unistd.h> /* sleep */
#include <stdlib.h> /* malloc */
#include <time.h> /* time_t */
#include <errno.h> /* errno */
#include <assert.h> /* assert */

#include "../include/scheduler.h" /* ScdCreate */
#include "../include/task.h" /*  TaskCreate */
#include "../include/pq.h" /* PqCreate */

struct scheduler
{
	pq_t *task_list;
	int should_stop;
};

int Prioritize(const void *tasks_in_list, const void *new_task, const void *params)
{
	(void)params;
	
	return TaskGetTime((task_t*)new_task) < TaskGetTime((task_t*)tasks_in_list);
}

int FindAndErase(const void *task_p, const void *uid_p, const void *params)
{
	(void)params;
	if(UIDIsSame(TaskGetUID(task_p), *(unid_t*)uid_p))
	{
		TaskDestroy((task_t*)task_p);
		return 1;
	}
	
	return 0;
}

scd_t *ScdCreate()
{
	scd_t *scheduler = NULL;
	pq_t *pq = PQCreate(Prioritize);
	if(NULL == pq)
	{
		return NULL;
	}
	
	scheduler = (scd_t*)malloc(sizeof(scd_t));
	if(NULL == scheduler)
	{
		free(pq);
		return NULL;
	}
	
	scheduler->task_list = pq;
	scheduler->should_stop = 0;
	
	return scheduler;
}

void ScdDestroy(scd_t *scheduler)
{
	assert(NULL != scheduler);	
	
	while(!ScdIsEmpty(scheduler))
	{
		TaskDestroy(PQPeek(scheduler->task_list));
		PQDequeue(scheduler->task_list);
	}
	
	PQDestroy(scheduler->task_list);
	free(scheduler);
}

unid_t ScdAdd(scd_t *scheduler, time_t interval, func_t func, void *params)
{
	int res = 0;
	
	task_t *new_task = NULL;
	
	assert(NULL != scheduler && NULL != func);
	
	new_task = TaskCreate(interval, func, params);
	if(NULL == new_task)
	{
		return bad_uid;
	}
	
	res = PQEnqueue(scheduler->task_list, new_task);
	if(res == 1)
	{
		return bad_uid;
	}
	
	return TaskGetUID(new_task);
}

int ScdRemove(scd_t *scheduler, unid_t task_uid)
{
	assert(NULL != scheduler && !UIDIsBad(task_uid));
	
	return PQErase(scheduler->task_list, (void*)&task_uid, FindAndErase);
}

size_t ScdRun(scd_t *scheduler)
{
	int res = 0;
	task_t *curr_task = NULL;
	time_t next_time = 0;
	
	assert(NULL != scheduler);
	
	scheduler->should_stop = 0;
	
	while(!ScdIsEmpty(scheduler) && !scheduler->should_stop)
	{
		curr_task = (task_t*)PQPeek(scheduler->task_list);
		
		next_time = TaskGetTime(curr_task) - time(NULL);
		
		while(next_time > 0)
		{
			next_time = sleep(next_time);
		}
		
		res = TaskExecute(curr_task);
		if(!res)
		{
			TaskDestroy(curr_task);
		}
		else
		{
			res = PQEnqueue(scheduler->task_list, curr_task);
			if(res > 0)
			{	
				errno = ENOMEM;
				
				return ScdSize(scheduler);
			}
		}
		
		PQDequeue(scheduler->task_list);
	}
	
	return ScdSize(scheduler);
}

void ScdStop(scd_t *scheduler)
{
	assert(NULL != scheduler);
	
	scheduler->should_stop = 1;
}

size_t ScdSize(const scd_t *scheduler)
{
	assert(NULL != scheduler);
	
	return PQSize(scheduler->task_list);
}

int ScdIsEmpty(const scd_t *scheduler)
{
	assert(NULL != scheduler);	
	
	return PQIsEmpty(scheduler->task_list);
}

