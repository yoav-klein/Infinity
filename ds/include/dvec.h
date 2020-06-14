

typedef struct Dvec dvec_t;

/* Data structure: Dynamic Vector */
/* The user must define an instance of type dvec_t*  in order to use the DS */

/* Creates a dynamic array and returns it's address. */
/* the recives as arguments the size of element and capacity */
dvec_t *DVECCreate(size_t size_of_element,size_t num_elements);

/* Frees the memory on which the array was saved on */
void DVECDestroy(dvec_t *dvec);

/* Returns the address of the indexth element in the array
First element's index is 0 */
void *DVECGetItemAddress(dvec_t *dvec, size_t index);

/* Appends the value that is stored on the address of 'element' to the array.
returns 1 on memory allocation failure */
int DVECPushBack(dvec_t *dvec, const void *element);

/* Pops one elements from the array */
void DVECPopBack(dvec_t *dvec);

/* Retruns the current number of elements in the array */
size_t DVECSize(const dvec_t *dvec);

/* Returns the current capacity of the array */
size_t DVECCapacity(const dvec_t *dvec);

/* Enables the user to set the capacity of the array to new_capacity
new_capacity is a number of elements. 
the function makes sure that the capacity won't be lower then the current size of array
returns 1 on memory allocation failure */
int DVECReserve(dvec_t *dvec, size_t new_capacity);
