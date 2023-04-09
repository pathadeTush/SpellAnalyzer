/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include "spell.h"

// Declaring dict tree
DICT dict;

// Microseconds to a second
#define MICRO_TO_S 1000000.0

// The function getrusage and the data type struct rusage are used to examine the resource usage of a process. They are declared in sys/resource.h.
 
struct rusage before, after;

// Benchmarks for autocorrect and dictionary function times
double time_load = 0.0, time_correct = 0.0, time_check = 0.0, time_unload = 0.0;

// Returns number of seconds between b and a. Defined below
double calculate(const struct rusage *b, const struct rusage *a);

// function to display timeDetails. Defined below
void getTimeDetails();

// function to print line contents with highlighting misspelled word. Defined below
void printLine(char LineContents[], int startIndex, int endIndex, int lineCount, int lineIndex);

// stores the total words loaded into the trie data structure here it is 'dict'
int totalWordsLoaded;

// stores the total words present text file.
unsigned long long totalWordsInFile = 0;

// stores the total misspelled words found in provided text file.
unsigned long long misspelledCount = 0;

int main(int argc, char *argv[])
{
    int temp = 0;
    if (argc == 2)
    {
        temp = strlen(argv[1]);
    }
    // Check for correct number of args
    if (
        argc < 2 ||
        argc > 3 ||
        (argc == 2 && !(isSmallAlpha(argv[1][0]) || isLargeAlpha(argv[1][0]))) ||
        (argc == 2 && !(temp >= 5)) ||
        (argc == 2 && !(temp >= 5 && argv[1][--temp] == 't' && argv[1][--temp] == 'x' && argv[1][--temp] == 't' && argv[1][--temp] == '.')) ||
        (argc == 3 && strcmp(argv[1], "-p") && strcmp(argv[1], "-w")))
    {
        printf("\n\t\t*********** Spell Analyzer *************\n\n");
        printf("Usage : ./SpellAnalyzer /path/to/text_file.txt \n");
        printf("Usage : ./SpellAnalyzer -w WordToBeChecked     \n");
        printf("Usage : ./SpellAnalyzer -p prefixWord          \n");

        return 0;
    }

    /*** Loading Data into the tree ***/

    dict = getNode('/'); // '/' represent root of trie data structure

    getrusage(RUSAGE_SELF, &before); // RUSAGE_SELF implies just the current process

    if ((totalWordsLoaded = LoadData(&dict, DATA)) && (totalWordsLoaded = LoadData(&dict, ADATA)))
    {
        getrusage(RUSAGE_SELF, &after);
        // Calculate time to load word data
        time_load = calculate(&before, &after);

        // printf("\tData Loaded Successfully!\n");
        // printf("\tToatal words loaded = %d\n", totalWordsLoaded);
    }
    else
    {
        printf("\tData could not be loaded successfully!\n");
        exit(0);
    }

    /** code for getting prefix word suggestions **/
    if (argc == 3 && !(strcmp(argv[1], "-p")))
    {
        printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~");
        printf("\n\t\t\t\t\t|   Spell Analyzer   |");
        printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~\n");

        char len = strlen(argv[2]);
        for (char i = 0; i < len; i++)
        {
            if (argv[2][i] >= 65 && argv[2][i] <= 90)
                argv[2][i] = argv[2][i] + 32;
        }

        printf("\n\tPrefix Suggestions...\n\n");

        getrusage(RUSAGE_SELF, &before);
        if (getPrefixSuggestions(argv[2]) == 0)
        {
            printf("\tNo suggetsions :(\n\tNo word has prefix '%s'. Please provide valid prefix.\n", argv[2]);
        }
        getrusage(RUSAGE_SELF, &after);
        time_correct = calculate(&before, &after);

        printf("\n");

        getrusage(RUSAGE_SELF, &before);
        UnloadData(&dict);
        getrusage(RUSAGE_SELF, &after);
        // Calculate time to load word data
        time_unload = calculate(&before, &after);

        return 0;
    }

    /** program to check for a word **/
    if (argc == 3 && !(strcmp(argv[1], "-w")))
    {
        printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~");
        printf("\n\t\t\t\t\t|   Spell Analyzer   |");
        printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~\n");

        char len = strlen(argv[2]);
        for (char i = 0; i < len; i++)
        {
            if (argv[2][i] >= 65 && argv[2][i] <= 90)
                argv[2][i] = argv[2][i] + 32;
        }

        getrusage(RUSAGE_SELF, &before);
        int found = checkWord(&dict, argv[2]);
        getrusage(RUSAGE_SELF, &after);
        // Calculate time to load word data
        time_check += calculate(&before, &after);

        if (!found)
        {
            printf("\n\t'%s' is missspelled!\n\n\tsuggestions: \n\n", argv[2]);
            int maxSuggestions = 0, indexForSuggestions = 0, z = 1;
            char suggestedWord[MAX_LENGTH], suggestions[MAX_LENGTH * MAX_SUGGESTIONS];

            getrusage(RUSAGE_SELF, &before);
            while (z <= MAX_EDIT_DISTANCE && maxSuggestions != MAX_SUGGESTIONS)
            {
                getSuggestions(dict, 0, argv[2], suggestedWord, &maxSuggestions, suggestions, &indexForSuggestions, z);
                z++;
            }
            getrusage(RUSAGE_SELF, &after);
            time_correct += calculate(&before, &after);

            if (maxSuggestions == 0)
            {
                char arr[20];                                // it stores the index at which word can be broken into multiple meaningful words
                int i = 0, found = 0, n = 0, canBeSegmented; // found and canBeSegmented are used as a flag. Found is used as a flag for wordSegmentation function (specifically in function) while canBeSegmented is used as a flag to check whether word can be segmented or not
                // i is used as index for array named arr

                getrusage(RUSAGE_SELF, &before);
                canBeSegmented = wordSegmentation(argv[2], 0, strlen(argv[2]) - 1, arr, &i, &found);
                getrusage(RUSAGE_SELF, &after);
                time_correct += calculate(&before, &after);

                if (!canBeSegmented)
                    printf("\tNo Suggestions! :(\n");
                else
                {
                    printf("\t\t--> 1: ");
                    for (char j = 0; j < i; j++)
                    {
                        for (char k = n; k <= arr[j]; k++)
                            printf("%c", argv[2][k]);
                        n = arr[j] + 1;
                        printf(" ");
                    }
                    printf("\n");
                }
            }
            printf("\n");
        }

        else
        {
            printf("\n\t'%s' is spelled correctly! \n\n", argv[2]);
        }

        getrusage(RUSAGE_SELF, &before);
        UnloadData(&dict);
        getrusage(RUSAGE_SELF, &after);
        // Calculate time to load word data
        time_unload = calculate(&before, &after);

        return 0;
    }

    /*
    *
    * program to check given text file
    *
    */

    FILE *textFile, *newFile;
    char *filePath, newFilePath[20], word[MAX_LENGTH] = {'\0'}, LineContents[MAX_CHARACTERS_IN_LINE], isEndOfFile = '\0', lineChar, chr;
    // WrongWordPostions stores the positions of misspelled words in a line
    int index = -1, indexForLineContents = -1, lineCount = 0;

    filePath = argv[1]; //  text file given via command-line argument

    textFile = fopen(filePath, "r");
    int t = 0;
    while (1)
    {
        char c = argv[1][t];
        if (c == '.')
            break;
        newFilePath[t] = c;
        t++;
    }

    newFilePath[t++] = '_';
    newFilePath[t++] = 'n';
    newFilePath[t++] = 'e';
    newFilePath[t++] = 'w';
    newFilePath[t++] = '.';
    newFilePath[t++] = 't';
    newFilePath[t++] = 'x';
    newFilePath[t++] = 't';
    newFilePath[t++] = '\0';

    newFile = fopen(newFilePath, "w+");

    // printf("%s", newFilePath);

    // if file could not opened
    if (textFile == NULL || newFile == NULL)
    {
        printf("\t'%s' could not opened!\n", filePath);
        getrusage(RUSAGE_SELF, &before);
        UnloadData(&dict);
        getrusage(RUSAGE_SELF, &after);
        // Calculate time to load word data
        time_unload = calculate(&before, &after);
        printf("\tData unloaded Successfully!\n");
        getTimeDetails();
        return 0;
    }

    // Code for checking character by character for each line starts from here.
    while (1)
    {
        while (1)
        {
            chr = fgetc(textFile);
            LineContents[++indexForLineContents] = chr;
            if (chr == '\n')
                break;
            if (chr == EOF)
            {
                --indexForLineContents;
                break;
            }
        }
        lineCount++;
        int k = 0, appostropheAtStart = 0, appostropheAtEnd = 0, appostropheInBetn = 0, found = 1;
        while (k <= indexForLineContents)
        {
            lineChar = LineContents[k];
            // printf("%c", lineChar);

            if (lineChar >= 65 && lineChar <= 90)
                lineChar = lineChar + 32;

            if (lineChar == '\'')
            {
                k++;
                char temp_char = LineContents[k];
                // if apostrophe is appeared as a word
                if (index == -1 && isWordEnd(temp_char))
                {
                    k++;
                    continue;
                }
                // if apostrophe is appeared at start of a word
                else if (index == -1 && !isWordEnd(temp_char))
                {
                    // printf("yes\n");
                    appostropheAtStart++;
                    word[++index] = temp_char;
                    k++;
                    continue;
                }
                // if apostrophe is appeared at last of a word
                else if (isWordEnd(temp_char))
                {
                    lineChar = temp_char;
                }
                // if s appeares after apostrophe
                else if (temp_char == 's' || temp_char == 'S')
                {
                    appostropheInBetn += 2;
                    word[++index] == temp_char;
                    k++;
                    continue;
                }
                // if apostrophe appeares in between word
                else if (temp_char >= 97 && temp_char <= 122)
                {
                    word[++index] = lineChar;
                    word[++index] = temp_char;
                    k++;
                    continue;
                }
                else
                {
                    lineChar = temp_char;
                }
            }
            if (lineChar >= 97 && lineChar <= 122)
            {
                word[++index] = lineChar;
                if (index == MAX_LENGTH)
                {
                    UnloadData(&dict);
                    printf("IndexError: Array index out of range (in main function)\n");
                    return 0;
                }
                k++;
                continue;
            }
            // if a end of word is arrived
            if (isWordEnd(lineChar))
            {

                if (index != -1)
                {
                    totalWordsInFile++;

                    int wordLen = strlen(word); // length of a word in a line
                    // if appostrophe is appeared at last
                    while (word[wordLen - 1] == '\'')
                    {
                        appostropheAtEnd++;
                        word[wordLen-- - 1] = '\0';
                    }

                    getrusage(RUSAGE_SELF, &before);
                    found = checkWord(&dict, word);
                    getrusage(RUSAGE_SELF, &after);
                    time_check += calculate(&before, &after);

                    if (!found)
                    {
                        int startIndex = k - wordLen - appostropheAtStart - appostropheAtEnd - appostropheInBetn; // start Index of Misspelled words in a line
                        int endIndex = k - appostropheAtStart - appostropheAtEnd - appostropheInBetn;             // end Index of Misspelled words in a line

                        // line with highlighting misspelled word is printed here
                        printLine(LineContents, startIndex, endIndex, lineCount, indexForLineContents);

                        // printf("\n\t\tmisspelled word: %s \n", word);
                        // printf("\t\tsuggestions for %s\n", word);

                        printf("\n\t----------------------------\n");
                        printf("\t|     Select a choice      |\n");
                        printf("\t----------------------------\n");

                        int maxSuggestions = 0, indexForSuggestions = 0, z = 1;
                        char suggestedWord[MAX_LENGTH],
                            suggestions[MAX_LENGTH * MAX_SUGGESTIONS + MAX_SUGGESTIONS]; // stores the suggested word for 'getSuggestions' function while for 'wordSegmentation' function it stores the endIndex of sub words from which word to be break.

                        getrusage(RUSAGE_SELF, &before);
                        while (z <= MAX_EDIT_DISTANCE && maxSuggestions != MAX_SUGGESTIONS)
                        {
                            getSuggestions(dict, 0, word, suggestedWord, &maxSuggestions, suggestions, &indexForSuggestions, z);
                            z++;
                        }
                        getrusage(RUSAGE_SELF, &after);
                        time_correct += calculate(&before, &after);

                        // if suggestion found from getSuggestions / minimum edit distance approach function
                        if (maxSuggestions)
                            printf("\t\t--> %d: SKIP (No Operation)\n", ++maxSuggestions);

                        int canBeSegmented;
                        // if suggestion not found from getSuggestions / minimum edit distance approach function
                        if (maxSuggestions == 0)
                        {
                            int found = 0, n = 0;

                            getrusage(RUSAGE_SELF, &before);
                            canBeSegmented = wordSegmentation(word, 0, strlen(word) - 1, suggestions, &indexForSuggestions, &found);
                            getrusage(RUSAGE_SELF, &after);
                            time_correct += calculate(&before, &after);

                            // if word can not be segmented i.e, misspelled word cannot be broken down to multiple meaningful words
                            if (!canBeSegmented)
                            {
                                printf("\n\t\tNo Suggestions! :(");
                            }
                            // if word can be segmented
                            else
                            {
                                // sggestions array contains the postion / index from which word has to be broken
                                printf("\t--> 1: ");
                                for (char j = 0; j < indexForSuggestions; j++)
                                {
                                    // printf("%d  n %d\n", arr[j], n);
                                    for (char m = n; m <= suggestions[j]; m++)
                                        printf("%c", word[m]);
                                    n = suggestions[j] + 1;
                                    printf(" ");
                                }
                                printf("\n\t--> 2: SKIP (No Operation)\n");
                            }
                        }

                        int choice;

                        // if suggestions found
                        if (maxSuggestions || canBeSegmented)
                        {
                            while (1)
                            {
                                printf("\n\t\tEnter a choice: ");
                                choice = getc(stdin) - '0';
                                getc(stdin);
                                if (maxSuggestions == 0 && canBeSegmented && (choice > 2 || choice < 1))
                                    printf("\n\tInvalid choice! Please enter a valid choice.\n");
                                else if (maxSuggestions && (choice > maxSuggestions || choice < 1))
                                    printf("\n\tInvalid choice! Please enter a valid choice.\n");
                                else
                                    break;
                            }
                        }

                        // if suggestions found from minimum edit distance approach and choice is other than 'nothing'
                        if (maxSuggestions && choice != maxSuggestions)
                        {

                            char c, EndLineData[MAX_CHARACTERS_IN_LINE]; // EndLineData stores the data present in a line after the end of wrong word
                            int lenOfEndLineData = 0, lenOfChoosedWord = 0, indexOfChoosedWord = 0, startIndexOfWrongWord = startIndex, endIndexOfWrongWord = endIndex;

                            //storing the remaining data from the ending position wrong word in EndLineData array
                            for (int i = endIndexOfWrongWord; i <= indexForLineContents; i++, lenOfEndLineData++)
                                EndLineData[lenOfEndLineData] = LineContents[i];

                            // getting the start index of suggested word from the array of suggested words which is suggestions here
                            for (int i = 0; i < indexForSuggestions; i++)
                            {
                                if (choice == suggestions[i])
                                {
                                    // printf("i %d\n", i);
                                    lenOfChoosedWord = i + 1;
                                    indexOfChoosedWord = i + 1; // starting index of choosed suggested word
                                    break;
                                }
                            }

                            // updating line contents with choosed suggested word
                            while (1)
                            {
                                c = suggestions[indexOfChoosedWord];
                                if (c == '|')
                                    break;
                                LineContents[startIndexOfWrongWord] = c; // adding suggested word in place of wrong word
                                startIndexOfWrongWord++;
                                indexOfChoosedWord++;
                            }
                            lenOfChoosedWord = indexOfChoosedWord - lenOfChoosedWord; // previously lenOfChoosedWord was starting postion of Choosed/suggested word by user. You can see in above for loop. And indexOfChoosedWord is one position ahead of the end position of Choosed/suggested word after completition on above while loop. Now difference of this two variable gives its length.

                            // adding remaining line data at the end after updating line with correct/ suggested word
                            for (int i = 0; i < lenOfEndLineData; i++)
                                LineContents[startIndexOfWrongWord++] = EndLineData[i];

                            indexForLineContents = startIndexOfWrongWord - 1; // at this position startIndexOfWrongWord is one position ahead of end Position of line. Hence startIndexOfWrongWord-1 will be the last index of a line.
                            k = k + lenOfChoosedWord - wordLen;               // updating outer while loop counter
                        }
                        // if no suggested words found but segementation of words can be done
                        else if (canBeSegmented)
                        {
                            if (choice == 1)
                            {
                                char EndLineData[MAX_CHARACTERS_IN_LINE];
                                int lenOfEndLineData = 0, startIndexOfWrongWord = startIndex, endIndexOfWrongWord = endIndex, n = 0;

                                //storing the remaining data after the wrong word in EndLineData
                                for (int i = endIndexOfWrongWord; i <= indexForLineContents; i++, lenOfEndLineData++)
                                    EndLineData[lenOfEndLineData] = LineContents[i];

                                // updating line contents with choosed suggested segmented words
                                for (char i = 0; i < indexForSuggestions; i++)
                                {
                                    for (char j = n; j <= suggestions[i]; j++)
                                        LineContents[startIndexOfWrongWord++] = word[j];

                                    n = suggestions[i] + 1;
                                    LineContents[startIndexOfWrongWord++] = ' ';
                                }

                                startIndexOfWrongWord--; // for extra space added in above loop

                                // startIndexOfWrongWord is one position ahead of the end position of Choosed/suggested word after completition on above for loop.
                                n = startIndexOfWrongWord - startIndex; // length of new / suggested word inserted.

                                // appending end line data to the line
                                for (int i = 0; i < lenOfEndLineData; i++)
                                    LineContents[startIndexOfWrongWord++] = EndLineData[i];

                                indexForLineContents = startIndexOfWrongWord - 1;
                                k = k + n - wordLen; // here indexForSuggestions is equal to length of suggested words by which wrong word has been replaced.
                            }
                        }

                        // giving -1, -1 random index not to highlight any words after updation
                        printLine(LineContents, -1, -1, lineCount, indexForLineContents);
                        printf("\n\nPress 'Enter' key to continue...  ");
                        getchar();
                        system("clear");
                        misspelledCount++;
                    }

                    for (char c = 0; c < wordLen; c++)
                        word[c] = '\0';

                    index = -1;
                    k++;
                    appostropheAtEnd = 0;
                    appostropheAtStart = 0;
                    appostropheInBetn = 0;
                    continue;
                }
                else
                {
                    appostropheAtEnd = 0;
                    appostropheAtStart = 0;
                    appostropheInBetn = 0;
                    k++;
                    continue;
                }
            }
            else
            {
                k++;
                continue;
            }
        }

        for (int i = 0; i <= indexForLineContents; i++)
        {
            if (fputc(LineContents[i], newFile) == EOF)
            {
                UnloadData(&dict);
                printf("Error Occured While writing to the new file!\n");
                exit(0);
            }
        }

        if (chr == EOF)
        {
            // printf("\nEnd Of File\n");
            break;
        }

        indexForLineContents = -1;
    }

    fclose(newFile);

    if (feof(textFile))
    {
        printf("*******************************************************************************\n");
        // printf("\n\n\tText File has been closed successfully!\n");
    }
    else
        printf("\n\n\tSomething went wrong. Text File has been closed before end of file reached!\n");

    getrusage(RUSAGE_SELF, &before);
    UnloadData(&dict);
    getrusage(RUSAGE_SELF, &after);
    time_unload = calculate(&before, &after);

    getTimeDetails();

    printf("\n\tNew file | %s | is created in a same location\n\n\tIt has all correction made! :)\n\n\
    <-------------------------- End Of Operation ----------------------->\n",
           newFilePath);

    return 0;
}

