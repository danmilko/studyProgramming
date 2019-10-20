/*
DEF_CMD(name, length, args,
{
    code;
})
*/

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
            StackPush(bin, stk);
            *sizeOfBin -= sizeof(Element_t);
            break;
        default:
            fread(&bin, sizeof(Element_t), 1, rf);
            StackPush(regs[(int)bin], stk);
            *sizeOfBin -= sizeof(Element_t);
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
            *sizeOfBin -= sizeof(Element_t);
            break;
        default:
            fread(&bin, sizeof(Element_t), 1, rf);
            regs[(int)bin] = StackPop(stk);
            *sizeOfBin -= sizeof(Element_t);
            break;
    }
    break;
})

DEF_CMD(ADD, 3, 0,
{
    Element_t a = StackPop(stk);
    Element_t b = StackPop(stk);

    StackPush(a + b, stk);
})

DEF_CMD(SUB, 3, 0,
{
    Element_t a = StackPop(stk);
    Element_t b = StackPop(stk);

    StackPush(a - b, stk);
})

DEF_CMD(MULT, 4, 0,
{
    Element_t a = StackPop(stk);
    Element_t b = StackPop(stk);

    StackPush(a * b, stk);
})

DEF_CMD(DIV, 3, 0,
{
    Element_t a = StackPop(stk);
    Element_t b = StackPop(stk);

    StackPush(b / a, stk);
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
        StackPush(output, stk);
        printf("%.2lf\n", output);
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

DEF_CMD(LABEL, 5, 1,
{

})

DEF_CMD(JUMP, 4, 1,
{
    char showing = 0;
    fread(&showing, 1, 1, rf);
    *sizeOfBin -= 1;
    Element_t bin = {};
    switch (showing)
    {
        case 0:
            fread(&bin, sizeof(Element_t), 1, rf);
            fseek(rf, (int)bin, 0);
            *sizeOfBin = allSize - (int)(bin);
            break;
        default:
            fread(&bin, sizeof(Element_t), 1, rf);
            fseek(rf, regs[(int)bin], 0);
            *sizeOfBin = allSize - (int)(bin);
            break;
    }
})
