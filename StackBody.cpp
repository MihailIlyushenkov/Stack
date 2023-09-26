#include "StackDef.h"

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
    case InvalidStackHash: printf("This stack has InvalidStackHash error (code is %d)", InvalidStackHash); break;
    case InvalidDataHash: printf("This stack has InvalidStackHash error (code is %d)", InvalidDataHash); break;
    case LeftCanaryDead: printf("This stack has LeftCanaryDead error (code is %d)", LeftCanaryDead); break;
    case RightCanaryDead: printf("This stack has RightCanaryDead error (code is %d)", RightCanaryDead); break;

    default: printf("Unexpected input value for ErrorPrint function"); break;
    }
}

size_t GetDataHash(stack * stk)
{
    size_t NewDataHash = 0;

    size_t power = base;

    for (char* i = (char*) stk->data; i < ((char*) stk->data) + (stk->capacity)*sizeof(elem_t);i++)
    {
        NewDataHash = (NewDataHash + (*i)*power)%mod;
        power = (power * base)%mod;
    }

    return NewDataHash;
}

size_t GetStackHash(stack * stk)
{
    size_t OldStackHash = stk->StackHashVal;
    size_t OldDataHash = stk->DataHashVal;

    stk->StackHashVal = 0;
    stk->DataHashVal = 0;

    size_t NewStackHash = 0;

    size_t power = base;

    for (char* i = (char*) stk; i < ((char*) stk) + 80; i++)
    {
        NewStackHash = (NewStackHash + (*i)*power)%mod;
        power = (power * base)%mod;
    }

    stk->StackHashVal = OldStackHash;
    stk->DataHashVal = OldDataHash;

    return NewStackHash;
}

void dump(stack * stk, const char* DumpCallFlile, const char* DumpCallFunction, int DumpCallLine)
{

    printf("\n\n----------DUMPING STACK----------\n\n");

    if (StackCheck(stk) != 0)
    {
        ErrorPrint(StackCheck(stk));
        return;
    }

    printf("Dump was called in file %s, in function %s, on line %d\n", DumpCallFlile, DumpCallFunction, DumpCallLine);

    printf("Dumping stack %s (was created in file %s, in function %s, on line %zu)\n\n\n",
                                        stk->stackname, stk->filename, stk->funcname, stk->line);

    printf("stack pointer is %p\n\n", stk);

    printf("(%p) left canary is %p\n\n", &(stk->leftcanary), stk->leftcanary);

    printf("(%p) stackname is %s\n", &(stk->stackname), stk->stackname);
    printf("(%p) filename is %s\n", &(stk->filename), stk->filename);
    printf("(%p) funcname is %s\n", &(stk->funcname), stk->funcname);
    printf("(%p) (creation) line is %zu\n\n", &(stk->line), stk->line);

    printf("(%p) data pointer is %p\n", &(stk->data), stk->data);
    printf("(%p) size is %zu\n", &(stk->size), stk->size);
    printf("(%p) capacity is %zu\n\n", &(stk->capacity), stk->capacity);

    printf("(%p) Hash (stack) value is %zu\n", &(stk->StackHashVal), stk->StackHashVal);
    printf("(%p) Hash (data) value is %zu\n", &(stk->DataHashVal), stk->DataHashVal);

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

        printf("%s[%zu]%s = %lf\n", prefix, j, postfix, *(stk->data + j));
    }
}

ErrorType StackCheck(stack * stk)
{
    if (stk == 0)
    {
        return VoidStack;
    }

    else if ((stk ->capacity == -1) && (stk->data == 0))
    {
        return DestroyedStack;
    }

    else if (stk->data == 0)
    {
        return VoidStackData;
    }

    else if (stk->StackHashVal != GetStackHash(stk))
    {
        printf("old is %zu, new is %zu\n", stk->StackHashVal, GetStackHash(stk));
        return InvalidStackHash;
    }

    else if (stk->DataHashVal != GetDataHash(stk))
    {
        return InvalidDataHash;
    }

    else if (stk->capacity < stk->size)
    {
        return SizeAndCapacityOvercrossing;
    }

    else if (stk->leftcanary != &(stk->leftcanary))
    {
        return LeftCanaryDead;
    }

    else if (stk->rightcanary != &(stk->rightcanary))
    {
        return RightCanaryDead;
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

    stk->StackHashVal = GetStackHash(stk);

    stk->DataHashVal = GetDataHash(stk);

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

    stk -> leftcanary = 0;

    stk -> stackname = 0;
    stk -> filename = 0;
    stk -> funcname = 0;
    stk -> line = 0;

    stk -> data = 0;
    stk -> size = 0;
    stk -> capacity = 0;

    stk -> StackHashVal = 0;
    stk -> DataHashVal = 0;

    stk -> rightcanary = 0;

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

    stk->StackHashVal = GetStackHash(stk);
    stk->DataHashVal = GetDataHash(stk);

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

    stk->DataHashVal = GetDataHash(stk);
    stk->StackHashVal = GetStackHash(stk);

    return NoError;
}


int main()
{

    ErrorType Error = NoError;

    stack MyStack;

    Error = STACKINIT(&MyStack, 5);

    DUMP(&MyStack);

    for (double i = 0; (i < 15) && (Error == 0); i++)
    {
        Error = StackPush(&MyStack, i*i);
    }

    printf("%d", Error);
    DUMP(&MyStack);

    elem_t value = 0;

    while ((MyStack.size > 0) && (Error != 0))
    {
        Error = StackPop(&MyStack, &value);
        printf("Poped value is %d\n", value);
    }
    DUMP(&MyStack);
}
