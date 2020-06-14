/***************************************
*		Watchdog - AppSide
*		Author: Yoav Klein
*		Reviewer: Yael Lotan
*		Date: 21.8.19
*****************************************/

#include <stdio.h> /* printf */
#include <sys/types.h> /* pid_t */
#include <stdlib.h> /* malloc */
#include <signal.h> /* sigaction */
#include <unistd.h> /* getppid */
#include <sys/ipc.h> /* semop */
#include <sys/sem.h> /* semop */
#include <errno.h> /* perror */
#include <pthread.h> /* pthread_create */
#include <string.h> /* memset */
#include <wait.h> /* wait */

#include "defs.h" /* colors */
#include "scheduler.h" /* ScdCreate */
#include "watchdog.h"

#define NUM_TRIES (3)
#define FREQUENCY (2)
#define FTOK_PATH "watchdog.out"
#define SEM_PERMS (0600)
#define BUFF_SIZE (20)
#define WD_PATH "./watchdog.out"
#define SEMID_IND (1)
#define WD_PATH_IND (0)
#define SND_USR2_TIMES (3)

enum semaphores { APP_READY, WD_READY, NUM_SEMS };

volatile sig_atomic_t g_flag = NUM_TRIES;
volatile sig_atomic_t g_got_usr2;

static int g_should_stop;
static pid_t g_wd_pid;
static pthread_t g_thread_id;
static char **g_new_argv;
static int g_semid;

void USR1Handler(int sig)
{
	(void)sig;
	
	g_flag = NUM_TRIES;
}

void USR2Handler(int sig)
{
	(void)sig;
	
	g_got_usr2 = 1;
}

int InitSemaphores(int wd_id)
{
	key_t key = ftok(FTOK_PATH, wd_id);
	
	if(-1 == key)
	{
		perror("failed to create key");
		
		return -1;
	}
	
	g_semid = semget(key, NUM_SEMS, IPC_CREAT | IPC_EXCL | SEM_PERMS);
	
	if(g_semid > 0) /* we are the first */
	{
		if(-1 == semctl(g_semid, APP_READY, SETVAL, 0))
		{
			perror("failed to setval of semaphore app_ready");
		
			return -1;
		}
		
		if(-1 == semctl(g_semid, WD_READY, SETVAL, 0))
		{
			perror("failed to setval of semaphore wd_ready");
			
			return -1;
		}
	}
	else if(errno = EEXIST) /* semaphore exists, we ain't the first */
	{
		g_semid = semget(key, NUM_SEMS, SEM_PERMS);
		if(-1 == g_semid)
		{
			perror("failed to get existing semaphore");
			
			return -1;
		}
	}
	else	/* failed for some other reason */  
	{
		perror("failed to create semaphore for some reason");
		
		return -1;
	}
	
	return 0;
}

void *StartWatching(void *args)
{
	scd_t *scheduler = (scd_t*)args;
	
	ScdRun(scheduler);
	
	ScdDestroy(scheduler);
	
	return args;
}

