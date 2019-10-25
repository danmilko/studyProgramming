#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "oneginLibCpp.h"
#include "MilkoStack.c"

int ConvertBinaryToInt(char bin[4]);
char* GetTextOfCommand(FILE* rf, int* sizeOfBin);

const int countLabels = 1024;

int labels[countLabels] = {};
int printedLabel[countLabels] = {};
int currLabel = 0;
int currString = 0;

int main()
{
    printf("Enter the name of executable file (max 1024 symbols) or press ENTER to use default name \"program.cbdm\": ");//cbd - created by Dan Milko
    char nameOfFile[1024] = "program.cbdm";
    scanf("%[^\n]", nameOfFile);
    getchar();
    int sizeOfBin = GetSizeOfFile(nameOfFile);
    int allSize = sizeOfBin;

    char* buffer = (char*)calloc(1024, 1);

    FILE* rf = fopen(nameOfFile, "rb");
    while (sizeOfBin > 0)
    {
        char* insertCmd = GetTextOfCommand(rf, &sizeOfBin);
        free(insertCmd);
        currString++;
    }
    sizeOfBin = allSize;
    currString = 0;
    fseek(rf, 0, 0);
    while (sizeOfBin > 0)
    {
        for (int i = 0; i < currLabel; i++)
        {
            if (labels[i] <= allSize - sizeOfBin && printedLabel[i] == 0)
            {
                char label[15] = {};
                sprintf(label, "LABEL label%d\n", i);
                printedLabel[i] = 1;
                buffer = strcat(buffer, label);
            }
        }
        char* insertCmd = GetTextOfCommand(rf, &sizeOfBin);
        buffer = strcat(buffer, insertCmd);
        free(insertCmd);
        currString++;
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
        #define DEF_CMD(name, length, number_args, code)                            \
        case name:                                                                  \
        {                                                                           \
            sprintf(result, "%s", #name);                                           \
            printf("%d\n", *sizeOfBin);                                             \
            (*sizeOfBin)--;                                                         \
            for (int i = 0; i < number_args; i++)                                   \
            {                                                                       \
                char showing = 0;                                                   \
                Element_t bin = {};                                                 \
                fread(&showing, 1, 1, rf);                                          \
                if (showing == 0)                                                   \
                {                                                                   \
                    fread(&bin, sizeof(Element_t), 1, rf);                          \
                    sprintf(result, "%s %lf", result, bin);                         \
                    (*sizeOfBin) -= sizeof(Element_t) + 1;                          \
                }                                                                   \
                else if (showing == 1)                                              \
                {                                                                   \
                    fread(&bin, sizeof(Element_t), 1, rf);                          \
                    sprintf(result, "%s %cx", result, (char)bin + 97);              \
                    (*sizeOfBin) -= sizeof(Element_t) + 1;                          \
                }                                                                   \
                else if (showing == 2)                                              \
                {                                                                   \
                    fread(&bin, sizeof(Element_t), 1, rf);                          \
                    char label[10] = {};                                            \
                    for (int i = 0; i < currLabel; i++)                             \
                    {                                                               \
                        if (labels[i] == (int)bin)                                  \
                        {                                                           \
                            sprintf(label, "label%d", i);                           \
                            break;                                                  \
                        }                                                           \
                    }                                                               \
                    if (label[0] == '\0')                                           \
                    {                                                               \
                        sprintf(label, "label%d", currLabel);                       \
                        labels[currLabel] = (int)bin;                               \
                        currLabel++;                                                \
                    }                                                               \
                    sprintf(result, "%s %s", result, label);                        \
                    (*sizeOfBin) -= sizeof(Element_t) + 1;                          \
                }                                                                   \
            }                                                                       \
            sprintf(result, "%s\n", result);                                        \
            return result;                                                          \
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
