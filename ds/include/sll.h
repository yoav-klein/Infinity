#include <stddef.h>

typedef struct node *it_t; 

typedef struct sll sll_t;

#ifndef CMP_F
#define CMP_F
typedef int (*cmp_f)(const void *data, const void *key);
#endif

#ifndef ACT_F
#define ACT_F
typedef int (*act_f)(void *data, const void *arg);
#endif

/* the user must define an sll_t* variable in order to use the list. 
SLLCreate returns an sll_t *                                */

sll_t *SLLCreate();

/* destroyes the linked list and frees all data */

void SLLDestroy(sll_t *sll);

/* creates a new member to 'sll' which will contain 'data'
and places it after the member 'iter'. 
it's the user's responsibility that 'iter' will be a valid member in 'sll' */

it_t SLLInsertAfter(sll_t *sll, it_t iter, const void *data); 

/* deletes the last member in 'sll'. user's responsibility not to send an empty 'sll' */

void SLLPopBack(sll_t *sll);

/* deletes the first member in 'sll'. user's responsibility not to send an empty 'sll' */
void SLLPopFront(sll_t *sll);

/* creates a new member in 'sll' which will contain 'data', and places it as the last
member in 'sll' */

it_t SLLPushBack(sll_t *sll, const void *data);

/* creates a new member in 'sll' which will contain 'data', and places it as the first
member in 'sll' */

it_t SLLPushFront(sll_t *sll, const void *data);

/* returns the data stored in 'iter'. behaviour is undefined for invalid iterator */
void *SLLGetData(it_t iter);

/* returns the first iterator in 'sll'. user's responsibility not to send an empty 'sll' */

it_t SLLBegin(const sll_t *sll);

/* returns the last iterator in 'sll'. user's responsibility not to send an empty 'sll' */

it_t SLLEnd(const sll_t *sll);

/* returns the next iterator after the iterator 'iter'. user's responsibility to 
send a valid iterator. */

it_t SLLNext(it_t iter);

/* operates the function 'action' on each member in the list, from 'from' to one
before 'to'. 'to' is exclusive. 
user's responbsibility that 'action' will be a function that it's signature
is correspondent to the one defined in the head of this page.
returns the status that is returned from the function supplied by the user
param is a parameter also given by the user, and will be supplied to 'action' */ 

int SLLForEach(it_t from, it_t to, const void *param, act_f action);

/* finds the member in which the compare function compare returns one. 'compare' will be supplied by the function SLLFind with the keyword and the data stored in the members specified by the user as 'from' and 'to'. 'to' is exclusive! if no match was found, the function will return the member 'to'. 
it's the user's responsibility to supply the function with an appropriate compare function as defined in the head of this page. */

it_t SLLFind(it_t from, it_t to, void *key, cmp_f compare);

/* returns 1 if the list 'sll' is empty. 0 otherwise */

int SLLIsEmpty(const sll_t *sll);

/* returns the number of members in 'sll' */

size_t SLLSize(const sll_t *sll);

/* flips the order of 'sll'. it's the user's responsibility that 'sll' 
will contain at least 2 members. */

sll_t *SLLFlip(sll_t *sll);

/* erases the member 'where' from 'sll' */

void SLLErase(sll_t *sll, it_t where); /* erases given iterator from list */

int SLLHasLoop(const sll_t *sll);

it_t SLLFindIntersection(const sll_t *sll1, const sll_t *sll2);



