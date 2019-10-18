#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "oneginLibCpp.h"
#include "commands.h"

int WorkWithCode(char* result, String* index, int countString);
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

    int totalCount = WorkWithCode(result, index, countStrings);

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

int WorkWithCode(char* result, String* index, int countStrings)
{
    int totalCount = 0;
    int countLine = 0;
    for (int i = 0; i < countStrings - 1; i++)
    {
        char* cmd = ConvertStringToCode(index[i], &countLine);
        if (cmd == nullptr)
        {
            printf("ERROR WHILE COMPILING: UNKNOWN COMMAND. LINE: %d\n", i + 1);
            abort();
        }
        else if (*cmd == (char)END)
        {
            result[totalCount] = *cmd;
            totalCount++;
            free(cmd);
            return totalCount;
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
    #define DEF_CMD( name, length, number_args, code ) \
    \
        if (strncmp(str.begin, #name, length) == 0)  \
        {                                               \
            if (number_args <= 0)                               \
            {                                               \
                char* res = (char*)calloc(1, 1);            \
                res[0] = name;                                 \
                *countCode = 1;                             \
                return res;                                 \
            }                                               \
            else                    \
            {                               \
                char* res = (char*)calloc(5, 1);            \
                *str.end = '\0';                            \
                int arg = atoi(str.begin + length);        \
                char* num = ConvertIntInBinary(arg);                        \
                res[0] = name;                                             \
                for (int i = 0; i < 4; i++)                             \
                {                                                       \
                    res[i + 1] = num[i];                                \
                }                                                       \
                free(num);                                              \
                *countCode = 5;                                         \
                return res;                                             \
            }                                                           \
        }

    #include "CommandDefines.h"

    #undef DEF_CMD
    return nullptr;
}

