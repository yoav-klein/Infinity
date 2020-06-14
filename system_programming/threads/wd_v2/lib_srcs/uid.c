/*****************************************
*		UID
*		Author: Yoav Klein
*		Reviewer: Yael
*		Date: 13.6.19
******************************************/

#include <unistd.h> /* getpid */

#include "../include/uid.h" /* UIDCreate */

const unid_t bad_uid;

unid_t UIDCreate()
{
	static size_t count = 0;

	unid_t new;
	new.pid = getpid();
	gettimeofday(&new.time, NULL);
	new.counter = __sync_fetch_and_add(&count, 1);
	
	return new;
}

int UIDIsSame(unid_t uid1, unid_t uid2)
{
	return (uid1.pid == uid2.pid && uid1.time.tv_sec == uid2.time.tv_sec && 
	uid1.time.tv_usec == uid2.time.tv_usec && uid1.counter == uid2.counter);
}

int UIDIsBad(unid_t uid)
{
	return  0 == uid.counter && 0 == uid.pid &&
			0 == uid.time.tv_sec && 0 == uid.time.tv_usec;
}

