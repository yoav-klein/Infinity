/******************************************
*		Watchdog - API file
*		Author: Yoav Klein
*		Reviewer: Olga Kondroshova
*		Date: 8.9.19
*********************************************/

#include <stdio.h> /* printf */
#include <sys/types.h> /* pid_t */
#include <stdlib.h> /* malloc */
#include <unistd.h> /* getppid */
#include <sys/ipc.h> /* semop */
#include <sys/sem.h> /* semop */
#include <errno.h> /* perror */
#include <pthread.h> /* pthread_create */
#include <signal.h> /* kill */

#include "scheduler.h" /* ScdCreate */
#include "wd_utils.h" /* StartWatching */
#include "staying_alive.h"

#define SND_USR2_TIMES (3)
#define WD_PATH "wd_exec.out"

pid_t target_pid;
extern volatile sig_atomic_t g_should_stop;
extern volatile sig_atomic_t g_count_signals;
extern volatile sig_atomic_t g_got_usr2;

void USR2Handler(int sig)
{
	(void)sig;
	
	g_got_usr2 = 1;
}

struct watchdog
{	
	char **argv;
	pthread_t thread_id;
	scd_t *scheduler;
	int semid;
};

enum sems { WAIT_SEM, SIGN_SEM };

void *CallStartWatching(void *args)
{
	watchdog_t wd_data = *(watchdog_t*)args;
	
	StartWatching(wd_data.argv, wd_data.scheduler, wd_data.semid, WD_PATH);
	
	return args;
}

int CreateThread(watchdog_t *wd_data)
{
	if(0 < pthread_create(&(wd_data->thread_id), NULL, CallStartWatching, wd_data))
	{
		return -1;
	}
	
	return 0;
}

void StartWatchDogIfShould(int semid, const char **argv)
{
	struct sembuf buf = { 0 };
	buf.sem_num = WAIT_SEM;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	
	if(0 == semctl(semid, SIGN_SEM, GETVAL)) /* we're the first, exec watchdog */
	{
		target_pid = fork();
		if(0 == target_pid)
		{
			if(-1 == execv(WD_PATH, (char**)argv))
			{
				perror("raising watchdog failed");
				
				exit(1);
			}
		}
		
		if(-1 == semop(semid, &buf, 1))
		{
			perror("semop");	
			
			return;
		}
	}
	else /* watchdog exist */
	{
		buf.sem_op = 1;
		target_pid = getppid();
		if(-1 == semop(semid, &buf, 1))
		{
			perror("semop");
			
			return;
		}
	}
}

watchdog_t *InitStructWithData(const char **argv, scd_t *scd, int semid)
{
	watchdog_t *wd_data = malloc(sizeof(*wd_data));
	if(!wd_data)
	{
		return NULL;
	}
	wd_data->argv= (char**)argv;
	wd_data->scheduler = scd;
	wd_data->semid = semid;
	
	return wd_data;
}

watchdog_t *KeepMeAlive(const char **argv)
{
	scd_t *scd = NULL;
	int semid = 0;
	watchdog_t *wd_data = NULL;
	
	if(-1 == InitHandlers())
	{
		return NULL;
	}
	
	semid = InitSemaphores(((char**)argv)[0]);
	
	if(-1 == semid)
	{
		return NULL;
	}
	
	scd = InitScheduler();
	if(!scd)
	{
		semctl(semid, 0, IPC_RMID);
	
		return NULL;
	}
	
	wd_data = InitStructWithData(argv, scd, semid);
	if(!wd_data)
	{
		semctl(semid, 0, IPC_RMID);
	
		return NULL;
	}
	
	StartWatchDogIfShould(semid, argv);
	
	if(-1 == CreateThread(wd_data))
	{
		printf("failed to create thread\n");
		
		return NULL;
	}
	
	return wd_data;
}

void FreeResources(watchdog_t *wd_data)
{
	ScdDestroy(wd_data->scheduler);
	semctl(wd_data->semid, 0, IPC_RMID);
	
	free(wd_data);
}

void LetMeDie(watchdog_t *watchdog_key)
{
	g_should_stop = 1;
	
	if(0 < pthread_join(watchdog_key->thread_id, NULL))
	{
		printf("failed to join thread\n");
	}
	
	FreeResources(watchdog_key);
}


