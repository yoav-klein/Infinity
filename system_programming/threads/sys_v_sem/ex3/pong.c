#include <stdio.h> /* printf */
#include <sys/sem.h> /* semget */
#include <signal.h> /* sigaction */
#include <sys/ipc.h> /* ftok */
#include <unistd.h> /* getcwd */
#include <errno.h> /* perror */
#include <stdlib.h> /* malloc */ 
#include <string.h> /* strcat */

#define BUFF_SIZE (100)
#define SEM_PERMS (0600)
#define IDENTIFIER (69)

enum semaphores { SEM_PING, SEM_PONG, NUM_SEMS };
int g_semid;

volatile sig_atomic_t flag;

void SigHandler(int sig)
{
	flag = 1;
}

int InitSemaphore()
{	
	key_t key = 0;
	char *path = "pong.out";
	
	key = ftok(path, IDENTIFIER);	
	
	g_semid = semget(key, NUM_SEMS, 0);

	if(-1 == g_semid)
	{
		perror("pong: semget");
	}
}

void PlayPingPong()
{
	int num_of_times = 20;
	
	struct sembuf sembuf_ping;
	struct sembuf sembuf_pong;
	
	sembuf_ping.sem_num = SEM_PING;
	sembuf_ping.sem_op = 1;
	sembuf_ping.sem_flg = 0;
	
	sembuf_pong.sem_num = SEM_PONG;
	sembuf_pong.sem_op = -1;
	sembuf_pong.sem_flg = 0;
	
	while(0 < num_of_times && !flag)
	{
		--num_of_times;
		
		if(-1 == semop(g_semid, &sembuf_pong, 1))
		{
			perror("pong: semop");
		}
		
		sleep(1);
		
		printf("Pong!\n");
		
		if(-1 == semop(g_semid, &sembuf_ping, 1))
		{
			perror("pong: semop");
		}
	}
	
	if(-1 == semctl(g_semid, 0, IPC_RMID))
	{
		perror("sem removal failed\n");
	}
}

void CleanUp()
{
	if(-1 == semctl(g_semid, 0, IPC_RMID))
	{
		perror("ping: couldn't remove semaphore");
	}
}

void InitSigHandler()
{
	struct sigaction sa;
	sa.sa_handler = SigHandler;
	
	if(-1 == sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction:");
		
		return;
	}
}

int main(int argc, char **argv)
{
	atexit(CleanUp);
	
	InitSigHandler();
	
	InitSemaphore();
	
	PlayPingPong();
		
	printf("Pong finished\n");
	return 0;
}


