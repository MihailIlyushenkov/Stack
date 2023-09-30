#include "StackDef.h"

int main()
{

    ErrorType Error = NoError;

    stack* MyStack;
    MyStack = MakeStack();
    Error = STACKINIT(MyStack, 5);

    DUMP(MyStack);

    for (double i = 0; (i < 15) && (Error == 0); i++)
    {
        Error = StackPush(MyStack, i*i);
    }

    printf("Error is %d\n", Error);
    DUMP(MyStack);

    double a = 0;

    for (int i = 0; (i < 7) && (Error == 0); i++)
    {
        Error = StackPop(MyStack, &a);
        printf("poped value is %lf\n", a);
    }

    DUMP(MyStack);

    return 0;
}

