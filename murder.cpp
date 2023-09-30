#include "StackDef.h"

int main()
{

    ErrorType Error = NoError;

    stack* MyStack;
    MakeStack(MyStack);

    Error = STACKINIT(MyStack, 5);

    DUMP(MyStack);

    for (double i = 0; (i < 15) && (Error == 0); i++)
    {
        Error = StackPush(MyStack, i*i);
    }

    printf("%d", Error);

    //MyStack->capacity = 7;

    DUMP(MyStack);
}
