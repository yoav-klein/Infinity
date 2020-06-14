/******************************************
*		Watchdog - shared library file
*		Author: Yoav Klein
*		Reviewer: Olga Kondroshova
*		Date: 8.9.19
*********************************************/

#include <stdio.h> /* perror */
#include <sys/types.h> /* pid_t */
#include <stdlib.h> /* malloc */
#include <signal.h> /* sigaction */
#include <sys/ipc.h> /* semget */
#include <sys/sem.h> /* semget */
#include <string.h> /* memset */
#include <unistd.h> /* fork */
#include <errno.h> /* errno */

#include "scheduler.h" /* ScdCreate */

#define WD_PATH "wd_exec.out"
#define PROJ_ID (55)
#define NUM_TRIES (3)
#define FREQUENCY (1)
#define SEM_PERMS (0600)

enum sems { WAIT_SEM, SIGN_SEM, NUM_SEMS };

extern pid_t target_pid;
volatile sig_atomic_t g_should_stop;
volatile sig_atomic_t g_count_signals;
volatile sig_atomic_t g_got_usr2;

void USR1Handler(int sig)
{
	(void)sig;
	
	g_count_signals = NUM_TRIES;
}

void USR2Handler(int sig); /* defined in watchdog.c and appside.c */ 

long CheckSignals(void *args)
{
	scd_t *scheduler = (scd_t*)args;

	--g_count_signals;
	if(0 == g_count_signals)
	{
		ScdStop(scheduler);
		
		g_count_signals = NUM_TRIES;
	}
	else if(1 == g_should_stop)
	{
		if(!g_got_usr2) 
		{
			kill(target_pid, SIGUSR2); 
		}
		else
		{
			ScdStop(scheduler);
		}
	} 
	
	return 0;
}

long Signal(void *args)
{
	(void)args;
	
	kill(target_pid, SIGUSR1);
	
	return 0;
}

scd_t *InitScheduler()
{
	unid_t uid_signal;
	unid_t uid_check;
	scd_t *scheduler = NULL;
	
	scheduler = ScdCreate();
	if(!scheduler)
	{
		return NULL;
	}
	
	uid_check = ScdAdd(scheduler, FREQUENCY, CheckSignals, scheduler);
	uid_signal = ScdAdd(scheduler, FREQUENCY, Signal, NULL);
	if(1 == UIDIsBad(uid_signal) || 1 == UIDIsBad(uid_check))
	{
		ScdDestroy(scheduler);

		return NULL;
	}
	
	return scheduler;	
}

int InitHandlers()
{
	struct sigaction sa_usr1 = { 0 };
	struct sigaction sa_usr2 = { 0 };

	sa_usr1.sa_handler = USR1Handler;
	sa_usr2.sa_handler = USR2Handler;
	
	if(-1 == sigaction(SIGUSR1, &sa_usr1, NULL))
	{
		perror("sigaction usr1 failed:");
		
		return -1;
	}
	
	if(-1 == sigaction(SIGUSR2, &sa_usr2, NULL))
	{
		perror("sigaction usr2 failed:");
		
		return -1;
	}
	
	return 0;
}

int InitSemaphores(char *app_path)
{
	key_t key = ftok(app_path, PROJ_ID);
	int semid = 0;
	
	if(-1 == key)
	{
		perror("failed to create key");
		
		return -1;
	}
	
	semid = semget(key, NUM_SEMS, IPC_CREAT | IPC_EXCL | SEM_PERMS);
	
	if(semid > 0) /* we are the first */
	{
		if(-1 == semctl(semid, WAIT_SEM, SETVAL, 0))
		{
			perror("failed to setval of semaphore app_ready");
		
			return -1;
		}
		
		if(-1 == semctl(semid, SIGN_SEM, SETVAL, 0))
		{
			perror("failed to setval of semaphore wd_ready");
			
			return -1;
		}
	}
	else if(EEXIST == errno) /* semaphore exists, we ain't the first */
	{
		semid = semget(key, NUM_SEMS, SEM_PERMS);
		if(-1 == semid)
		{
			perror("failed to get existing semaphore");
			
			return -1;
		}
		
		if(-1 == semctl(semid, SIGN_SEM, SETVAL, 1))  /* sign that wd exist */
		{
			perror("failed to set value of sign_sem");
		}
	
	}
	else	/* failed for some other reason */ 
	{
		perror("failed to create semaphore for some reason");
		
		return -1;
	}
	
	return semid;
}

void StartWatching(char **argv, scd_t *scheduler, int semid, char *who_to_revive)
{
	struct sembuf buf = { 0 };
	buf.sem_num = WAIT_SEM;
	buf.sem_op = -1;
	buf.sem_flg = 0;
	
	while(!g_should_stop)
	{
		ScdRun(scheduler);
		
		if(!g_should_stop)
		{
			target_pid = fork();
			if(0 == target_pid)
			{
				execv(who_to_revive, argv);
			}
			
			semop(semid, &buf, 1);
		}
	}
}

