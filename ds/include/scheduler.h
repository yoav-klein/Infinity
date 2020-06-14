#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "uid.h"

#ifndef FUNC_T
#define FUNC_T

typedef long(*func_t)(void *params); /* returns negative number for remove task, 0 for no change in inverval or positive number for new inverval  */

#endif

typedef struct scheduler scd_t;

/* creates a scheduler. returns a scd_t pointer to the scheduler. */
scd_t *ScdCreate();

/* destroyes the scheduler. */
void ScdDestroy(scd_t *scheduler);

/* adds a task to the schduler. receives as parameters: a specific 
'scheduler', interval in seconds for executing the function. 
'func' - function to execute, from type func_t. 'params' - parameters
that will be passed to the 'func'.
the function returns a unique id from type unid_t. if fails,
returns 'bad uid'. the user can check if the uid is 'bad uid' 
using the function int UIDIsBad(unid_t uid) that returns 1 for 
bad uid and 0 otherwise. 
the function 'func' should return the new interval to be set. if 0 is 
returned, no change will occur. if -1 will be returned, the task
will no be recurred. */
unid_t ScdAdd(scd_t *scheduler, time_t interval, func_t func, void *params);

/* the function removes the task with the specified task_uid .
 returns 0 on success or 1 on failure (uid not found)*/
int ScdRemove(scd_t *scheduler, unid_t task_uid); 

/* runs the scheduler. in order to stop, the user should create a task
that implements the function ScdStop(scheduler) with the specified time.
returns the remaining tasks to be done. */
size_t ScdRun(scd_t *scheduler); 

/* Stops the running of scheduler. */
void ScdStop(scd_t *scheduler);

/* returns the number of tasks in the scheduler. */
size_t ScdSize(const scd_t *scheduler);

/* returns 1 if the scheduler is empty. 0 otherwise. */
int ScdIsEmpty(const scd_t *scheduler);


#endif