int CreateThread(scd_t *scd)
{
	if(0 < pthread_create(&g_thread_id, NULL, StartWatching, scd))
	{
		return -1;
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
	
	printf(GREENL"\t\t\tApp Signals\n"RESET);
	kill(g_wd_pid, SIGUSR1);
	
	return 0;
}

long CheckSignals(void *args)
{
	struct sembuf buf_wd_ready;
	
	buf_wd_ready.sem_num = WD_READY;
	buf_wd_ready.sem_op = -2;
	buf_wd_ready.sem_flg = 0;
	
	(void)args;
	
	if(1 == g_should_stop)
	{
		return -1;
	}
	
	--g_flag;
	if(0 == g_flag)
	{
		wait(NULL);
		
		printf(GREENL"Reviving watchdog\n"RESET);
		g_wd_pid = fork();
		if(0 == g_wd_pid)
		{
			if(-1 == execv(WD_PATH, g_new_argv))
			{
				perror("raising watchdog failed");
			
				exit(1);
			}
		}
		
		semop(g_semid, &buf_wd_ready, 1);
		g_flag = NUM_TRIES;
	}
	
	return 0;
}

scd_t *InitScheduler()
{
	scd_t *scheduler = NULL;
	unid_t uid_check;
	unid_t uid_signal;
	
	scheduler = ScdCreate();
	if(!scheduler)
	{
		return NULL;
	}
	
	uid_check = ScdAdd(scheduler, FREQUENCY, CheckSignals, NULL);
	uid_signal = ScdAdd(scheduler, FREQUENCY, Signal, NULL);
	if(1 == UIDIsBad(uid_signal) || 1 == UIDIsBad(uid_check))
	{
		ScdDestroy(scheduler);
		
		return NULL;
	}
	
	return scheduler;
}

int CreateNewArgv(int argc, char **argv)
{
	int i = 0;
	
	g_new_argv = malloc(sizeof(*g_new_argv) * (argc + 3)); /* room for semid and watchdog.out */
	if(NULL == g_new_argv)
	{
		return -1;
	}
	
	g_new_argv[WD_PATH_IND] = WD_PATH;
	g_new_argv[SEMID_IND] = malloc(sizeof(char) * BUFF_SIZE);
	if(NULL == g_new_argv[SEMID_IND])
	{
		return -1;
	}
	
	sprintf(g_new_argv[SEMID_IND], "%d", g_semid);
	
	for(i = 0; i < argc; ++i)
	{
		g_new_argv[i + 2] = argv[i];
	}
	g_new_argv[i + 2] = NULL;
	
	return 0;
}

int InitHandlers()
{
	struct sigaction sa_usr1;
	struct sigaction sa_usr2;
	
	memset(&sa_usr1, 0, sizeof(sa_usr1));
	memset(&sa_usr2, 0, sizeof(sa_usr1));
	
	sa_usr1.sa_handler = USR1Handler;
	sa_usr2.sa_handler = USR2Handler;
	
	if(-1 == sigaction(SIGUSR1, &sa_usr1, NULL))
	{
		perror("sigaction failed");
		
		return -1;
	}
	
	if(-1 == sigaction(SIGUSR2, &sa_usr2, NULL))
	{
		perror("sigaction failed");
		
		return -1;
	}
	
	return 0;
}

int StartWatchDogIfShould()
{
	struct sembuf buf_app_ready;
	struct sembuf buf_wd_ready;
	
	buf_app_ready.sem_num = APP_READY;
	buf_app_ready.sem_op = 1;
	buf_app_ready.sem_flg = 0;
	
	buf_wd_ready.sem_num = WD_READY;
	buf_wd_ready.sem_op = -1;
	buf_wd_ready.sem_flg = 0;
	
	if(0 == semctl(g_semid, WD_READY, GETVAL)) /* we're the first, exec watchdog */
	{
		printf(GREENL"No watchDog - create it\n"RESET);
		g_wd_pid = fork();
		if(0 == g_wd_pid)
		{
			if(-1 == execv(WD_PATH, g_new_argv))
			{
				perror("raising watchdog failed");
				
				return -1;
			}
		}
		
		printf(GREENL"App: Waiting for watchdog\n"RESET);
		if(-1 == semop(g_semid, &buf_wd_ready, 1))
		{
			perror("semop: buf_wd_ready");	
			
			return -1;
		}
	}
	else
	{
		printf(GREEN"Watchdog exist\n"RESET); /*  watchdog exists */
		g_wd_pid = getppid();
		if(-1 == semop(g_semid, &buf_app_ready, 1))
		{
			perror("semop:buf_app_ready");
			
			return -1;
		}
	}
}

int WDStart(int argc, char **argv, int wd_id)
{
	scd_t *scd = NULL;
	
	if(-1 == InitHandlers())
	{
		return WD_E_SIGACT;
	}
	
	if(-1 == InitSemaphores(wd_id))
	{
		return WD_E_SEM;
	}
	
	if(-1 == CreateNewArgv(argc, argv))
	{
		printf("malloc failed\n");
		
		return WD_E_MEM;
	}
	
	StartWatchDogIfShould();
	
	scd = InitScheduler();
	
	if(NULL == scd)
	{
		printf("failed to create scheduler\n");
		
		return WD_E_MEM;
	}
	
	if(-1 == CreateThread(scd))
	{
		printf("failed to create thread\n");
		
		return WD_E_THREAD;
	}
	
	return WD_OK;
}

void WDStop()
{	
	int i = 0;
	
	g_should_stop = 1;
	
	/*  plan A */
	for(i = 0; i < SND_USR2_TIMES && !g_got_usr2; ++i)
	{
		kill(g_wd_pid, SIGUSR2);
		
		sleep(FREQUENCY);
	}
	
	/* plan B */
	kill(g_wd_pid, SIGKILL);
	
	if(-1 == semctl(g_semid, 0, IPC_RMID))
	{
		perror("failed to remove semaphore");
	}
	
	if(0 < pthread_join(g_thread_id, NULL))
	{
		printf("failed to join thread\n");
	}
	
	free(g_new_argv[SEMID_IND]);
	free(g_new_argv);
}


