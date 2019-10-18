/*
DEF_CMD(name, length, args,
{
    code;
})
*/

DEF_CMD(PUSH, 4, 1,
{
    char bin[4] = {};
    fread(bin, 1, sizeof(Element_t), rf);
    int pushed = ConvertBinaryToInt(bin);
    StackPush(pushed, stk);
    *sizeOfBin -= 4;
    break;
})

DEF_CMD(ADD, 3, 0,
{
    int a = StackPop(stk);
    int b = StackPop(stk);

    StackPush(a + b, stk);
})

DEF_CMD(SUB, 3, 0,
{
    int a = StackPop(stk);
    int b = StackPop(stk);

    StackPush(a - b, stk);
})

DEF_CMD(MULT, 4, 0,
{
    int a = StackPop(stk);
    int b = StackPop(stk);

    StackPush(a * b, stk);
})

DEF_CMD(DIV, 3, 0,
{
    int a = StackPop(stk);
    int b = StackPop(stk);

    StackPush(b / a, stk);
})

DEF_CMD(MOD, 3, 0,
{
    int a = StackPop(stk);
    int b = StackPop(stk);

    StackPush(a % b, stk);
})

DEF_CMD(PRINT, 5, 0,
{
    if (stk->current > 0)
    {
        int output = StackPop(stk);
        StackPush(output, stk);
        printf("%d\n", output);
    }
    else
    {
        printf("Error: empty stack.\n");
    }
})

DEF_CMD(DUMP, 4, 0,
{
    StackDump(stk, __LINE__);
})

DEF_CMD(END, 3, 0,
{
    exit(0);
})
