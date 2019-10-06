#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "oneginLibCpp.h"
#include "MilkoStack.h"

int ConvertBinaryToInt(char bin[4]);
void ExecuteCommand(FILE* rf, Stack_t* stk, int* sizeOfBin);

int main()
{
    printf("Enter the name of executable file (max 1024 symbols) or press ENTER to use default name \"program.cbdm\": ");//cbd - created by Dan Milko
    char nameOfFile[1024] = "program.cbdm";
    scanf("%[^\n]", nameOfFile);
    getchar();
    int sizeOfBin = GetSizeOfFile(nameOfFile);

    Stack_t stk;
    STACK_INIT(&stk);

    FILE* rf = fopen(nameOfFile, "rb");
    for (int i = 0; i < sizeOfBin; i++)
    {
        ExecuteCommand(rf, &stk, &sizeOfBin);
    }
    fclose(rf);
    return 0;
}

void ExecuteCommand(FILE* rf, Stack_t* stk, int* sizeOfBin)
{
    char cmd = 0;
    fread(&cmd, 1, 1, rf);
    if (cmd < ADD)
    {
        switch(cmd)
        {
            case PUSH:
            {
                char bin[4] = {};
                fread(bin, 1, 4, rf);
                int pushed = ConvertBinaryToInt(bin);
                StackPush(pushed, stk);
                *sizeOfBin -= 4;
                break;
            }
            default:
                printf("ERROR: Unknown command.");
                abort();
        }
    }
    else
    {
        switch (cmd)
        {
            case PRINT:
            {
                int output = StackPop(stk);
                StackPush(output, stk);
                printf("%d\n", output);
                return;
            }
            case DUMP:
                StackDump(stk, __LINE__);
                return;
            case END:
                exit(0);

        }
        int a = StackPop(stk);
        int b = StackPop(stk);
        switch (cmd)
        {
            case ADD:
            {
                StackPush(a + b, stk);
                return;
            }
            case SUB:
            {
                StackPush(a - b, stk);
                return;
            }
            case MULT:
            {
                StackPush(a * b, stk);
                return;
            }
            case DIV:
            {
                StackPush(b / a, stk);
                return;
            }
            case MOD:
            {
                StackPush(b % a, stk);
                return;
            }
            default:
                printf("ERROR: Unknown command.");
                abort();
        }
    }
}

int ConvertBinaryToInt(char bin[4])
{
    int mult = 1;
    int res = 0;
    for (int i = 0; i < 4; i++)
    {
        res = res + mult * (int)bin[i];
        mult *= 256;
    }
    return res;
}
