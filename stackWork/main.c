#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ASSERT_OK(stk, line)                                                  \
{                                                                       \
    int res = StackVerify(stk);                                         \
    if(res) { StackDump(stk, line); abort(); }                                         \
}

#define STACK_INIT(stk) { StackInit(&stk); stk.name = #stk; }

const int Default_Size = 4;
const int Resize_Smaller = 2;
const int PoisonValue = -1;

#define VERIFY_ERROR_DATA 1
#define VERIFY_ERROR_SIZE 2
#define VERIFY_ERROR_CURRENT 3
#define VERIFY_OK 0
#define VERIFY_DESTRUCTED -1

typedef int Element_t;
typedef struct
{
    Element_t* data;
    int current;
    int error;
    int size;
    char* name;
} Stack_t;

void StackInit(Stack_t* stk);
void StackDestruct(Stack_t* stk);
int StackCheckForResize(Stack_t* stk);
int StackPush(Element_t pushed, Stack_t* stk);
Element_t StackPop(Stack_t *stk);
int StackVerify(Stack_t* stk);
void StackDump(Stack_t* stk, int line);

struct
{
    char pinus[1];
    Stack_t* hole;
} StackPenetrator;

int main()
{
    Stack_t stack1;
    STACK_INIT(stack1);
    for (int i = -500; i <= -400; ++i)
    {
        StackPush (i, &stack1);
    }


    while (stack1.current != 0)
    {
        printf ("hello\n");
        StackPop (&stack1);
    }

    return 0;
}

void StackInit(Stack_t* stk)
{
    assert(stk);
    stk->data = (Element_t*)calloc(Default_Size, sizeof(Element_t));
    for (int i = 0; i < Default_Size; i++)
    {
        stk->data[i] = PoisonValue;
    }
    stk->current = 0;
    stk->error = 0;
    stk->size = Default_Size;
    stk->name = "undefined";
}
void StackDestruct(Stack_t* stk)
{
    ASSERT_OK(stk, __LINE__);
    free(stk->data);
    stk->current = 0;
    stk->size = 0;
    stk->error = VERIFY_DESTRUCTED;
}
int StackCheckForResize(Stack_t* stk)
{
    ASSERT_OK(stk,  __LINE__);
    if (stk->size == stk->current)
    {
        stk->data = (Element_t*)realloc(stk->data, stk->size * Resize_Smaller * sizeof(Element_t));
        stk->size *= Resize_Smaller;
        ASSERT_OK(stk, __LINE__);
        return 1;
    }
    else if (stk->current > 0 && stk->size / (stk->current + 1) > Resize_Smaller && stk->size > Default_Size)
    {
        stk->data = (Element_t*)realloc(stk->data, (stk->size / Resize_Smaller) * sizeof(Element_t));
        stk->size /= Resize_Smaller;
        ASSERT_OK(stk, __LINE__);
        return 1;
    }
    return 0;
}
int StackPush(Element_t pushed, Stack_t* stk)
{
    StackCheckForResize(stk);
    stk->data[stk->current] = pushed;
    stk->current++;
    ASSERT_OK(stk, __LINE__);
    return 1;
}
Element_t StackPop(Stack_t *stk)
{
    ASSERT_OK(stk, __LINE__);
    Element_t temp = { PoisonValue };
    if (stk->current > 0)
    {
        stk->current--;
        int t = stk->current;
        temp = stk->data[stk->current];
        stk->data[stk->current] = PoisonValue;
        StackCheckForResize(stk);
        stk->size = 1;
    }
    return temp;
}
int StackVerify(Stack_t* stk)
{
    if (stk->size < 0)
    {
        stk->error = VERIFY_ERROR_SIZE;
        return VERIFY_ERROR_SIZE;
    }
    if (stk->current < 0)
    {
        stk->error = VERIFY_ERROR_CURRENT;
        return VERIFY_ERROR_CURRENT;
    }
    if (stk->error != 0)
    {
        return stk->error;
    }
    if (stk->data == NULL)
    {
        stk->error = VERIFY_ERROR_DATA;
        return VERIFY_ERROR_DATA;
    }
    return VERIFY_OK;
}
void StackDump(Stack_t* stk, int line)
{
    printf("Line: %d\nStack information:\n"
            "\tname: %s\n"
            "\tsize: %d\n"
            "\tcurrent: %d\n"
            "\terror: %d ", line, stk->name, stk->size, stk->current, stk->error);
    switch(stk->error)
    {
        case VERIFY_OK:
            printf("(ok)");
            break;
        case VERIFY_ERROR_DATA:
            printf("(Stack data is null)");
            break;
        case VERIFY_ERROR_SIZE:
            printf("(Stack size is not correct (size < 0))");
            break;
        case VERIFY_ERROR_CURRENT:
            printf("(Current pointer on element < 0)");
            break;
        case VERIFY_DESTRUCTED:
            printf("(This stack is destructed)");
            break;
        default:
            printf("(WRONG ERROR VALUE!)");
    }
    printf("\n");
    printf("Elements:\n");
    for (int i = 0; i < stk->size; i++)
    {
        if (i < stk->current)
        {
            printf("*");
        }
        printf("\t{%d}: %d", i, stk->data[i]);
        if (stk->data[i] == PoisonValue)
        {
            printf("\t[POISON]");
        }
        printf("\n");
    }
    printf("\n");
}

