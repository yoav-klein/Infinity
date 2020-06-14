#include <stddef.h>

/* ADT: Circular buffer */
/* The user needs to create a cbuf_t* in order to use */
typedef struct Cbuf cbuf_t;

/* creates a circular buffer and returns it's address. recives number of bytes */
cbuf_t *CBUFCreate(size_t nbytes);

/* destroyes cbuf */
void CBUFDestroy(cbuf_t *cbuf);

/* writes in cbuf, nbytes from data. returns number of bytes written.  */
/* doesn't override data that was written and wasn't read */
/* behaviour is undefined when data contains less the nbytes */
size_t CBUFWrite(cbuf_t* cbuf, const void *data, size_t nbytes);

/* reading nbytes from cbuf and writes it on data. returns number of bytes read */
/* reads only data that wasn't read before. */
/* behaviour is undefined if data contains less then nbytes */
size_t CBUFRead(cbuf_t* cbuf, void *data, size_t nbytes);

/* returns the capacity of cbuf */
size_t CBUFCapacity(cbuf_t *cbuf);

/* returns 1 if there isn't new data on cbuf */
int CBUFIsEmpty(const cbuf_t *cbuf);

/* returns the number of bytes available for writing */
size_t CBUFFreeSpace(cbuf_t *cbuf);

