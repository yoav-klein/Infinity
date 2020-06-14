/*****************************
* C-WS6                      *
* CODE BY:     SASHA LIMAREV *   
* REVIEWED BY: YOAV KLEIN    *                          
******************************/ 

#include <stdio.h>  /*  printf */

typedef unsigned int uint_t;

int pow2(unsigned int x, int y); 
int IsPowOfTwoLoop(unsigned int num);
int IsPowOfTwo(uint_t num);
int AddOne(unsigned int num);
int HasThreeOnes(unsigned int num);
void PrintThreeOnes(unsigned int arr[], int arr_len);
void DisplayBits(uint_t *var_ptr);
uint_t ByteMirrorNoLoop(uint_t num);
uint_t ByteMirrorLoop(uint_t num);
int AreSixAndTwoOn(uint_t num);
int AreSixOrTwoOn(uint_t num);
uint_t SwapThreeFive(uint_t num);
int CountSetBitsLoop(uint_t num);
int CountSetBits(uint_t num);
uint_t FirstDevBySixteen(uint_t num);
void DisplayBitsFloat(float num);
void SwapVars(uint_t *var1, uint_t *var2);
int GetBitCount(uint_t num);

int main()
{
	
    float f = 2;
	DisplayBitsFloat(f);
    
     return 0;
}

int pow2(uint_t x, int y)
{   
    if(y > 0)
    {
        return x << y;
    }
    else
    {
        return x >> (-y);
    }
}

int IsPowOfTwoLoop(uint_t num)
{
    uint_t test_against = 1;

    while(test_against > 0)
    {     
        if(num == test_against)
        {
            return 1;
        }  

        test_against = test_against << 1;
    }

    return 0;
}

