#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "oneginLibCpp.h"
#include "commands.h"
#include "MilkoStack.c"

#define ARG_ELEMENT_T 0;
#define ARG_REGISTER 1;
#define ARG_LABEL_JUMP 2;

int WorkWithCode(char* result, String* index, int countString, char labels[1024][32]);
char* ConvertIntInBinary(int num);
char* ConvertStringToCode(String str, int* countLine, int* currentJump);
void LabelsToNumbers(char* result, int totalCount);

char labels[1024][32] = {};
int labelsCount[1024] = {};
char jumps[1024][32] = {};

int main()
{
    //memset(labels, 0, 1024 * 32);
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
    int currentJump = 0;
    //buffer = (char*)calloc(sizeOfBuffer, 1);

    int totalCount = WorkWithCode(result, index, countStrings, labels);

    LabelsToNumbers(result, totalCount);

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

int WorkWithCode(char* result, String* index, int countStrings, char labels[1024][32])
{
    int totalCount = 0;
    int countLine = 0;
    int currentLabel = 0;
    int currentJump = 0;
    for (int i = 0; i < countStrings - 1; i++)
    {
        countLine = 0;
        if (*index[i].begin == '\n')
        {
            continue;
        }
        char* cmd = ConvertStringToCode(index[i], &countLine, &currentJump);

        //printf("%d\n", countLine);
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
            for (int j = 0; j < sizeof(labelsCount) / sizeof(int); j++)
            {
                //printf("%s %s\n", cmd + 1, labels[j]);
                if (strncmp(cmd + 1, labels[j], sizeof(Element_t)) == 0)
                {
                    printf("ERROR WHILE COMPILING: DOUBLE LABEL. LINE: %d\n", i + 1);
                    abort();
                }
            }
            for (int j = 1; j <= strlen(cmd + 1); j++)
            {
                (labels[currentLabel][j - 1]) = *(cmd + j);
                //printf("%c", *(cmd + j));
            }
            labelsCount[currentLabel] = totalCount;
            currentLabel++;
            totalCount--;
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

void LabelsToNumbers(char* result, int totalCount)
{
    for(int i = 0; i < totalCount; i++)
    {
        #define DEF_CMD(name, length, number_args, code)    \
        if (result[i] == (char)name)                        \
        {                                                           \
            if (name < JUMP)                                                        \
            {                                                                   \
                i += number_args * (sizeof(Element_t) + 1);                                 \
            }                                                                   \
            else                                                                    \
            {                                                                       \
                int found = 0;                                                          \
                for (int j = 0; j < 2; j++)                               \
                {                                                                           \
                    found = 0;                                                                            \
                    for (int k = 0; k < 2; k++)                 \
                    {                                                                           \
                        if (!found)                                                                         \
                        {                                                                                   \
                            if (strcmp(jumps[j], labels[k] + 1) == 0 && labels[k] + 1 != "")                    \
                            {                                                                               \
                                Element_t copy = (double)(labelsCount[k]);                                    \
                                memcpy(&result[i + 2], &copy, sizeof(Element_t));                               \
                                found = 1;                                                                  \
                                i += sizeof(Element_t) + 2;                                                     \
                                break;                                                                          \
                            }                                                                                       \
                        }                                                                                       \
                    }                                                                                           \
                }                                                                                                   \
                if (!found)                                                                                     \
                {                                                                                               \
                    printf("ERROR WHILE COMPILING: NO LABEL FOR JUMP. JUMP LABEL: %s\n", jumps[i]);                     \
                    abort();                                                                                    \
                }                                                                                                   \
            }                                                                                                   \
        }

        #include "CommandDefines.h"

        #undef DEF_CMD
    }
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
char* ConvertStringToCode(String str, int* countCode, int* currentJump)
{
    #define DEF_CMD(name, length, number_args, code) \
    \
        if (strncmp(str.begin, #name, length) == 0)  \
        {                                                       \
            *str.end = '\0';                                        \
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
                    {                                                                         \
                        *(res + *countCode) = ARG_ELEMENT_T;                                                \
                        memcpy(res + 2 + i * (sizeof(Element_t) + 1), &temp, sizeof(Element_t));            \
                        countTotalMove += countMove + 1;\
                        *countCode += (sizeof(Element_t) + 1); \
                    }                                                               \
                    else if (sscanf(str.begin + length + countTotalMove, " %s%n", reg, &countMove) == 1)                \
                    {                                \
                        if (name < JUMP && name != LABEL)                                \
                        {                                               \
                            Element_t index = (double)(reg[0] - 97);                                              \
                            if (index < 0 || reg[1] != 'x')                                              \
                            {                                                                   \
                                return nullptr;                                                                        \
                            }                                                                       \
                            *(res + *countCode) = ARG_REGISTER;                                                \
                            memcpy(res + 2 + i * (sizeof(Element_t) + 1), &index, sizeof(Element_t));\
                            countTotalMove += 3;                                            \
                            *countCode += (sizeof(Element_t) + 1); \
                        }                                                   \
                        else if (name == LABEL)                                  \
                        {                                                   \
                            *(res + *countCode) = ARG_LABEL_JUMP;                                                \
                            for (int j = 0; j < sizeof(labelsCount) / sizeof(int); j++)                      \
                            {                                                                           \
                                if (strcmp(reg + 1, labels[j]) == 0)                                                \
                                {                                                                                   \
                                    return nullptr;                                                                 \
                                }                                                                                   \
                            }                                                                                   \
                            strcpy((res + 2), reg);                                               \
                            *(res + 2 + strlen(reg) + 1) = '\0';                        \
                            return res;                                     \
                            *countCode = 0;                                \
                        }                                                       \
                        else if (name >= JUMP)                                  \
                        {                                             \
                            *(res + 1) = ARG_LABEL_JUMP;                                            \
                            memset((res + 2), 0, sizeof(Element_t));                       \
                            strcpy(jumps[*currentJump], reg);                                              \
                            *currentJump += 1;                                                          \
                            *countCode = sizeof(Element_t) + 2;                    \
                            return res;                                     \
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

