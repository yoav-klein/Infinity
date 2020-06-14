#include <stdio.h>
#include <unistd.h>

#include "staying_alive.h"

int main(int argc, char **argv)
{
	int time_to_sleep = 20;
	watchdog_t *wd = NULL;
	
	printf("-------TEST-------\n");
	sleep(5);
	(void)argc;
	
	wd = KeepMeAlive((const char **)argv);
	
	while(time_to_sleep > 0)
	{
		printf("time to sleep: %d\n", time_to_sleep);
		time_to_sleep = sleep(time_to_sleep);
	}
	
	LetMeDie(wd);
	
	return 0;
}