int IsPowOfTwo(uint_t num)
{
    uint_t res = num & (num - 1);

    if(0 != res)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int AddOne(uint_t num)
{
    uint_t result = 0;
    uint_t mask = 1;
    uint_t xor_count = 0;  
    uint_t i = 0;  

    while(0 != (mask & num))
    {
        mask = mask << 1;
        ++xor_count;
    }

    result = (num | mask);
    mask = 1;

    for(; i < xor_count; ++i)
    {
        result = result ^ mask;
        mask = mask << 1;
    }

    return result;
}

int HasThreeOnes(uint_t num)
{
    int i = 0;
    uint_t mask = 1;

    int one_bit_counter = 0;

    for( ; i < 32; ++i)
    {
        if(mask == (mask & num))
        {
            ++one_bit_counter;
        }

        mask = mask << 1;
    }

    if(3 == one_bit_counter)
    {
        return 1;
    }
  
    return 0;
}

void PrintThreeOnes(uint_t arr[], int arr_len)
{
    int i = 0;
    
    for( ; i < arr_len; ++i)
    {
        if(HasThreeOnes(arr[i]))
        {
            printf("Num: %u, i: %i\n", arr[i], i);
        }
    }   
}

uint_t ByteMirrorNoLoop(uint_t num) /* needssfixing */
{
    uint_t result = 0;
    
    uint_t mask0 = 0xFFFF0000;
    uint_t mask1 = 0xFF00FF00; 
    uint_t mask2 = 0xF0F0F0F0;
    uint_t mask3 = 0xCCCCCCCC; 
    uint_t mask4 = 0xAAAAAAAA;

    result = ((mask0 & num) >> 16) | ((~mask0 & num) << 16);
    result = ((mask1 & result) >> 8) | ((~mask1 & result) << 8);
    result = ((mask2 & result) >> 4) | ((~mask2 & result) << 4);
    result = ((mask3 & result) >> 2) | ((~mask3 & result) << 2);
    result = ((mask4 & result) >> 1) | ((~mask4 & result) << 1);
  
    return result;
}

uint_t ByteMirrorLoop(uint_t num) 
{
    uint_t result = 0;
    uint_t temp1 = 0;
    uint_t temp2 = 0;
    uint_t mask_right = 0x1; /* 00000001 */
    uint_t mask_left = 0x80000000; /*1000.....0000 */

    int i = 0;

    for(; i < 16; ++i)
    {
        temp1 = (num & mask_right) << (31 - (i*2));

        temp2 = (num & mask_left) >> (31 - (i*2));

        result = (result | (temp1 | temp2));

        mask_left = mask_left >> 1;
        mask_right = mask_right << 1;
    }    

    return result;
}

int AreSixAndTwoOn(uint_t num)
{
    uint_t test_num = 0x22; 
    uint_t result = test_num & num;

    return result;
}

int AreSixOrTwoOn(uint_t num)
{
    uint_t test_num = 0x22; 
    uint_t result = test_num | num;

    return result;
}

uint_t SwapThreeFive(uint_t num) /* condense this */
{
    uint_t result = 0;
    uint_t temp = 0;

    uint_t mask_third = 0x4; /* 00000100 */
    uint_t mask_fifth = 0x10; /* 00010000 */
    uint_t mask_rest = 0xEB; /* 11101011 */

    result = num & mask_rest;

    temp = (num & mask_third) << 2 | (num & mask_fifth) >> 2;

    result = result | temp;

    return result;
}

int CountSetBitsLoop(uint_t num)
{
    int result = 0;
    uint_t mask = 0x1;
    int i = 0;

    for(; i < 32; ++i)
    {
        if(1 == (num & mask))
        {
            ++result;
        }

        num = num >> 1;
    }

    return result;
}

int CountSetBits(uint_t num)
{
    uint_t mask = 0xFF; /* 00..011111111 */

    int count1 = GetBitCount(num & mask);
    int count2 = GetBitCount((num & (mask << 8)) >> 8);
    int count3 = GetBitCount((num & (mask << 16)) >> 16);
    int count4 = GetBitCount((num & (mask << 24)) >> 24);
     
    return (count1 + count2 + count3 + count4);
}

uint_t FirstDevBySixteen(uint_t num)
{
   uint_t result = 0;

   int i = 0;

   for(; i < 27; ++i)
   {      
        uint_t mask_left = 0x10 << i;
        uint_t mask_right = 0x10;

        int j = 0;
        
        for(; j < i; ++j)
        {   
            uint_t test = num & mask_left;

            if(0 != test)
            {
                result = test;
            }

            mask_left = mask_left | mask_right;
            mask_right = mask_right << 1;
        }
   }

    return result;
}

void SwapVars(uint_t *var1, uint_t *var2)
{
    *var2 = (*var1) ^ (*var2);
    *var1 = (*var1) ^ (*var2);
    *var2 = (*var1) ^ (*var2);
}

void DisplayBits(uint_t *var_ptr) 
{
    uint_t var = *var_ptr;
	
    uint_t mask = 1;
    uint_t i = 0;

    uint_t var_size = sizeof(uint_t);
   
    mask = mask << ((var_size * 8) - 1);

    for(; i < var_size * 8; ++i)
    {
        uint_t curr_bit = mask & var;

        if(0 == (i % 8) && 0 < i)
        {
            printf("-");
        }
        
        if(0 != curr_bit)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }

        mask = mask >> 1;
    }

    printf("\n");
}    

void DisplayBitsFloat(float num) /* no way to test yet, should work tho */
{
    uint_t *fake_ptr = (uint_t *) &num;

    DisplayBits(fake_ptr);
}   

int GetBitCount(uint_t num)
{
    
    static const uint_t byte_lut[] = 
    {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 
        3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 
        3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 
        5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 
        3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 
        4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 
        5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 
        4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 
        3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 
        5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 
        4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
        6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 
        5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 
        3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 
        4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 
        5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 
        3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 
        5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 
        5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 
        7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 
        4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 
        5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 
        6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 
        6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 
        5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 
        7, 6, 7, 7, 8
    };

    return byte_lut[num];
}
