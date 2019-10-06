#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define SETCONSOLECOLOR(color) { printf("\033[%dm", color); }
#define RESETCOLOR() { printf("\033[0m"); }
#define COLOR_RED 41
#define COLOR_GREEN	32

typedef struct
{
    char* begin;
    char* end;
} String;

const int CountTestsFiles = 3;
const int CountTestsCmp = 3;
const int CountTestsSwap = 5;
const int CountTestsSorts = 1;

int FGetAll(char* str, int cnt, FILE* rf);
int CountN(char* str, int cnt);
long GetSizeOfFile(char* name);
int FillBuffer(char* text, long sizeOfText, char* name);
void GetIndexes(String* index, char* text, int countText, int countStr);
void GetStringByAdresses(char* adress1, char* s);
void Swap(String* adress1, String* adress2);
void BubbleSortText(String* index, int textSize, int (*cmp)(char*, char*, char*, char*));
void OutputText(String* index, int countStrings, char* name);
int CompareFront(char* str1Begin, char* str2Begin, char* str1End, char* str2End);
int CompareBack(char* str1Begin, char* str2Begin, char* str1End, char* str2End);
void UserInterface();
void Test_Swap();
void Test_QuickSort();
void TestInterface();
void Test_WorkWithFile();
void Test_FrontComparing();
void Test_BackComparing();

//working with input file-------------------------------------------------------------------

int FGetAll(char* str, int cnt, FILE* rf)
{
    /**
        @brief That's the function that reads all the text from a file and writes it in a char array.
        @param str the pointer on a char array.
        @param cnt size of the text that you need to read.
        @param rf pointer on a file stream. It should have "r" mode.
    */
    assert(str);
    assert(rf);

    for (int i = 0; i < cnt - 1; i++)
    {
        fscanf(rf, "%c", str + i);
    }
    *(str + cnt) = '\0';
    return 1;
}
int CountN(char* str, int cnt)
{
    /**
        @brief This function counts the number of the strings in a char array.
        @param str the pointer on a char array.
        @param cnt the size of the array.
    */
    assert(str);

    int countN = 0;
    for (char* ptr = str; ptr - str < cnt - 1; ptr++)
    {
        if (*ptr == '\n')
        {
            countN++;
        }
        if (*ptr == '\0')
        {
            break;
        }
    }

    return countN + 1;
}
long GetSizeOfFile(char* name)
{
    /**
        @brief This function opens the file and returns the size of its text.
        @param name the name of the file that you want to read from
    */
    assert(name);

    FILE* f = fopen(name, "r");
    if (f == NULL)
    {
        printf("Can't open the file. Maybe you've typed an incorrect name?\n");
        return -2;
    }

    if (fseek(f, 0, 2) == 0)
    {
        long t = ftell(f);
        fclose(f);
        return t;
    }
    fclose(f);
    return -2;
}
int FillBuffer(char* text, long sizeOfText, char* name)
{
    /**
        @brief This function opens a file and calls FGetAll() to read it.
        @param text the pointer on a char array.
        @param sizeOfText size of the char array.
        @param name the name of the file (wow).
    */
    FILE* f = fopen(name, "r");
    if (f == NULL)
    {
        printf("Can't open the file. Maybe you've typed an incorrect name?\n");
        return -1;
    }
    FGetAll(text, (int)sizeOfText, f);
    fclose(f);
    return 0;
}
void GetIndexes(String* index, char* text, int countText, int countStr)
{
    /**
        @brief This function reads the text array and
        writes the adresses of the beggining
        of every string to the special array.
        @param index the pointer on a pointer array.
        @param text the pointer on a char array.
        @param countText size of the char array that you want to have read.
    */
    index[0].begin = text;
    int j = 0;
    for (int i = 0; i < countText; i++)
    {
        if (*(text + i) == '\n')
        {
            j++;
            index[j - 1].end = (text + i);
            index[j].begin = (text + i + 1);
        }
    }
    if (index[j].begin < &text[countText - 1])
    {
        index[j].end = &text[countText - 1];
    }
    else
    {
        index[j].end = index[j].begin;
    }
}
void GetStringByAdresses(char* adress1, char* s)
{
    /**
        @brief This function reads the string by a pointer
        @param adress1 the pointer on the begging of the string.
        @param s a pointer on a string to write in.
    */
    for (int i = 0; *(adress1 + i) != '\n'; i++)
    {
        s[i] = *(adress1 + i);
        if (*(adress1 + i) == '\n')
        {
            break;
        }
    }
    //printf("%s", s);
}
char* GetStringFromStructure(String s)
{
    int length = s.end - s.begin;
    char* res = (char*)calloc(length, 1);
    for (int i = 0; i <= length; i++)
    {
        res[i] = *(s.begin + i);
    }
    return res;
}

