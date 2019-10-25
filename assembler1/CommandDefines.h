/*
DEF_CMD(name, length, args,
{
    code;
})
*/

#define PUSH_ELEM(elem) StackPush(elem, stk);
#define DECREASE_ON_ELEM *sizeOfBin -= sizeof(Element_t);
#define ARITHMETIC(symbol)                          \
{                                                   \
    if (stk->current > 1)                           \
    {                                               \
        Element_t a = StackPop(stk);                \
        Element_t b = StackPop(stk);                \
        Element_t c = a symbol b;                   \
        StackPush(c, stk);                          \
    }                                               \
    else                                            \
    {                                               \
        printf("Error: popping empty stack.\n");    \
    }                                               \
}

#define DO_JUMP                                 \
{                                               \
    Element_t bin = {};                         \
    fseek(rf, 1, 1);                            \
    fread(&bin, 1, sizeof(Element_t), rf);      \
    fseek(rf, (int)bin, 0);                     \
    *sizeOfBin = allSize - (int)(bin);          \
}

#define JUMP_COND(symbol)                   \
{                                           \
    ARITHMETIC(-);                          \
    Element_t diff = StackPop(stk);         \
    if (diff symbol 0)                      \
    {                                       \
        DO_JUMP;                            \
    }                                       \
    else                                    \
    {                                       \
        fseek(rf, sizeof(Element_t) + 1, 1);\
    }                                       \
}

DEF_CMD(PUSH, 4, 1,
{
    char showing = 0;
    fread(&showing, 1, 1, rf);
    *sizeOfBin -= 1;
    Element_t bin = {};
    switch (showing)
    {
        case 0:
            fread(&bin, sizeof(Element_t), 1, rf);
            PUSH_ELEM(bin);
            DECREASE_ON_ELEM;
            break;
        default:
            fread(&bin, sizeof(Element_t), 1, rf);
            PUSH_ELEM(regs[(int)bin]);
            DECREASE_ON_ELEM;
            break;
    }
    break;
})

DEF_CMD(POP, 3, 1,
{
    char showing = 0;
    fread(&showing, 1, 1, rf);
    *sizeOfBin -= 1;
    Element_t bin = {};
    switch (showing)
    {
        case 0:
            fread(&bin, sizeof(Element_t), 1, rf);
            StackPop(stk);
            DECREASE_ON_ELEM;
            break;
        default:
            fread(&bin, sizeof(Element_t), 1, rf);
            regs[(int)bin] = StackPop(stk);
            DECREASE_ON_ELEM;
            break;
    }
    break;
})

DEF_CMD(ADD, 3, 0,
{
    ARITHMETIC(+);
})

DEF_CMD(SUB, 3, 0,
{
    ARITHMETIC(-);
})

DEF_CMD(MULT, 4, 0,
{
    ARITHMETIC(*)
})

DEF_CMD(DIV, 3, 0,
{
    ARITHMETIC(/);
})

DEF_CMD(SCAN, 4, 0,
{
    Element_t read = {};
    scanf("%lf", &read);
    StackPush(read, stk);
})

DEF_CMD(PRINT, 5, 0,
{
    if (stk->current > 0)
    {
        Element_t output = StackPop(stk);
        PUSH_ELEM(output);
        printf("%.2lf\n", output);
    }
    else
    {
        printf("Error: popping empty stack.\n");
    }
})

DEF_CMD(DUMP, 4, 0,
{
    StackDump(stk, __LINE__);
})

DEF_CMD(VIDEO, 5, 1,
{
    int point = (int)StackPop(stk);
    char showing = 0;
    fread(&showing, 1, 1, rf);
    *sizeOfBin -= 1;
    Element_t bin = {};
    switch (showing)
    {
        case 0:
            fread(&bin, sizeof(Element_t), 1, rf);
            video[point] = bin;
            DECREASE_ON_ELEM;
            break;
        default:
            fread(&bin, sizeof(Element_t), 1, rf);
            video[point] = regs[(int)bin];
            DECREASE_ON_ELEM;
            break;
    }
})
/*
help:
BLACK 	    0
RED		    1
GREEN	    2
YELLOW	    3
BLUE	    4
MAGENTA     5
CYAN	    6
WHITE	    7
*/

DEF_CMD(PAINT, 5, 0,
{
    CONSOLE_CLEAR;
    for (int i = 0; i < sizeScreen; i++)
    {
        for (int j = 0; j < sizeScreen; j++)
        {
            int point = j + sizeScreen * i;
            SETCONSOLECOLOR(video[point] + 40);
            printf("  ");
        }
        RESETCOLOR();
        printf("\n");
    }
})

DEF_CMD(SQRT, 4, 0,
{
    Element_t a = StackPop(stk);
    a = sqrt(a);
    PUSH_ELEM(a);
})

DEF_CMD(END, 3, 0,
{
    exit(0);
})

DEF_CMD(LABEL, 5, 1,
{

})

DEF_CMD(JUMP, 4, 1,
{
    DO_JUMP;
})

DEF_CMD(JE, 2, 1,
{
    JUMP_COND(==);
})

DEF_CMD(JBE, 3, 1,
{
    JUMP_COND(>=);
})

DEF_CMD(JSE, 3, 1,
{
    JUMP_COND(<=);
})

DEF_CMD(JB, 2, 1,
{
    JUMP_COND(>);
})

DEF_CMD(JS, 2, 1,
{
    JUMP_COND(<);
})

DEF_CMD(CALL, 4, 1,
{
    Element_t pos = (double)(ftell(rf) + sizeof(Element_t) + 1);
    StackPush(pos, call_stk);
    DO_JUMP;
})

DEF_CMD(RETURN, 6, 0,
{
    int ret_jump = StackPop(call_stk);
    fseek(rf, ret_jump, 0);
    *sizeOfBin = allSize - (int)(ret_jump);
})

