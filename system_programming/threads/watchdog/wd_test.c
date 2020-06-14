#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "watchdog.h"

#define WD_ID (55)

int main(int argc, char **argv)
{
	int status;
	unsigned int time_to_sleep = 50;
	
	status = WDStart(argc, argv, WD_ID);
	
	while(time_to_sleep > 0)
	{
		printf("time to sleep: %d\n", time_to_sleep);
		time_to_sleep = sleep(time_to_sleep);
	}
	
	WDStop();
	
	return 0;
}
