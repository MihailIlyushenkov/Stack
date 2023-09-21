#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef int elem_t;

enum ErrorType
{
    NoError = 0,
    VoidStack = 1,
    InvalidMemoryInitiallisation = 2,
    VoidStackData = 3,
    SizeAndCapacityOvercrossing = 4,
    DestroyedStack = 5
};

struct stack
{
    elem_t * data = 0;
    size_t size = 0;
    size_t capacity = 0;
};


void dump(stack * stk)
{
    printf("stack pointer is %p\n", stk);
    printf("data pointer is %p\n", stk ->data);
    printf("size is %zu\n", stk->size);
    printf("capacity is %zu\n", stk->capacity);

    // const char * prefix = "*";

    for (size_t j = 0; j < stk->capacity; j++)
    {
        printf("%zu", j);
        printf("[%zu] = %d\n", j, *(stk->data + j));
    }
}

ErrorType StackCheck(stack * stk)
{
    if (stk == 0)
    {
        return VoidStack;
    }

    else if ((stk ->capacity == -1) || (stk->data == 0))
    {
        return DestroyedStack;
    }

    else if (stk->capacity < stk->size)
    {
        return SizeAndCapacityOvercrossing;
    }

    return NoError;
}

ErrorType StackCtor(stack * stk, size_t capacity)
{
    stk -> size = 0;
    stk -> capacity = capacity;
    stk -> data = (elem_t*) calloc(capacity, sizeof(elem_t));

    if (stk -> capacity == InvalidMemoryInitiallisation)
    {
        return InvalidMemoryInitiallisation;
    }
    return NoError;
}

ErrorType StackDtor(stack * stk)
{
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
        printf("dumpinpush\n");
        dump(stk);
        return StackCheck(stk);
    }

    if ( (stk -> capacity) == (stk -> size))
    {

        printf("reallocating stack, old size is %zu, new size is %zu\n", stk->capacity, (stk->capacity)*2);

        elem_t* newptr = (elem_t*) realloc(stk -> data, (stk->capacity)*2*sizeof(elem_t));
        stk -> data = newptr;
        stk -> capacity = (stk->capacity)*2;
    }

    stk->data[stk->size] = value;
    stk->size = stk->size + 1;

    return NoError;
}

ErrorType StackPop(stack * stk, elem_t* value)
{
    if (StackCheck(stk) != 0)
    {
        printf("dumpinpop\n");
        dump(stk);
        return StackCheck(stk);
    }

    *value = stk->data[stk->size - 1];
    stk->size = stk->size - 1;

    return NoError;
}

int main()
{
    ErrorType Error = NoError;

    stack MyStack = {};
    Error = StackCtor(&MyStack, 5);
    printf("%d\n", Error);
    // dump(&MyStack);

    for (int i = 0; i < 10; i++)
    {
        Error = StackPush(&MyStack, i*i);
    }

    int value = 0;
    while (MyStack.size > 0)
    {
        StackPop(&MyStack, &value);
        printf("%d\n", value);
    }
    // dump(&MyStack);
}
