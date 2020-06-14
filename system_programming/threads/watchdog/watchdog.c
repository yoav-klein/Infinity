/******************************************
*		Watchdog
*		Author: Yoav Klein
*		Reviewer: Yael Lotan
*		Date: 21.8.19
*********************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <sys/types.h> /* pid_t */
#include <stdlib.h> /* malloc */
#include <signal.h> /* sigaction */
#include <unistd.h> /* getppid */
#include <sys/ipc.h> /* semop */
#include <sys/sem.h> /* semop */
#include <errno.h> /* perror */
#include <string.h> /* memset */
#include <wait.h> /* wait */

#include "defs.h" /* colors */
#include "scheduler.h" /* ScdCreate */

#define NUM_TRIES (3)
#define SEM_ID_INDEX (1)
#define PATH_INDEX (2)
#define FREQUENCY (2)

enum semaphores { APP_READY, WD_READY };

volatile sig_atomic_t g_should_stop;
volatile sig_atomic_t g_flag = NUM_TRIES;
static pid_t g_app_pid;

void USR1Handler(int sig)
{
	(void)sig;
	
	g_flag = NUM_TRIES;
}

void USR2Handler(int sig)
{
	(void)sig;
	g_should_stop = 1;
	
	kill(g_app_pid, SIGUSR2);
}

long CheckSignals(void *args)
{
	struct sembuf buf_app_ready;
	char **argv = (char**)args;
	int semid = 0;
	
	assert(args);
	
	semid = atoi(argv[SEM_ID_INDEX]);
	
	buf_app_ready.sem_num = APP_READY;
	buf_app_ready.sem_op = -1;
	buf_app_ready.sem_flg = 0;
	
	if(1 == g_should_stop)
	{
		return -1;
	}
	
	--g_flag;
	if(0 == g_flag)
	{
		wait(NULL);
		
		g_app_pid = fork();
		if(0 == g_app_pid)
		{
			/* raise the app with argv without the the first 2 arguments, which are watchdog.out and 				semid */
			if(-1 == execv(argv[PATH_INDEX], (argv + 2))) 
			{
				perror("failed to open app:");
				
				exit(1);
			}
		}
		printf("WD: Raised app: %d\n", g_app_pid);
		
		semop(semid, &buf_app_ready, 1);
		g_flag = NUM_TRIES;
	}
	
	return 0;
}

long Signal(void *args)
{
	(void)args;
	
	if(1 == g_should_stop)
	{
		return -1;
	}
	
	printf(YELLOW"\t\t\tWD Signals\n");
	kill(g_app_pid, SIGUSR1);
	
	return 0;
}

scd_t *InitScheduler(char **argv)
{
	unid_t uid_signal;
	unid_t uid_check;
	scd_t *scheduler = NULL;
	
	scheduler = ScdCreate();
	if(!scheduler)
	{
		printf("couldn't create schduler\n");
		
		exit(1);
	}
	
	uid_check = ScdAdd(scheduler, FREQUENCY, CheckSignals, argv);
	uid_signal = ScdAdd(scheduler, FREQUENCY, Signal, NULL);
	if(1 == UIDIsBad(uid_signal) || 1 == UIDIsBad(uid_check))
	{
		ScdDestroy(scheduler);
		printf("couldn't create task signal\n");
		
		exit(1);
	}
	
	return scheduler;	
}

void SignReady(char **argv)
{
	struct sembuf buf_wd_ready;
	
	buf_wd_ready.sem_num = WD_READY;
	buf_wd_ready.sem_op = 2; /* signs that watchdog exists */
	buf_wd_ready.sem_flg = 0;
	
	int semid = atoi(argv[SEM_ID_INDEX]);
	
	sleep(1);
	printf(YELLOW"WD: I'm ready!\n"RESET);
	
	if(-1 == semop(semid, &buf_wd_ready, 1))
	{
		perror("watchdog: semop wd_ready + 2");
		
		exit(1);
	}
}

void InitHandlers()
{
	struct sigaction sa_usr1;
	struct sigaction sa_usr2;
	
	memset(&sa_usr1, 0, sizeof(sa_usr1));
	memset(&sa_usr2, 0, sizeof(sa_usr1));
	
	sa_usr1.sa_handler = USR1Handler;
	sa_usr2.sa_handler = USR2Handler;
	
	if(-1 == sigaction(SIGUSR1, &sa_usr1, NULL))
	{
		perror("sigaction usr1 failed:");
		
		exit(1);
	}
	
	if(-1 == sigaction(SIGUSR2, &sa_usr2, NULL))
	{
		perror("sigaction usr2 failed:");
		
		exit(1);
	}
}

void RunScheduler(scd_t *scheduler)
{
	ScdRun(scheduler);
}

int main(int argc, char **argv)
{
	scd_t *scheduler = NULL;
	
	g_app_pid = getppid();
	
	InitHandlers();
	
	scheduler = InitScheduler(argv);
	if(NULL == scheduler)
	{
		return 1;
	}
	
	SignReady(argv);
	
	RunScheduler(scheduler);
	
	ScdDestroy(scheduler);
	
	return 0;
}