//comparing strings------------------------------------------------------------------------

int CompareFront(char* str1Begin, char* str2Begin, char* str1End, char* str2End)
{
    assert(str1Begin);
    assert(str2Begin);
    assert(str1End);
    assert(str2End);
    for (int k = 0; *(str1Begin + k) != '\n' || *(str2Begin + k) != '\n'; k++)
    {
        if (*(str1Begin + k) == *(str2Begin + k))
        {
            continue;
        }
        if (*(str1Begin + k) > *(str2Begin + k))
        {
            return 1;
        }
        if (*(str1Begin + k) < *(str2Begin + k))
        {
            return -1;
        }
    }
    return 0;
}
int isCorrectSymbol(char c)
{
    if ((c < 'a' || c > 'z') &&
        (c < 'A' || c > 'Z') &&
        (c < '0' || c > '9'))
    {
        return 0;
    }
    return 1;
}
int CompareBack(char* str1Begin, char* str2Begin, char* str1End, char* str2End)
{
    assert(str1Begin);
    assert(str2Begin);
    assert(str1End);
    assert(str2End);
    if (str1Begin >= str1End)
    {
        return 1;
    }
    if (str2Begin >= str2End)
    {
        return -1;
    }
    int step1 = 0;
    int step2 = 0;
    while (*(str1End - step1) == *(str2End - step2) && (str1End - step1) >= str1Begin && (str2End - step2) >= str2Begin)
    {
        step1++;
        step2++;
        while (isCorrectSymbol(*(str1End - step1)) == 0 && (str1End - step1) >= str1Begin)
        {
            step1++;
        }
        while (isCorrectSymbol(*(str2End - step2)) == 0 && (str2End - step2) >= str2Begin)
        {
            step2++;
        }

    }
    if (*(str1End - step1) > *(str2End - step2))
    {
        return 1;
    }
    if (*(str1End - step1) < *(str2End - step2))
    {
        return -1;
    }
    return 0;
}
void Swap(String* adress1, String* adress2)
{
    /**
        @brief Changes the adresses between two pointers.
        @param adress1 the first pointer.
        @param adress2 the second pointer.
    */
    assert(adress1);
    assert(adress2);
    String temp = *adress1;
    *adress1 = *adress2;
    *adress2 = temp;
}

//sorting strings-------------------------------------------------------------------------

void BubbleSortText(String* index, int textSize, int (*cmp)(char*, char*, char*, char*))
{
    /**
        @brief This sorts an array by the bubble sorting (not effective).
        @param index the pointer on a pointer array.
        @param textSize count of the strings (and count of the pointers in the array).
    */
    assert(index);
    for (int i = 0; i < textSize - 1; i++)
    {
        for (int j = 0; j < textSize - 1; j++)
        {
            if (cmp(index[j].begin, index[j + 1].begin, index[j].end, index[j + 1].end) == 1)
            {
                Swap(&index[j], &index[j + 1]);
            }
        }
    }
}
void QuickSort(String* mas, int left, int right, int (*cmp)(char*, char*, char*, char*))
{
    assert(mas);
    int i = left;
    int j = right;
    String pivot;
    pivot.begin = mas[(left + right) / 2].begin;
    pivot.end = mas[(left + right) / 2].end;
    while (i <= j)
    {
        while (cmp(mas[i].begin, pivot.begin, mas[i].end, pivot.end) == -1 && i < right)
        {
              i++;
        }
        while (cmp(mas[j].begin, pivot.begin, mas[j].end, pivot.end) == 1 && left < j)
        {
              j--;
        }
        if (i <= j)
        {
            Swap(&mas[i], &mas[j]);
            i++;
            j--;
        }
    }
    if (left < j)
    {
        QuickSort(mas, left, j, cmp);
    }
    if (i < right)
    {
        QuickSort(mas, i, right, cmp);
    }
}

//writing text in file--------------------------------------------------------------------

void OutputText(String* index, int countStrings, char* name)
{
    /**
        @brief This function writes the sorted (or not, if you want to use it somewhere else :-) ) text in a file.
        @param index the pointer on a pointer array.
        @param countStrings size of the char array that you want to have read.
        @param name thw name of an output file (wow).
    */
    FILE* wf = fopen(name, "a");
	for (int i = 0; i < countStrings; i++)
	{
		for (int j = 0; index[i].begin + j <= index[i].end; j++)
		{
            if (index[i].begin == index[i].end)
            {
                continue;
            }
            fprintf(wf, "%c", *(index[i].begin + j));
		}
	}
    fprintf(wf, "\n");
	fclose(wf);
}

//the name answers everything-------------------------------------------------------------