void printLine(char LineContents[], int startIndex, int endIndex, int lineCount, int indexForLineContents)
{
    if (startIndex != -1)
    {
        printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~");
        printf("\n\t\t\t\t\t|   Spell Analyzer   |");
        printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~");
    }

    printf("\n\n\t\t\t\t\t    --------------\n");
    printf("\t\t\t\t\t    |   Line %d   |", lineCount);
    printf("\n\t\t\t\t\t    --------------\n\n");

    if (startIndex == -1)
        printf("\t\t\t\t\tAfter Updation....\n\n");

    int k = 0;
    while (k <= indexForLineContents)
    {
        if (k == startIndex)
            printf(" %c-", 26);
        if (k == endIndex)
            printf("-%c ", 26);

        printf("%c", (LineContents[k] == EOF) ? '\0' : LineContents[k]);
        k++;
    }
}

void getTimeDetails()
{
    printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~");
    printf("\n\t\t\t\t\t|   Spell Analyzer   |");
    printf("\n\t\t\t\t\t ~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n---------------------------------------------------------------------------------------\n\n");
    printf("\tTotal words loaded from dataset : %d\n", totalWordsLoaded);
    printf("\tTotal words in text file        : %lld\n", totalWordsInFile);
    printf("\tTotal misspelled words found    : %lld\n", misspelledCount);
    printf("\tTime taken for DATA LOADING     : %.3f seconds\n", time_load);
    printf("\tTime taken for WORDS CHECKING   : %.3f seconds\n", time_check);
    printf("\tTime taken for WORDS CORRECTION : %.3f seconds\n", time_correct);
    printf("\tTime taken for DATA UNLOADING   : %.3f seconds\n\n", time_unload);
    printf("\tTIME IN TOTAL                   : %.3f seconds\n\n", time_load + time_check + time_unload + time_correct);

    printf("\n---------------------------------------------------------------------------------------\n");

    return;
}

/*
 *
 * Returns number of seconds between b and a.
 * Uses resource usage structures.
 *
 */
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * MICRO_TO_S + a->ru_utime.tv_usec) - (b->ru_utime.tv_sec * MICRO_TO_S + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * MICRO_TO_S + a->ru_stime.tv_usec) - (b->ru_stime.tv_sec * MICRO_TO_S + b->ru_stime.tv_usec))) /
                MICRO_TO_S);
    }
}
// ru_utime This is the total amount of time spent executing in user mode, expressed in a timeval structure (seconds plus microseconds)
// ru_stime This is the total amount of time spent executing in kernel mode, expressed in a timeval structure (seconds plus microseconds)
// .tv_sec gives time present in seconds and tv_usec gives time present in micro seconds.

// In other words, the ru_utime struct returns the amount of time your program has spent doing actual computation, and the ru_stime struct returns the amount of time your program has been waiting for an answer from the kernel when doing disk access, printing to the screen etc.