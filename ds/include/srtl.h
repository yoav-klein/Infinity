#include <stddef.h> /* size_t */

#include "dll.h" /* it_t */

typedef int (*scmp_f)(const void *data, const void *key, const void *params);

typedef int (*isbefore_f)(const void *data, const void *new_data, const void *params); 

typedef int (*sact_f)(void *data, const void *arg);

typedef struct srtl srtl_t;

typedef it_t sit_t;

/*  creates a sorted list. receives as paramteres a function according
to the scmp_f typedef and parameters for comparison. 
returns a pointer to the new sorted list created */
srtl_t *SrtLCreate(isbefore_f IsBefore, const void *params);

/* receives 'srtl' and destroyes it */
void SrtLDestroy(srtl_t *srtl);

/* receives 'srtl' and returns it's size */
size_t SrtLSize(const srtl_t *srtl);

/*  returns 1 if  srtl is empty */
int SrtLIsEmpty(const srtl_t *srtl); 

/* inserts a new data element 'data' to 'srtl', and returns the iterator inserted */
sit_t SrtLInsert(srtl_t *srtl, void *data);

/* erases an element 'where' and returns the one after it */
sit_t SrtLErase(sit_t where); 

/* erases the first element in srtl */
void SrtLPopFront(srtl_t *srtl);

/* erases the last element in srtl */
void SrtLPopBack(srtl_t *srtl);

/* uses the function 'compare' defined by the user to find the appropriate
iterator from 'from to 'to' according to 'key' and 'params'. if the data
wasn't found, returns 'to' */
sit_t SrtLFind(sit_t from, sit_t to, scmp_f compare,const void *key,const void *params); 

/* operates the function 'action' defined by the user on each data
in iterators from 'from' to 'to'. 'params' is transferred to 'action'
returns the status returned by 'action'  */ 
int SrtLForEach(sit_t from, sit_t to, sact_f action, const void *params);

/* returns the first iterator in 'srtl' */ 
sit_t SrtLBegin(const srtl_t *srtl);

/* returns the last iterator in 'srtl' */
sit_t SrtLEnd(const srtl_t *srtl);

/* returns the next iterator after iter. the user shouldn't pass the last
iterator to this function */
sit_t SrtLNext(const sit_t iter);

/* reutrns the previous iterator before 'iter'. the user shouldn't pass the first
iterator to the function */
sit_t SrtLPrev(const sit_t iter); 

/* returns the data in 'iter' */
void *SrtLGetData(const sit_t iter);

int SrtLIsSame(const sit_t iter1, const sit_t iter2);

/* merges src into dest */
void SrtLMerge(srtl_t *dest, srtl_t *src); /* 2 Lists with the same params */