void UserInterface()
{
    printf("Программа сортирует строки стиха (и просто строки) по алфавиту.\n"
           "Введите имя файла (с расширением, например name.txt) со стихом. Ограничение 50 символов: ");
    char nameOfFile[50] = "";
    scanf("%s", nameOfFile);
    long sizeOfBuffer = GetSizeOfFile(nameOfFile) + 1;

    if (sizeOfBuffer < 0)
    {
        SETCONSOLECOLOR(COLOR_RED);
        printf("\tWRONG! CAN'T OPEN THE FILE. FILE PATH: %s\n", nameOfFile);
        RESETCOLOR();
    }

    char* buffer = (char*)calloc(sizeOfBuffer + 1, 1);
    FillBuffer(buffer, sizeOfBuffer, nameOfFile);
    //buffer[sizeOfBuffer] = '\n';

    int countStrings = CountN(buffer, sizeOfBuffer);
    String* index = (String*)calloc(countStrings, sizeof(String));
    GetIndexes(index, buffer, sizeOfBuffer, countStrings);
    QuickSort(index, 0, countStrings - 1, CompareFront);
    //BubbleSortText(index, countStrings, CompareFront);

    printf("Введите имя файла для записи полученного стиха. Ограничение 50 символов: ");
    scanf("%s", nameOfFile);

    OutputText(index, countStrings, nameOfFile);

    QuickSort(index, 0, countStrings - 1, CompareBack);
    //BubbleSortText(index, countStrings, CompareBack);
    OutputText(index, countStrings, nameOfFile);

    FILE* wf = fopen(nameOfFile, "a");
    fprintf(wf, "%s", buffer);
    fclose(wf);

    free(index);
    free(buffer);
}

//testing functions-----------------------------------------------------------------------

void TestInterface()
{
    printf("Testing work with files:\n");
    Test_WorkWithFile();
    printf("Testing comparing:\n");
    Test_FrontComparing();
    Test_BackComparing();
    printf("Testing swap function:\n");
    Test_Swap();
    printf("Testing quick sort:\n");
    Test_QuickSort();
}

void Test_WorkWithFile()
{
    char* folder = "tests/";
    char* names[32] = { "inputTest1.txt", "inputTest2.txt", "inputTest3.txt" };
    char* correctResults[1024] = { "3 2\n", "9 3\n", "730 24\n" }; //a\n\0 - 3 symbols, 2 strings
    for (int i = 0; i < CountTestsFiles; i++)
    {
        char path[32] = {};
        snprintf(path, 32, "%s%s", folder, names[i]);
        int sizeOfBuffer = GetSizeOfFile(path) + 1;
        if (sizeOfBuffer <= 0)
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\tWRONG! CAN'T OPEN THE FILE. FILE PATH: %s\n", path);
            RESETCOLOR();
            continue;
        }
        char* buffer = (char*)calloc(sizeOfBuffer + 1, 1);
        FillBuffer(buffer, sizeOfBuffer, path);
        int countStrings = CountN(buffer, sizeOfBuffer);
        char res[1024] = "";
        sprintf(res, "%d %d\n", sizeOfBuffer, countStrings);
        if (CompareFront(res, correctResults[i], (char*)1, (char*)1) == 0)
        {
            SETCONSOLECOLOR(COLOR_GREEN);
            printf("\tOK\n");
            RESETCOLOR();
        }
        else
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\tWRONG!\n\tTest result: %s\tShould be: %s", res, correctResults[i]);
            RESETCOLOR();
        }
    }
}

