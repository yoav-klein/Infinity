/******************************************
*		Watchdog - WD file
*		Author: Yoav Klein
*		Reviewer: Olga Kondroshova
*		Date: 8.9.19
*********************************************/

#include <sys/types.h> /* pid_t */
#include <unistd.h> /* getppid */
#include <sys/ipc.h> /* semop */
#include <sys/sem.h> /* semop */
#include <stdio.h> /* perror */
#include <stdlib.h> /* exit */
#include <signal.h> /* kill */

#include "scheduler.h" /* ScdCreate */
#include "wd_utils.h" /* StartWatching */

enum sems { WAIT_SEM, SIGN_SEM };

pid_t target_pid;
extern volatile sig_atomic_t g_should_stop;
extern volatile sig_atomic_t g_count_signals;
extern volatile sig_atomic_t g_got_usr2;

void USR2Handler(int sig)
{
	(void)sig;
	
	g_got_usr2 = 1;
	g_should_stop = 1;
}

void SignReady(int semid)
{
	struct sembuf buf;
	
	buf.sem_num = WAIT_SEM;
	buf.sem_op = 1; /* signs that watchdog exists */
	buf.sem_flg = 0;
	
	if(-1 == semop(semid, &buf, 1))
	{
		perror("watchdog: semop");
		
		exit(1);
	}
}

int main(int argc, char **argv)
{
	scd_t *scheduler = NULL;
	int semid = 0;
	(void)argc;
	
	target_pid = getppid();
	
	if(InitHandlers())
	{
		return 1;
	}
	
	scheduler = InitScheduler(target_pid);
	if(NULL == scheduler)
	{
		return 1;
	}
	
	semid = InitSemaphores(argv[0]);
	
	SignReady(semid);
	StartWatching(argv, scheduler, semid, argv[0]);
	
	kill(target_pid, SIGUSR2);
	
	ScdDestroy(scheduler);
	
	printf("Wd Exiting\n");
	return 0;
}


