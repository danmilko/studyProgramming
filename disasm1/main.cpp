#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "oneginLibCpp.h"
#include "MilkoStack.c"

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
    char* result = (char*)calloc(32, 1);
    fread(&cmd, 1, 1, rf);

    if (cmd == END)
    {
        sprintf(result, "END");
        *sizeOfBin = 0;
        return result;
    }

    switch (cmd)
    {
        #define DEF_CMD(name, length, number_args, code)\
        case name:                                      \
        {                                               \
            sprintf(result, "%s", #name);                     \
            printf("%d\n", *sizeOfBin);                     \
            (*sizeOfBin)--;                            \
            for (int i = 0; i < number_args; i++)       \
            {                                                                   \
                char showing = 0;                                           \
                Element_t bin = {};                                       \
                fread(&showing, 1, 1, rf);                  \
                if (showing != 1)                                       \
                {                                                       \
                    fread(&bin, sizeof(Element_t), 1, rf);              \
                    sprintf(result, "%s %lf", result, bin);             \
                    (*sizeOfBin) -= sizeof(Element_t) + 1;                        \
                }                                                               \
                else                                                            \
                {                                                               \
                    fread(&bin, sizeof(Element_t), 1, rf);                      \
                    sprintf(result, "%s %cx", result, (char)bin + 97);               \
                    (*sizeOfBin) -= sizeof(Element_t) + 1;                        \
                }                                                          \
            }                                           \
            sprintf(result, "%s\n", result);                      \
            return result;                              \
        }

        #include "CommandDefines.h"

        #undef DEF_CMD
        default:
        {
            printf("ERROR: Unknown command.");
            abort();
        }
    }
/*
    if (cmd < ADD)
    {
        switch(cmd)
        {
            case PUSH:
            {
                Element_t bin = {};
                sprintf(result, "PUSH %d\n", bin);
                *sizeOfBin -= 1 + sizeof(Element_t);
                return result;
            }
        }
    }
    else if (cmd >= ADD && cmd <= END)
    {
        if (cmd == END)
        {
            sprintf(result, "END");
            *sizeOfBin = 0;
            return result;
        }
        sprintf(result, "%s\n", CommandNames[cmd]);
        *sizeOfBin -= 1;
        return result;
    }
    else
    {
        printf("ERROR: Unknown command.");
        abort();
    }
    */
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
