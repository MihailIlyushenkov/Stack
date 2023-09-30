#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef double elem_t;

#define DUMP(stk) dump((stk), __FILE__, __func__, __LINE__)

#define STACKINIT(stk, capacity) StackCtor(stk, capacity, #stk, __FILE__, __func__, __LINE__)

const int base = 31, mod = 1e9+7;

enum ErrorType
{
    NoError = 0,
    VoidStack = 1,
    InvalidMemoryInitiallisation = 2,
    VoidStackData = 3,
    SizeAndCapacityOvercrossing = 4,
    DestroyedStack = 5,
    InvalidStackCapacity = 6,
    InvalidDataHash = 7,
    InvalidStackHash = 8,
    LeftCanaryDead = 9,
    RightCanaryDead = 10
};

struct stack;


ErrorType StackCheck(stack * stk);
void dump(stack * stk, const char* DumpCallFlile, const char* DumpCallFunction, int DumpCallLine);


stack* MakeStack(stack * a);

ErrorType StackCtor(stack * stk, size_t capacity, const char* stackname, const char* filename, const char* funcname, int linenumb);
ErrorType StackDtor(stack * stk);
ErrorType StackPush (stack * stk, elem_t value);
ErrorType StackPop(stack * stk, elem_t* value);
