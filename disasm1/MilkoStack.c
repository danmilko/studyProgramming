#include <stddef.h>

/**
    @brief SET_SAVERS enables/disables the savers
*/
#define SET_SAVERS
/**
    @brief SET_HASHER enables/disables the hash and some functions that are connected with it
*/
#define SET_HASHER

/**
    @brief Checks if stack is ok
*/
#define ASSERT_OK(stk, line)                                                            \
{                                                                                       \
    (stk)->error = StackVerify(stk);                                                      \
    if((stk)->error) { StackDump(stk, line); abort(); }                                   \
}

#ifdef SET_HASHER
/**
    @brief Initialises all stack.
*/
#define STACK_INIT(stk)         \
{                               \
    StackInit(stk);            \
    (stk)->name = #stk;            \
    (stk)->hash = CalcHash(stk);  \
}                               \

#else
#define STACK_INIT(stk)         \
{                               \
    StackInit(&stk);            \
    stk.name = #stk;            \
}                               \

#endif // SET_HASHER

/**
    @file MilkoStack.c
*/

const int Default_Size = 4;
const int Resize_Smaller = 2;
const int PoisonValue = -1;
const int Hysteresis = 2;
const int Saver_Number = 4294295;
const int Hash_Mult = 37;

#define VERIFY_ERROR_DATA 1
#define VERIFY_ERROR_SIZE 2
#define VERIFY_ERROR_CURRENT 3
#define VERIFY_ERROR_HASH 4
#define VERIFY_ERROR_SAVER 5
#define VERIFY_OK 0
#define VERIFY_DESTRUCTED -1

typedef double Element_t;
/**
        @brief This is my stack realisation. It includes some signalization
        (if SET_HASHER or/and SET_SAVERS are defined)
    */
typedef struct
{
    /**
        @brief saver1 is the first saver (if this defence is on)
    */
    #ifdef SET_SAVERS
    int saver1;
    #endif // SET_SAVERS
    /**
        @brief current is the current size of stack
    */
    int current;
    /**
        @brief data is an array of elements
    */
    Element_t* data;
    /**
        @brief size is the size of stack in memory
    */
    int size;
    /**
        @brief name is the name of stack (if it's initialised by StackInit() it's name is "undefined")
    */
    char* name;
    /**
        @brief error is the error number
    */
    int error;
    /**
        @brief hash is the hash of stack, that calculates every operation (if this defence is on)
    */
    #ifdef SET_HASHER
    unsigned int hash;
    #endif // SET_HASHER
    /**
        @brief saver2 is the second saver (if this defence is on)
    */
    #ifdef SET_SAVERS
    int saver2;
    #endif // SET_SAVERS
} Stack_t;

void StackInit(Stack_t* stk);
void StackDestruct(Stack_t* stk);
int StackCheckForResize(Stack_t* stk);
int StackPush(Element_t pushed, Stack_t* stk);
Element_t StackPop(Stack_t *stk);
int StackVerify(Stack_t* stk);
void StackDump(Stack_t* stk, int line);
unsigned int CalcHash(Stack_t* stk);

