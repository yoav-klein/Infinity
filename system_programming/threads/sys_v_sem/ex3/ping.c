#include <stdio.h> /* printf */
#include <sys/sem.h> /* semget */
#include <sys/ipc.h> /* ftok */
#include <unistd.h> /* getcwd */
#include <errno.h> /* perror */
#include <stdlib.h> /* malloc */ 
#include <string.h> /* strcat */
#include <signal.h> /* sigaction */

#define BUFF_SIZE (100)
#define IDENTIFIER (69)
#define SEM_PERMS (0600)
#define INIT_SEM_VAL (2)
#define PATH "pong.out"

volatile sig_atomic_t flag = 0;
int g_semid;
enum semaphores { SEM_PING, SEM_PONG, NUM_SEMS };

void SigHandler(int sig)
{
	flag = 1;
}

void InitSemaphore()
{
	key_t key = 0;
	
	key = ftok(PATH, IDENTIFIER);	
	
	g_semid = semget(key, NUM_SEMS, IPC_CREAT | SEM_PERMS);
	
	
	printf("ping: key - %d, id - %d\n", key, g_semid);
	
	/* init ping to 1 */
	if(-1 == semctl(g_semid, SEM_PING, SETVAL, 1))
	{
		perror("semctl");
	}
	
	/* and pong to 0 */
	if(-1 == semctl(g_semid, SEM_PONG, SETVAL, 0))
	{
		perror("semctl");
	}

}

void OpenPong()
{
	pid_t pong_pid = 0;
	
	pong_pid = fork();
	
	if(!pong_pid)
	{
		if(-1 == execl("pong.out", "pong.out", NULL))
		{
			perror("couldn't open pong");
		}
	}
}

void PlayPingPong()
{
	int num_of_times = 20;
	
	struct sembuf sembuf_ping;
	struct sembuf sembuf_pong;
	
	sembuf_ping.sem_num = SEM_PING;
	sembuf_ping.sem_op = -1;
	sembuf_ping.sem_flg = 0;
	
	sembuf_pong.sem_num = SEM_PONG;
	sembuf_pong.sem_op = 1;
	sembuf_pong.sem_flg = 0;
	
	while(0 < num_of_times && !flag)
	{
		--num_of_times;
		
		if(-1 == semop(g_semid, &sembuf_ping, 1))
		{
			perror("ping: semop");
		}
		
		printf("Ping!\n");
		
		sleep(1);
			
		if(-1 == semop(g_semid, &sembuf_pong, 1))
		{
			perror("ping: semop");
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

int main()
{
	atexit(CleanUp);
	
	InitSigHandler();	
	
	InitSemaphore();

	/*OpenPong();
*/
	PlayPingPong();

	printf("Ping finished!\n");
	
	return 0;
}


