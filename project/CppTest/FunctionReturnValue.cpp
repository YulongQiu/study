#include "FunctionReturnValue.h"

int returnLocalVarable()
{
    int i = 0;
    return i;    // ok, return value is a copy of i
}

int* returnLocalAddress()
{
    int i = 0;
    return &i;      // warning: address of local variable 'i' returned
}

char* returnConstant()
{
    char *arr = "hello,world";
    return arr;        // ok, constant is saved in the Constant space
}
