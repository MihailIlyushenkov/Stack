#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef int elem_t;

const int base = 31, mod = 1e9+7;

#define DUMP(stk) dump((stk), __FILE__, __func__, __LINE__)
#define STACKINIT(stk, capacity) StackCtor(stk, capacity, #stk, __FILE__, __func__, __LINE__)

enum ErrorType
{
    NoError = 0,
    VoidStack = 1,
    InvalidMemoryInitiallisation = 2,
    VoidStackData = 3,
    SizeAndCapacityOvercrossing = 4,
    DestroyedStack = 5,
    InvalidStackCapacity = 6,
    InvalidHash = 7,
    LeftCanaryDead = 8,
    RightCanaryDead = 9
};


struct stack
{
    void* leftcanary = 0;

    const char* stackname = 0;
    const char* filename = 0;
    const char* funcname = 0;
    size_t line = 0;

    elem_t * data = 0;
    size_t size = 0;
    size_t capacity = 0;

    size_t HashVal = 0;

    void* rightcanary = 0;
};

ErrorType StackCheck(stack * stk);

void ErrorPrint(ErrorType Error)
{
    switch (Error)
    {
    case NoError: printf("This stack has no errors"); break;
    case VoidStack: printf("This stack has VoidStack error (code is %d)", VoidStack); break;
    case InvalidMemoryInitiallisation: printf("This stack has InvalidMemoryInitiallisation error (code is %d)", InvalidMemoryInitiallisation); break;
    case VoidStackData: printf("This stack has VoidStackData error (code is %d)", VoidStackData); break;
    case SizeAndCapacityOvercrossing: printf("This stack has SizeAndCapacityOvercrossing error (code is %d)", SizeAndCapacityOvercrossing); break;
    case DestroyedStack: printf("This is destroyed stack (code is %d)", DestroyedStack);
    case InvalidStackCapacity: printf("This stack has InvalidStackCapacity error (code is %d)", InvalidStackCapacity); break;
    case InvalidHash: printf("This stack has InvalidHash error (code is %d)", InvalidHash); break;
    case LeftCanaryDead: printf("This stack has LeftCanaryDead error (code is %d)", LeftCanaryDead); break;
    case RightCanaryDead: printf("This stack has LeftCanaryDead error (code is %d)", RightCanaryDead); break;

    default: printf("Unexpected input value for ErrorPrint function"); break;
    }
}

size_t HashFunc(stack * stk)
{
    size_t a = 0;
    size_t OldHash = stk->HashVal;

    stk->HashVal = 0;

    size_t NewHash = 0;
    size_t power = base;

    for (char* i = (char*) stk; i < ((char*) stk) + 80; i++)
    {
        NewHash = (NewHash + (*i)*power)%mod;
        power = (power * base)%mod;
    }

    for (char* i = (char*) stk->data; i < ((char*) stk->data) + (stk->capacity)*sizeof(elem_t); i++)
    {
        NewHash = (NewHash + (*i)*power)%mod;
        power = (power * base)%mod;
    }

    stk -> HashVal = OldHash;

    return NewHash;
}

void dump(stack * stk, const char* DumpCallFlile, const char* DumpCallFunction, int DumpCallLine)
{

    if (StackCheck(stk) != 0)
    {
        ErrorPrint(StackCheck(stk));
        return;
    }

    printf("\n\n----------DUMPING STACK----------\n\n");
    printf("Dump was called in file %s, in function %s, on line %d\n", DumpCallFlile, DumpCallFunction, DumpCallLine);

    printf("Dumping stack %s (was created in file %s, in function %s, on line %d)\n\n\n",
                                        stk->stackname, stk->filename, stk->funcname, stk->line);

    printf("stack pointer is %p\n\n", stk);

    printf("(%p) left canary is %p\n\n", &(stk->leftcanary), stk->leftcanary);

    printf("(%p) stackname is %s\n", &(stk->stackname), stk->stackname);
    printf("(%p) filename is %s\n", &(stk->filename), stk->filename);
    printf("(%p) funcname is %s\n", &(stk->funcname), stk->funcname);
    printf("(%p) (creation) line is %d\n\n", &(stk->line), stk->line);

    printf("(%p) data pointer is %p\n", &(stk->data), stk->data);
    printf("(%p) size is %zu\n", &(stk->size), stk->size);
    printf("(%p) capacity is %zu\n\n", &(stk->capacity), stk->capacity);

    printf("(%p) Hash value is %zu\n", &(stk->HashVal), stk->HashVal);

    printf("(%p) right canary is %p\n", &(stk->rightcanary), stk->rightcanary);


    const char * prefix = "*";
    const char * postfix = "";

    for (size_t j = 0; j < stk->capacity; j++)
    {
        if (j >= stk->size)
        {
            prefix= " ";
            postfix = "(void)";
        }

        printf("%s[%zu]%s = %d\n", prefix, j, postfix, *(stk->data + j));
    }
}

