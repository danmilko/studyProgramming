#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "MilkoStack.h"

int main()
{
    Stack_t stack1;
    STACK_INIT(stack1);
    for (int i = -500; i <= -490; ++i)
    {
        StackPush (i, &stack1);
    }


    while (stack1.current != 0)
    {
        printf ("hello\n");
        StackPop (&stack1);
    }
    StackDestruct(&stack1);
    stack1.size = 4;
    stack1.current = 1;
    ASSERT_OK(&stack1, __LINE__);

    return 0;
}
