#include <stdio.h>
#include <stdlib.h>
#include "MilkoStack.h"

#define SET_SAVERS
#define SET_HASHER

void TestInterface()
{
    TestPushAndPop();
    TestSavers();
    TestHash();
    printf("There will be an error: \n");
    CrashTestName();
}

void TestPushAndPop()
{
    Stack_t stk = {};
    STACK_INIT(&stk);
    for (int i = 0; i < 10; i++)
    {
        StackPush(i, &stk);
    }
    Element_t tmp = PoisonValue;
    for (int i = 0; i < 10; i++)
    {
        tmp = StackPop(&stk);
        if (tmp == PoisonValue)
        {
            printf("\tFailed. Popping from empty stack that's not empty.\n");
            return;
        }
    }

    tmp = StackPop(&stk);
    if (tmp != PoisonValue)
    {
        printf("\tFailed. Error when popping from empty stack.\n");
        return;
    }

    printf("Pushing and popping: OK\n");
}

void TestSavers()
{
    Stack_t stk = {};
    STACK_INIT(&stk);
    stk.saver1 = 10;
    stk.error = StackVerify(&stk);
    if (stk.error == VERIFY_ERROR_SAVER)
    {
        printf("Saver1: OK\n");
    }
    else
    {
        StackDump(&stk, -1);
        return;
    }
    stk.saver2 = 10;
    stk.error = StackVerify(&stk);
    if (stk.error == VERIFY_ERROR_SAVER)
    {
        printf("Saver2: OK\n");
    }
    else
    {
        StackDump(&stk, -1);
        return;
    }
}

void TestHash()
{
    Stack_t stk = {};
    STACK_INIT(&stk);
    stk.hash = -1;
    stk.error = StackVerify(&stk);
    if (stk.error == VERIFY_ERROR_HASH)
    {
        printf("Hashing: OK\n");
    }
    else
    {
        StackDump(&stk, -1);
    }
}

void TestError()
{
    Stack_t stk = {};
    STACK_INIT(&stk);
    stk.error = 13;
    ASSERT_OK(&stk, __LINE__);
}

void CrashTestData()
{
    Stack_t stk = {};
    STACK_INIT(&stk);
    stk.data = 1;
    ASSERT_OK(&stk, __LINE__);
}

void CrashTestName()
{
    Stack_t stk = {};
    STACK_INIT(&stk);

    StackDump(&stk, __LINE__);

    stk.name = 1;
    ASSERT_OK(&stk, __LINE__);
}

void CrashTestDataValue()
{
    Stack_t stk = {};
    STACK_INIT(&stk);
    stk.data[7] = 7;
    for (int i = 0; i < 7; i++)
    {
        StackPush(i, &stk);
    }
    ASSERT_OK(&stk, __LINE__);
}
