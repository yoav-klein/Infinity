#include <stddef.h>

typedef struct stack stack_t;

/* Stack ADT */
/* In order to use the ADT, the user needs to define a pointer to stack_t  */

/* STACKCreate - creates a stack_t instance and returns it's address */
/* The user needs to define a pointer to stack_t, and initialize it with STACKCreate */
stack_t *STACKCreate(size_t n_elements, size_t size_of_element);

/* STACKPush - pushes the value referenced by 'element' to 'stack. returns 0 on success and 1 on failure. the function will push size_of_element bytes from the address pointed by element  */
/* If stack is full, no value will be pushed, and the user will be announced  */
int STACKPush(stack_t *stack, const void* element);

/* STACKPeek - returns the address of the last element in stack  */
/* if stack is empty, will return NULL */
void *STACKPeek(const stack_t *stack);

/* STACKPop - pops the last element in 'stack'. if stack is empty, user will be announced */
void STACKPop(stack_t *stack);

/* STACKSize - returns the number of elements currently in 'stack' */
size_t STACKSize(const stack_t *stack);

/* STACKDestroy - frees the memory on which 'stack' is saved on */
void STACKDestroy(stack_t *stack);
