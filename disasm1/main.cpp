#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "oneginLibCpp.h"

int ConvertBinaryToInt(char bin[4]);
char* GetTextOfCommand(FILE* rf, int* sizeOfBin);

int main()
{
    printf("Enter the name of executable file (max 1024 symbols) or press ENTER to use default name \"program.cbdm\": ");//cbd - created by Dan Milko
    char nameOfFile[1024] = "program.cbdm";
    scanf("%[^\n]", nameOfFile);
    getchar();
    int sizeOfBin = GetSizeOfFile(nameOfFile);

    char* buffer = (char*)calloc(1024, 1);

    FILE* rf = fopen(nameOfFile, "rb");
    while (sizeOfBin > 0)
    {
        char* insertCmd = GetTextOfCommand(rf, &sizeOfBin);
        buffer = strcat(buffer, insertCmd);
        free(insertCmd);
    }
    fclose(rf);

    printf("Enter the name of result file (max 1024 symbols) or press ENTER to use default name \"program.cbdm\": "); //cbdm - created by Dan Milko
    strcpy(nameOfFile, "program_disasm.mda");
    scanf("%[^\n]", nameOfFile);
    getchar();

    FILE* wf = fopen(nameOfFile, "w");
    fprintf(wf, "%s", buffer);
    fclose(wf);

    free(buffer);
    return 0;
}

char* GetTextOfCommand(FILE* rf, int* sizeOfBin)
{
    char cmd = 0;
    char* result = (char*)calloc(16, 1);
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
                sprintf(result, "PUSH %d\n", pushed);
                *sizeOfBin -= 5;
                return result;
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
                sprintf(result, "PRINT\n");
                *sizeOfBin -= 1;
                return result;
            }
            case DUMP:
                sprintf(result, "DUMP\n");
                *sizeOfBin -= 1;
                return result;
            case END:
                sprintf(result, "END");
                *sizeOfBin = 0;
                return result;
        }
        switch (cmd)
        {
            case ADD:
            {
                sprintf(result, "ADD\n");
                *sizeOfBin -= 1;
                return result;
            }
            case SUB:
            {
                sprintf(result, "SUB\n");
                *sizeOfBin -= 1;
                return result;
            }
            case MULT:
            {
                sprintf(result, "MULT\n");
                *sizeOfBin -= 1;
                return result;
            }
            case DIV:
            {
                sprintf(result, "DIV\n");
                *sizeOfBin -= 1;
                return result;
            }
            case MOD:
            {
                sprintf(result, "MOD\n");
                *sizeOfBin -= 1;
                return result;
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
