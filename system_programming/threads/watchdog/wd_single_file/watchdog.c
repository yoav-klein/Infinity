#include <stdlib.h> /* exit */
#include <stdio.h> /* printf */
#include <sys/types.h> /* pid_t */
#include <signal.h> /* sigaction */
#include <unistd.h> /* getppid */
#include <sys/ipc.h> /* semop */
#include <sys/sem.h> /* semop */
#include <errno.h> /* perror */
#include <string.h> /* memset */
#include <wait.h> /* wait */
#include <pthread.h> /* pthread_create */

#include "defs.h" /* colors */
#include "scheduler.h" /* ScdCreate */
#include "watchdog.h"

#define SEM_PERMS (0600)
#define NUM_SEMS (1)
#define NUM_TRIES (3)
#define FREQUENCY (2)
#define FTOK_PATH "test.out"
#define IM_WD (1)
#define IM_APP (2)
#define SND_USR2_TIMES (3)

volatile sig_atomic_t g_flag = NUM_TRIES;
volatile sig_atomic_t g_got_usr2;

static int g_is_wd;
static int g_should_stop;
static pid_t g_target_pid;
static int g_semid;
static pthread_t g_thread_id;

void USR1Handler(int sig)
{
	(void)sig;

	g_flag = NUM_TRIES;
}

void USR2Handler(int sig)
{
	(void)sig;
	g_should_stop = 1;
	
	if(g_is_wd)
	{
		kill(g_target_pid, SIGUSR2);
	}
	else
	{
		g_got_usr2 = 1;
	}
}

int InitSemaphore(int wd_id)
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
		if(-1 == semctl(g_semid, 0, SETVAL, 0))
		{
			perror("failed to setval of semaphore");
			
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

int AmIWatchDog()
{
	struct sembuf buf = { 0, (IM_APP * (-1)), 0 };
	
	int sem_val = semctl(g_semid, 0, GETVAL);
	
	if(0 == sem_val) /* we're the first, fork watchdog */
	{
		printf(GREENL"No watchDog - create it\n"RESET);
		g_target_pid = fork();
		if(0 == g_target_pid)
		{
			g_is_wd = 1;
			g_target_pid = getppid();
		}
	}
	else
	{
		g_target_pid = getppid();
		if(IM_APP == sem_val)
		{
			g_is_wd = 0;
		}
		else
		{
			g_is_wd = 1;
		}
	}
	
	if(-1 == semctl(g_semid, 0, SETVAL, 0))
	{
		perror("semctl failed:");
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
	
	if(g_is_wd)
	{
		printf(YELLOW"\t\t\tWD Signals %d\n"RESET, getpid());
	}
	else
	{
		printf(GREENL"\t\t\tApp Signals %d\n"RESET, getpid());
	}
	
	kill(g_target_pid, SIGUSR1);
	
	return 0;
}

long CheckSignals(void *args)
{
	char **argv = (char**)args;
		
	struct sembuf buf =  {0, 0, 0};
	
	if(1 == g_should_stop)
	{
		return -1;
	}
	
	--g_flag;
	if(0 == g_flag)
	{
		wait(NULL);
		
		printf(GREENL"%d Reviving\n"RESET, getpid());
		
		if(-1 == semctl(g_semid, 0, SETVAL, g_is_wd == 1 ? IM_APP : IM_WD))
		{
			perror("semctl failed");
		}
		
		g_target_pid = fork();
		if(0 == g_target_pid)
		{
			if(-1 == execv(argv[0], argv))
			{
				perror("raising failed");
			
				exit(1);
			}
		}
		
		semop(g_semid, &buf, 1);
		g_flag = NUM_TRIES;
	}
	
	return 0;
}

scd_t *InitScheduler(char **argv)
{
	scd_t *scheduler = NULL;
	unid_t uid_check;
	unid_t uid_signal;
	
	scheduler = ScdCreate();
	if(!scheduler)
	{
		return NULL;
	}
	
	uid_check = ScdAdd(scheduler, FREQUENCY, CheckSignals, argv);
	uid_signal = ScdAdd(scheduler, FREQUENCY, Signal, NULL);
	if(1 == UIDIsBad(uid_signal) || 1 == UIDIsBad(uid_check))
	{
		ScdDestroy(scheduler);
		
		return NULL;
	}
	
	return scheduler;
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

int WDStart(int argc, char **argv, int wd_id)
{
	scd_t *scd = NULL;
	
	if(-1 == InitHandlers())
	{
		return WD_E_SIGACT;
	}
	
	if(-1 == InitSemaphore(wd_id))
	{
		return WD_E_SEM;
	}
	
	AmIWatchDog();
	
	scd = InitScheduler(argv);
	
	if(NULL == scd)
	{
		printf("failed to create scheduler\n");
		
		return WD_E_MEM;
	}
	
	if(1 == g_is_wd)
	{
		StartWatching((void*)scd);
		
		exit(0);
	}
	else
	{
		if(-1 == CreateThread(scd))
		{
			printf("failed to create thread\n");
		
			return WD_E_THREAD;
		}
	}
	
	return WD_OK;
}

void WDStop()
{
	int i = 0;
	
	g_should_stop = 1;
	
	for(i = 0; i < SND_USR2_TIMES && !g_got_usr2; ++i)
	{
		kill(g_target_pid, SIGUSR2);
		
		sleep(FREQUENCY);
	}
	if(!g_got_usr2)
	{
		kill(g_target_pid, SIGKILL);
	}
	
	if(-1 == semctl(g_semid, 0, IPC_RMID))
	{
		perror("failed to remove semaphore");
	}
	
	if(0 < pthread_join(g_thread_id, NULL))
	{
		printf("failed to join thread\n");
	}
}

int main(int argc, char **argv)
{
	unsigned int time_to_sleep = 30;
	
	WDStart(argc, argv, 59);
	
	while(time_to_sleep > 0)
	{
		printf("time to sleep: %d\n", time_to_sleep);
		time_to_sleep = sleep(time_to_sleep);
	}
	
	WDStop();
	
	return 0;
}	

