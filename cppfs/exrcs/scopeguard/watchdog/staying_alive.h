#ifndef __STAYING_ALIVE_H__
#define __STAYING_ALIVE_H__

/************************************************************************
* Project           : WatchDog                      	 	            *
*										                                *
* File Name         : staying_alive.h    								*    	*								                           			    *       
* Version           : 1.0							 					*
************************************************************************/


typedef struct watchdog watchdog_t;

/*
    RETURN: NULL on failure
*/
watchdog_t *KeepMeAlive(const char **argv);

/*
    Args:
        watchdog_key - returned value from KeepMeAlive
*/
void LetMeDie(watchdog_t *watchdog_key);




/*----------------------------------------------------------------------------*/
#endif /* __STAYING_ALIVE_H__ */