void StackInit(Stack_t* stk)
{
    /**
        @brief This function initializes the stack, gives it parameters the default values
        @param stk is the pointer on a stack that should be initialized
        @warning This function doesn't set the name of stack and it's hash. Use STACK_INIT() to init all stack
        @see STACK_INIT(Stack_t stk) - initializes all stack.
    */
    assert(stk);
    #ifdef SET_SAVERS
    stk->saver1 = Saver_Number;
    stk->saver2 = Saver_Number;
    #endif // SET_SAVERS
    #ifdef SET_HASHER
    stk->hash = 0;
    #endif // SET_HASHER
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
    /**
        @brief Destructs the stack.
        @param stk is the pointer on a stack that should be destructed
        @warning After the stack is destruted, you should initialize it again. Don't use it without reinit, or you will get an error.

        @see STACK_INIT(Stack_t stk) - initializes all stack.
        @see StackInit() - initializes stack without hash and name.
    */
    ASSERT_OK(stk, __LINE__);
    free(stk->data);
    stk->current = 0;
    stk->size = 0;
    stk->error = VERIFY_DESTRUCTED;
}
int StackCheckForResize(Stack_t* stk)
{
    /**
        @brief Resizes stack if it's neccessary (it checks it by itself).
        @param stk is the pointer on a stack that should be checked.
        @return 1 if it's resized.
        @return 0 if it's the same size.
    */
    ASSERT_OK(stk,  __LINE__);
    if (stk->size == stk->current)
    {
        stk->data = (Element_t*)realloc(stk->data, stk->size * Resize_Smaller * sizeof(Element_t));
        stk->size *= Resize_Smaller;
        #ifdef SET_HASHER
        stk->hash = CalcHash(stk);
        #endif // SET_HASHER
        ASSERT_OK(stk, __LINE__);
        return 1;
    }
    else if (stk->current > 0 && stk->size / Resize_Smaller > stk->current + Hysteresis && stk->size > Default_Size)
    {
        stk->data = (Element_t*)realloc(stk->data, (stk->size / Resize_Smaller) * sizeof(Element_t));
        stk->size /= Resize_Smaller;
        #ifdef SET_HASHER
        stk->hash = CalcHash(stk);
        #endif // SET_HASHER
        ASSERT_OK(stk, __LINE__);
        return 1;
    }
    //stk->current-=100000000;
    return 0;
}
int StackPush(Element_t pushed, Stack_t* stk)
{
    /**
        @brief Pushes an element in stack.
        @param pushed is the element that should be placed in stack
        @param stk is the pointer on a stack
        @warning This function can resize the stack.
        @see StackCheckForResize() - changes the size of stack
    */
    StackCheckForResize(stk);
    stk->data[stk->current] = pushed;
    stk->current++;
    #ifdef SET_HASHER
    stk->hash = CalcHash(stk);
    #endif // SET_HASHER
    ASSERT_OK(stk, __LINE__);
    return 1;
}
Element_t StackPop(Stack_t *stk)
{
    /**
        @brief Gets an element and removes it from stack.
        @param stk is the pointer on a stack
        @warning This function can resize the stack.
        @return Element_t if the stack is not empty
        @return PoisonValue if the stack is empty
        @see StackCheckForResize() - changes the size of stack
    */
    ASSERT_OK(stk, __LINE__);
    Element_t temp = { PoisonValue };
    if (stk->current > 0)
    {
        stk->current;
        int t = stk->current--;
        temp = stk->data[stk->current];
        stk->data[stk->current] = PoisonValue;
        #ifdef SET_HASHER
        stk->hash = CalcHash(stk);
        #endif // SET_HASHER
        StackCheckForResize(stk);
    }
    #ifdef SET_HASHER
    stk->hash = CalcHash(stk);
    #endif // SET_HASHER
    return temp;
}
int StackVerify(Stack_t* stk)
{
    /**
        @brief Checks if the stack is correct.
        @param stk is the pointer on a stack
        @return 0 if everything is ok
        @return ERROR_VALUE if there's an error
        @see ASSERT_OK() - checks the stack using this function
    */
    //printf("%u\n", stk->hash);
    if (stk->size < Default_Size)
    {
        //stk->error = VERIFY_ERROR_SIZE;
        return VERIFY_ERROR_SIZE;
    }
    if (stk->current < 0)
    {
        //stk->error = VERIFY_ERROR_CURRENT;
        return VERIFY_ERROR_CURRENT;
    }
    if (stk->error != 0)
    {
        return stk->error;
    }
    if (stk->data == NULL)
    {
        //stk->error = VERIFY_ERROR_DATA;
        return VERIFY_ERROR_DATA;
    }
    #ifdef SET_SAVERS
    if (stk->saver1 != Saver_Number || stk->saver2 != Saver_Number)
    {
        //stk->error = VERIFY_ERROR_SAVER;
        return VERIFY_ERROR_SAVER;
    }
    #endif // SET_SAVERS
    #ifdef SET_HASHER
    unsigned int old_hash = stk->hash;
    unsigned int new_hash = CalcHash(stk);
    if (old_hash != new_hash)
    {
        stk->hash = new_hash;
        return VERIFY_ERROR_HASH;
    }
    stk->hash = new_hash;
    #endif // SET_HASHER

    return VERIFY_OK;
}
void StackDump(Stack_t* stk, int line)
{
    /**
        @brief Prints the stack state on a screen. It's usually called from ASSERT_OK() if something is wrong, but
        you can use it if you want to see everything in stack
        @param stk is the pointer on a stack
        @param line is the line from where the function was called
        @see ASSERT_OK() - checks the stack using this function
    */
    printf("***************************************\n");
    printf("Line: %d\nStack information:\n"

            #ifdef SET_SAVERS
            "\tsaver1: %u\n"
            #endif // SET_SAVERS

            "\tname: %s\n"
            "\tsize: %d\n"
            "\tcurrent: %d\n"

            #ifdef SET_HASHER
            "\thash: %x\n"
            #endif // SET_HASHER

            #ifdef SET_SAVERS
            "\tsaver2: %u\n"
            #endif // SET_SAVERS

            "\terror: %d ", line,

            #ifdef SET_SAVERS
            stk->saver1,
            #endif // SET_SAVERS

            stk->name, stk->size, stk->current,

            #ifdef SET_HASHER
            stk->hash,
            #endif // SET_HASHER

            #ifdef SET_SAVERS
            stk->saver2,
            #endif // SET_SAVERS
            stk->error);
    switch(stk->error)
    {
        case VERIFY_OK:
            printf("(ok)");
            break;
        case VERIFY_ERROR_DATA:
            printf("(Stack data is null)");
            break;
        case VERIFY_ERROR_SIZE:
            printf("(Stack size is not correct (size < %d))", Default_Size);
            break;
        case VERIFY_ERROR_CURRENT:
            printf("(Current pointer on element < 0)");
            break;
        #ifdef SET_HASHER
        case VERIFY_ERROR_HASH:
            printf("(Hash is incorrect)");
            break;
        #endif // SET_HASHER
        #ifdef SET_SAVERS
        case VERIFY_ERROR_SAVER:
            printf("(Wrong saver value. Normal saver value: %d)", Saver_Number);
            break;
        #endif // SET_SAVERS
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
    printf("---------------------------------------");
    printf("\n");
}
#ifdef SET_HASHER
unsigned int CalcHash(Stack_t* stk)
{
    /**
        @brief Calculates the hash of stack.
        @param stk is the pointer on a stack
        @return the hash (even if it's incorrect, this function used in StackVerify()
        @see StackVerify() - checks the stack using this function
        @warning It's not recommended to use it somewhere not in this file. It could cause some problems if something breaks
        during this function is proceeding.
    */
    int old_hash = stk->hash;
    stk->hash = 0;
    unsigned int res = 1;
    for (char* i = (char*)stk; i <= (char*)(&stk->error) + sizeof(Element_t); i++)
    {
        //printf("%d ", *i);
        res = (res * Hash_Mult) + (*i + 129) % Hash_Mult;
    }
    //printf("\n");
    for (char* i = (char*)stk->data; i <= (char*)stk->data + stk->size; i++)
    {
        //printf("%d ", *i);
        res = (res * Hash_Mult) + (*i + 129) % Hash_Mult;
    }
    stk->hash = old_hash;
    //printf("\n");
    return res;
}
#endif
