#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "commands.h"
#include "oneginLibCpp.h"
#include "MilkoStack.h"
#include <unistd.h>

#define CONSOLE_CLEAR printf("%s", "\x1B[2J\x1B[H");

int ConvertBinaryToInt(char bin[4]);
void ExecuteCommand(FILE* rf, Stack_t* stk, int* sizeOfBin, int allSize, Stack_t* call_stk);

const int sizeScreen = 32;

Element_t regs[26] = {};

int video[sizeScreen * sizeScreen] = {};

int main()
{
    printf("Enter the name of executable file (max 1024 symbols) or press ENTER to use default name \"program.cbdm\": ");//cbd - created by Dan Milko
    char nameOfFile[1024] = "program.cbdm";
    scanf("%[^\n]", nameOfFile);
    getchar();
    int sizeOfBin = GetSizeOfFile(nameOfFile);

    Stack_t stk;
    STACK_INIT(&stk);

    Stack_t call_stk;
    STACK_INIT(&call_stk);

    FILE* rf = fopen(nameOfFile, "rb");
    int allSize = sizeOfBin;
    while (sizeOfBin > 0)
    {
        ExecuteCommand(rf, &stk, &sizeOfBin, allSize, &call_stk);
    }
    fclose(rf);
    return 0;
}

void ExecuteCommand(FILE* rf, Stack_t* stk, int* sizeOfBin, int allSize, Stack_t* call_stk)
{
    char cmd = 0;
    //printf("Current position: %d\n", ftell(rf));
    fread(&cmd, 1, 1, rf);
    *sizeOfBin -= 1;
    switch(cmd)
    {
    #define DEF_CMD(name, length, number_args, code)        \
        case name:                                          \
        {                                                   \
            code                                            \
            return;                                         \
        }

        #include "CommandDefines.h"

        #undef DEF_CMD
        default:
        {
            printf("ERROR: Unknown command.");
            abort();
        }
    }
}

//unused
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
