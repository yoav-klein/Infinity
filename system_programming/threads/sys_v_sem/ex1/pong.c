#include <stdio.h> /* printf */
#include <sys/sem.h> /* semget */

#include <sys/ipc.h> /* ftok */
#include <unistd.h> /* getcwd */
#include <errno.h> /* perror */
#include <stdlib.h> /* malloc */ 
#include <string.h> /* strcat */

#define BUFF_SIZE (100)
#define SEM_PERMS (0600)
#define IDENTIFIER (69)

enum semaphores { SEM_PING, SEM_PONG, NUM_SEMS };

int InitSemaphore()
{	
	int semid = 0;
	key_t key = 0;
	char *path = "pong.out";
	
	key = ftok(path, IDENTIFIER);	
	
	semid = semget(key, NUM_SEMS, 0);

	if(-1 == semid)
	{
		perror("pong: semget");
	}
	
	return semid;
}

void PlayPingPong(int semid)
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
	
	while(0 < num_of_times)
	{
		--num_of_times;
		
		if(-1 == semop(semid, &sembuf_pong, 1))
		{
			perror("pong: semop");
		}
		
		sleep(3);
		
		printf("Pong!\n");
		
		if(-1 == semop(semid, &sembuf_ping, 1))
		{
			perror("pong: semop");
		}
	}
}

int main(int argc, char **argv)
{
	int semid = 0;
	
	semid = InitSemaphore();	
	
	PlayPingPong(semid);
		
	printf("Pong finished\n");
	return 0;
}


