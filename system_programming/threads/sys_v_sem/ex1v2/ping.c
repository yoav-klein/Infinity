#include <stdio.h> /* printf */
#include <sys/sem.h> /* semget */
#include <sys/ipc.h> /* ftok */
#include <unistd.h> /* getcwd */
#include <errno.h> /* perror */
#include <stdlib.h> /* malloc */ 
#include <string.h> /* strcat */

#define BUFF_SIZE (100)
#define IDENTIFIER (69)
#define SEM_PERMS (0600)
#define INIT_SEM_VAL (2)

int g_semid;
enum semaphores { SEM_PING, SEM_PONG, NUM_SEMS };

void InitSemaphore()
{
	key_t key = 0;
	char *path = "pong.out";
	
	key = ftok(path, IDENTIFIER);	
	
	g_semid = semget(key, NUM_SEMS, IPC_CREAT | SEM_PERMS);
	
	printf("ping: %s\n", path);
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
	
	struct sembuf sembuf_arr[2];
	
	sembuf_arr[0].sem_num = SEM_PING;
	sembuf_arr[0].sem_op = -1;
	sembuf_arr[0].sem_flg = 0;
	
	sembuf_arr[1].sem_num = SEM_PONG;
	sembuf_arr[1].sem_op = 1;
	sembuf_arr[1].sem_flg = 0;
	
	while(0 < num_of_times)
	{
		--num_of_times;
		
		if(-1 == semop(g_semid, sembuf_arr, 2))
		{
			perror("ping: semop");
		}
		
		sleep(3);
		
		printf("Ping!\n");
	}
}

void CleanUp()
{
	if(-1 == semctl(g_semid, 0, IPC_RMID))
	{
		perror("ping: couldn't remove semaphore");
	}
}

int main()
{
	int semid = 0;
	
	atexit(CleanUp);
	
	InitSemaphore();

	/*OpenPong();
*/
	PlayPingPong(semid);

	printf("Ping finished!\n");
	
	return 0;
}