ErrorType StackCheck(stack * stk)
{
    if (stk == 0)
    {
        return VoidStack;
    }

    else if (stk->data == 0)
    {
        return VoidStackData;
    }

    else if (stk->HashVal != HashFunc(stk))
    {
        return InvalidHash;
    }

    else if ((stk ->capacity == -1) || (stk->data == 0))
    {
        return DestroyedStack;
    }

    else if (stk->capacity < stk->size)
    {
        return SizeAndCapacityOvercrossing;
    }

    else if (stk->leftcanary != &(stk->leftcanary))
    {
        return LeftCanaryError;
    }

    else if (stk->rightcanary != &(stk->rightcanary))
    {
        return RightCanaryError;
    }

    return NoError;
}

ErrorType StackCtor(stack * stk, size_t capacity, const char* stackname, const char* filename, const char* funcname, int linenumb)
{
    if (stk == 0)
    {
        return VoidStack;
    }

    if (capacity <= 0)
    {
        return InvalidStackCapacity;
    }

    stk -> size = 0;
    stk -> capacity = capacity;

    stk -> data = (elem_t*)calloc(capacity * sizeof(elem_t), 1);

    stk->stackname = stackname;
    stk->filename = filename;
    stk->funcname = funcname;
    stk->line = linenumb;

    if (stk -> data == 0)
    {
        return InvalidMemoryInitiallisation;
    }

    stk->rightcanary = &(stk->rightcanary);
    stk->leftcanary = &(stk->leftcanary);

    stk->HashVal = HashFunc(stk);

    return NoError;
}

ErrorType StackDtor(stack * stk)
{

    if (StackCheck(stk) != 0)
    {
        DUMP(stk);
        return StackCheck(stk);
    }

    free(stk -> data);
    stk -> data = 0;
    stk -> size = 0;
    stk -> capacity = 0;
    return NoError;
}

ErrorType StackPush (stack * stk, elem_t value)
{
    if (StackCheck(stk) != 0)
    {
        DUMP(stk);
        return StackCheck(stk);
    }

    if ( (stk -> capacity) == (stk -> size))
    {

        printf("reallocating stack, old capacity is %zu, new capacity is %zu\n", stk->capacity, (stk->capacity)*2);

        elem_t* newptr = (elem_t*) realloc(stk -> data, (stk->capacity)*2*sizeof(elem_t));
        stk -> data = newptr;
        stk -> capacity = (stk->capacity)*2;

        for (size_t i = stk->size; i < stk->capacity; i++)
        {
            (stk->data)[i] = 0;
        }
    }

    stk->data[stk->size] = value;
    stk->size = stk->size + 1;

    stk->HashVal = HashFunc(stk);

    return NoError;
}

ErrorType StackPop(stack * stk, elem_t* value)
{
    if (StackCheck(stk) != 0)
    {
        DUMP(stk);
        return StackCheck(stk);
    }

    *value = stk->data[stk->size - 1];
    (stk->data)[stk->size - 1] = 0;
    stk->size = stk->size - 1;

    stk->HashVal = HashFunc(stk);

    return NoError;
}

int main()
{
    ErrorType Error = NoError;

    stack MyStack;

    DUMP(0);
//     Error = STACKINIT(&MyStack, 5);
//
//     DUMP(&MyStack);
//
//     for (int i = 0; i < 4; i++)
//     {
//         Error = StackPush(&MyStack, i*i);
//     }
//
//     DUMP(&MyStack);
//
//     int value = 0;
//     while (MyStack.size > 0)
//     {
//         StackPop(&MyStack, &value);
//         printf("Poped value is %d\n", value);
//     }
//     DUMP(&MyStack);
}
