#ifndef MILKOSTACK_H_INCLUDED
#define MILKOSTACK_H_INCLUDED


#include "MilkoStack.c";

void StackInit(Stack_t* stk);
void StackDestruct(Stack_t* stk);
int StackCheckForResize(Stack_t* stk);
int StackPush(Element_t pushed, Stack_t* stk);
Element_t StackPop(Stack_t *stk);
int StackVerify(Stack_t* stk);
void StackDump(Stack_t* stk, int line);
unsigned int CalcHash(Stack_t* stk);

#endif // MILKOSTACK_H_INCLUDED