void Test_FrontComparing()
{
    printf("\tTesting front comparing:\n");
    char strings[3][2][32] = {
                            "a\n", "s\n",
                            "a\n", "a\n",
                            "s\n", "a\n"
                        };
    int correctResults[3] = { -1, 0, 1 };
    for (int i = 0; i < CountTestsCmp; i++)
    {
        int res = CompareFront(strings[i][0], strings[i][1], (char*)1, (char*)1);
        char showExtra[1024] = "";
        snprintf(showExtra, 1024, "\t\tTest result: %d\n\t\tShould be: %d\n", res, correctResults[i]);
        if (res == correctResults[i])
        {
            SETCONSOLECOLOR(COLOR_GREEN);
            printf("\t\tOK\n");
            RESETCOLOR();
        }
        else
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\t\tWRONG!\n%s", showExtra);
            RESETCOLOR();
        }
    }
}
void Test_BackComparing()
{
    printf("\tTesting back comparing:\n");
    char strings[3][2][9] = {
                            "abcdes\n", "abcdef\n",
                            "agf;ns\n", "agfns;\n",
                            "aqwsng\n", "bqwsng\n"
                        };
    int correctResults[3] = { 1, 0, -1 };
    for (int i = 0; i < CountTestsCmp; i++)
    {
        int res = CompareBack(strings[i][0], strings[i][1], strings[i][0] + 8, strings[i][1] + 8);
        char showExtra[1024] = "";
        snprintf(showExtra, 1024, "\t\tTest result: %d\n\t\tShould be: %d\n", res, correctResults[i]);
        if (res == correctResults[i])
        {
            SETCONSOLECOLOR(COLOR_GREEN);
            printf("\t\tOK\n");
            RESETCOLOR();
        }
        else
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\t\tWRONG!\n%s", showExtra);
            RESETCOLOR();
        }
    }
}
void Test_Swap()
{
    char* nameOfFile = "tests/swapText.txt";
    long sizeOfBuffer = GetSizeOfFile(nameOfFile) + 1;
    if (sizeOfBuffer < 0)
    {
        SETCONSOLECOLOR(COLOR_RED);
        printf("CAN'T OPEN THE FILE! PATH: %s\n", nameOfFile);
        RESETCOLOR();
        return;
    }
    char* buffer = (char*)calloc(sizeOfBuffer + 1, 1);
    FillBuffer(buffer, sizeOfBuffer, nameOfFile);
    int countStrings = CountN(buffer, sizeOfBuffer);
    String* index = (String*)calloc(countStrings, sizeof(String));
    GetIndexes(index, buffer, sizeOfBuffer, countStrings);

    for (int i = 0; i < CountTestsSwap; i++)
    {
        String first = index[i];
        String second = index[i + 1];
        char showExtra[1024] = "";
        char* strings[4] = {};
        strings[2] = GetStringFromStructure(index[i + 1]);
        strings[3] = GetStringFromStructure(index[i]);
        Swap(&first, &second);
        strings[0] = GetStringFromStructure(first);
        strings[1] = GetStringFromStructure(second);
        snprintf(showExtra, 1024, "\tTest result:\n\t%s\t%s\n\tShould be:\n\t%s\t%s\n", strings[0], strings[1], strings[2], strings[3]);
        if (first.begin == index[i + 1].begin && second.begin == index[i].begin && first.end == index[i + 1].end && second.end == index[i].end)
        {
            SETCONSOLECOLOR(COLOR_GREEN);
            printf("\tOK\n");
            RESETCOLOR();
        }
        else
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\tWRONG!\n%s", showExtra);
            RESETCOLOR();
        }
    }
}

void Test_QuickSort()
{
    char* folder = "tests/";
    char* namesIn[32] = { "sortTest1.in" };
    char* namesOut[32] = { "sortTest1.out" };
    for (int i = 0; i < CountTestsSorts; i++)
    {
        char path[32] = {};
        snprintf(path, 32, "%s%s", folder, namesIn[i]);
        int sizeOfBuffer = GetSizeOfFile(path) + 1;
        if (sizeOfBuffer <= 0)
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\tWRONG! CAN'T OPEN THE FILE. FILE PATH: %s\n", path);
            RESETCOLOR();
            continue;
        }

        char* bufferIn = (char*)calloc(sizeOfBuffer + 1, 1);
        FillBuffer(bufferIn, sizeOfBuffer, path);

        snprintf(path, 32, "%s%s", folder, namesOut[i]);
        sizeOfBuffer = GetSizeOfFile(path) + 1;
        if (sizeOfBuffer <= 0)
        {
            SETCONSOLECOLOR(COLOR_RED);
            printf("\tWRONG! CAN'T OPEN THE FILE. FILE PATH: %s\n", path);
            RESETCOLOR();
            continue;
        }
        char* bufferOut = (char*)calloc(sizeOfBuffer + 1, 1);
        FillBuffer(bufferOut, sizeOfBuffer, path);

        int countStrings = CountN(bufferIn, sizeOfBuffer);
        String* index = (String*)calloc(countStrings, sizeof(String));
        GetIndexes(index, bufferIn, sizeOfBuffer, countStrings);
        QuickSort(index, 0, countStrings - 1, CompareFront);

        String* indexOut = (String*)calloc(countStrings + 1, sizeof(String));
        GetIndexes(indexOut, bufferOut, sizeOfBuffer, countStrings);

        for (int i = 1; i < countStrings; i++)
        {
            if (CompareFront(index[i].begin, indexOut[i].begin, index[i].end, indexOut[i].end) != 0)
            {
                SETCONSOLECOLOR(COLOR_RED);
                printf("\tWRONG!\n");
                RESETCOLOR();
                break;
            }
        }
        SETCONSOLECOLOR(COLOR_GREEN);
        printf("\tOK\n");
        RESETCOLOR();
    }
}
