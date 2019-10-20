#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "oneginLibCpp.h"
#include "commands.h"
#include "MilkoStack.c"

int WorkWithCode(char* result, String* index, int countString, char labels[1024][64]);
char* ConvertIntInBinary(int num);
char* ConvertStringToCode(String str, int* countLine);

int main()
{
    printf("Enter the name of code file (max 1024 symbols) or press ENTER to use default name \"program.mda\": ");
    char nameOfFile[1024] = "program.mda";
    scanf("%[^\n]", nameOfFile);
    getchar();
    long sizeOfBuffer = GetSizeOfFile(nameOfFile) + 1;

    if (sizeOfBuffer < 0)
    {
        SETCONSOLECOLOR(COLOR_RED);
        printf("\tWRONG! CAN'T OPEN THE FILE. FILE PATH: %s\n", nameOfFile);
        RESETCOLOR();
        return -1;
    }

    char* buffer = (char*)calloc(sizeOfBuffer + 1, 1);
    FillBuffer(buffer, sizeOfBuffer, nameOfFile);

    int countStrings = CountN(buffer, sizeOfBuffer);
    String* index = (String*)calloc(countStrings, sizeof(String));
    GetIndexes(index, buffer, sizeOfBuffer, countStrings);
    char* result = (char*)calloc(sizeOfBuffer * 2, 1);
    //buffer = (char*)calloc(sizeOfBuffer, 1);

    char labels[1024][64] = {};

    int totalCount = WorkWithCode(result, index, countStrings, labels);

    printf("Enter the name of result file (max 1024 symbols) or press ENTER to use default name \"program.cbdm\": "); //cbdm - created by Dan Milko
    strcpy(nameOfFile, "program.cbdm");
    scanf("%[^\n]", nameOfFile);
    getchar();

    FILE* wf = fopen(nameOfFile, "wb");
    fwrite(result, sizeof(char), totalCount, wf);
    fclose(wf);

    free(index);
    free(result);
    free(buffer);

    return 0;
}

int WorkWithCode(char* result, String* index, int countStrings, char labels[1024][64])
{
    int totalCount = 0;
    int countLine = 0;
    int currentLabel = 0;
    for (int i = 0; i < countStrings - 1; i++)
    {
        countLine = 0;
        char* cmd = ConvertStringToCode(index[i], &countLine);

        printf("%d\n", countLine);
        if (cmd == nullptr)
        {
            printf("ERROR WHILE COMPILING: UNKNOWN COMMAND OR ARGUMENT. LINE: %d\n", i + 1);
            abort();
        }
        else if (*cmd == (char)END)
        {
            result[totalCount] = *cmd;
            totalCount++;
            free(cmd);
            return totalCount;
        }
        else if (*cmd == (char)LABEL)
        {
            for (int j = 7; j < countLine; j++)
            {
                *(labels[currentLabel] + j) = *(cmd + j);
            }
        }
        else
        {
            for (int j = 0; j < countLine; j++)
            {
                result[totalCount + j] = *(cmd + j);
            }
        }
        free(cmd);
        totalCount += countLine;
    }
    printf("ERROR WHILE COMPILING: NO ENDING.");
    abort();
}

char* ConvertIntInBinary(int num)
{
    char* a = (char*)calloc(4, 1);
    for (int i = 0; i < 4; i++)
    {
        a[i] = (char)((num % 256));
        num /= 256;
    }
    return a;

}
char* ConvertStringToCode(String str, int* countCode)
{
    #define DEF_CMD(name, length, number_args, code) \
    \
        if (strncmp(str.begin, #name, length) == 0)  \
        { \
            *str.end = '\0';                            \
            printf("%s\n", str.begin);                                          \
            if (number_args <= 0)                               \
            {                                               \
                *countCode = 1;                            \
                char* res = (char*)calloc(1, 1);            \
                res[0] = name;                              \
                return res;                                 \
            }                                               \
            else                    \
            {                               \
                char* res = (char*)calloc(1 + (sizeof(Element_t) + 1) * number_args, 1);            \
                int countMove = 0;                                        \
                int countTotalMove = 0;                                 \
                *countCode += 1;                                         \
                res[0] = name;                                             \
                for (int i = 0; i < number_args; i++)                             \
                {                                                               \
                    Element_t temp = {};                                                               \
                    char* reg = (char*)calloc(64, 1);                                        \
                    if (sscanf(str.begin + length + countTotalMove, "%lf%n", &temp, &countMove) == 1)          \
                    {                                                           \
                        printf("%d %.2lf %d\n", i, temp, name);                              \
                        *(res + *countCode) = 0;                                                \
                        memcpy(res + 2 + i * (sizeof(Element_t) + 1), &temp, sizeof(Element_t));            \
                        countTotalMove += countMove + 1;\
                        *countCode += (sizeof(Element_t) + 1); \
                    }                                                               \
                    else if (sscanf(str.begin + length + countTotalMove, "%s", reg) == 1)                \
                    {\
                        printf("%d\n", i);                                 \
                        if (name < JUMP && name != LABEL)                                \
                        {                                               \
                            Element_t index = (double)(reg[0] - 97);                                              \
                            if (index < 0)                                              \
                            {                                                                   \
                                return nullptr;                                                                        \
                            }                                                                       \
                            *(res + *countCode) = 1;                                                \
                            memcpy(res + 2 + i * (sizeof(Element_t) + 1), &index, sizeof(Element_t));\
                            countTotalMove += 3;                                            \
                            *countCode += (sizeof(Element_t) + 1); \
                        }                                                   \
                    }                                                                           \
                    else                                                            \
                    {                                                               \
                        return nullptr;                                                         \
                    }                                                                       \
                }                                                       \
                                                             \
                                                         \
                return res;                                             \
            }                                                           \
        }

    #include "CommandDefines.h"

    #undef DEF_CMD
    return nullptr;
}

