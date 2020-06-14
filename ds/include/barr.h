#define TEST(x) if(1 == x()) { printf(#x " OK!\n\n"); } \
else { printf(#x " Failed!\n\n"); }
#define UNUSED(x) (void)(x)

/* General:  behaviour is undefined for 'index' not between 0 - 63 */

/* BARRSetBit -  sets the bit at the given index in arr to value. behaviour is undefined to value not 0 or 1
return value: size_t, the new array */
size_t BARRSetBit(size_t arr, int index, int value);

/* BARRSetOn -  sets the bit at the given index in arr to 1 
return value: size_t, the new array */
size_t BARRSetOn(size_t arr, int index);

/* BARRSetOff - sets the bit at the given index in arr to 0 
return value: size_t, the new array*/
size_t BARRSetOff(size_t arr, int index);

/* BARRFlip - flips the value of the bit at the given index in arr
return value: size_t, the new array */
size_t BARRFlip(size_t arr, int index);

/* BARRMirror - sets each bit to the value of it's corresponding mirror bit 
return value: size_t, the new array */
size_t BARRMirror(size_t arr);

/* BARRRotateLeft - rotates the array arr 'amount' times left. undefined behaviour for negative value of 'amount'
return value: size_t, the new array */
size_t BARRRotateLeft(size_t arr, size_t amount);

/* BARRRotateRight - rotates the array arr 'amount' times right. undefined behaviour for negative value of 'amount.
return value: size_t, the new array. */
size_t BARRRotateRight(size_t arr, size_t amount);

/* BARRIsOn: returns 1 if the bit at the given index in arr is on, 0 otherwise. */
int BARRIsOn(size_t arr, int index);

/* BARRIsOff: returns 1 if the bit at the given index in arr is off, 0 otherwise. */
int BARRIsOff(size_t arr, int index);

/* BARRCountOn - return the number of set bits in arr. */
size_t BARRCountOn(size_t arr);

/* BARRCountOff - returns the number of unset bits in arr. */
size_t BARRCountOff(size_t arr);

size_t BARRMirrorLUT(size_t arr);

size_t BARRCountOnLUT(size_t arr);

