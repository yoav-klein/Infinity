typedef struct dll_node *it_t; 

typedef struct dll dll_t;


typedef int (*cmp_f)(const void *data, const void *key, const void *params); 

/* arg (pointer to an argument) is optional, could be NULL */

typedef int (*act_f)(void *data, const void *params);  

/*0 FOR SUCCES */

/* Creates a dll instance, returns dll_t *, null if failed */
dll_t *DLLCreate();

/* Destroys dll instance */
void DLLDestroy(dll_t *dll);

/* returns size of dll instance */
size_t DLLSize(const dll_t *dll);

int DLLIsEmpty(const dll_t *dll); /* 1 For Empty */

/* returns the first element in the list. */
it_t DLLBegin(const dll_t *dll); 

/* returns dummy node */
it_t DLLEnd(const dll_t *dll);

it_t DLLNext(const it_t iter); /* do not pass DLLEnd ("end of dll") as 'iter' */

it_t DLLPrev(const it_t iter); /* do not pass DLLBegin */

int DLLIsSameIter(const it_t iter1,const it_t iter2);

/* returns the data stored in the iterator */
void *DLLGetData(const it_t iter);

/* creates a new node and inserts it after the given iterator iter. 
returns the new iterator. on failure, returns the DLLEnd */
it_t DLLInsert(dll_t *dll, it_t iter, void *data); 

it_t DLLErase(it_t where);  /*returns the node after the one removed*/ 

/* returns DLLEnd on failure */
it_t DLLPushBack(dll_t *dll, void *data);

/* returns DLLEnd on failure */
it_t DLLPushFront(dll_t *dll, void *data);

void DLLPopBack(dll_t *dll);

void DLLPopFront(dll_t *dll);

/* Runs until the action function returns something that's not 0.
returns the last status returned from the action function */
int DLLForEach(it_t from, it_t to, act_f action, void *params);

/* returns to if not found */
it_t DLLFind(it_t from, it_t to, cmp_f compare, const void *key, const void *params);


it_t DLLSpliceBefore(it_t where, it_t from, it_t to);

/*err return val of a func which returns an iter should be the end_iter*/
