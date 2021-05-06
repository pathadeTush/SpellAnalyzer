/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include "spell.h"

/** function to check ascii value is of small alphabet or not **/
int isSmallAlpha(int ch)
{
    if (ch >= 97 && ch <= 122)
        return 1;
    else
        return 0;
}

/** function to check ascii value is of large alphabet or not **/
int isLargeAlpha(int ch)
{
    if (ch >= 65 && ch <= 90)
        return 1;
    else
        return 0;
}

/** function to check end of word **/
int isWordEnd(char ch)
{
    if (ch != '\'' && !(ch >= 97 && ch <= 122))
        return 1;
    else
        return 0;
}

/** returns minimum of a, b and c **/
int min(int a, int b, int c)
{
    if (a < b)
    {
        if (a < c)
            return a;
        else
            return c;
    }
    else if (b < c)
        return b;
    else
        return c;
}

/** function to get editDistance from string s1 and s2  **/
int editDistance(const char *s1, int l1, const char *s2, int l2)
{

    // table / 2D array to store the results of substrings
    int table[l1 + 1][l2 + 1];

    for (int i = 0; i <= l1; i++)
    {
        for (int j = 0; j <= l2; j++)
        {
            // if first string is empty then to convert string1 to string2 we need to insert all characters of string2 into string1
            // In this case edit distance between them is equal to length of string2
            if (i == 0)
                table[i][j] = j;

            // if second string is empty then to convert string2 to string1 we need to insert all characters of string1 into string2
            // In this case edit distance between them is equal to length of string1
            else if (j == 0)
                table[i][j] = i;

            // if last character of two strings is same then edit distance is same as that of for tabel[i][j]
            else if (s1[i - 1] == s2[j - 1])
                table[i][j] = table[i - 1][j - 1];

            // if last character is different then consider all possibilites i.e, insertion, deletion, and replace
            else
            {
                table[i][j] = 1 + min(table[i][j - 1],    // insertion
                                      table[i - 1][j],    // deletion
                                      table[i - 1][j - 1] // replace
                                  );
            }
        }
    }
    return table[l1][l2];
}

/** function to get suggesstions for misspelled words **/
void getSuggestions(DICT dict, int level, const char *wrongWord, char suggestedWord[], int *maxSuggestions, char suggestions[], int *indexForSuggestions, int EditDistance)
{

    // if word is complete and the difference betn it's length and length of wrong word is atmost maximum edit distance
    if (dict->isWordComplete && abs(level - strlen(wrongWord)) <= EditDistance)
    {
        int k = editDistance(suggestedWord, level, wrongWord, strlen(wrongWord));
        if (k == EditDistance)
        {
            // printf("k %d  level%d  ", k, level);
            (*maxSuggestions)++;
            suggestedWord[level] = '\0';

            int i, len = strlen(suggestedWord);
            // suggestions array stores the all suggested words with there number in that list. \
            // For 2 words abc, abcd suggestions array will look like, \
            suggestions = ['1','a','b','c','|','2','a','b','c','d','|']
            suggestions[(*indexForSuggestions)++] = (*maxSuggestions); // Number of suggested word

            for (i = 1; i <= len; i++)
                suggestions[(*indexForSuggestions)++] = suggestedWord[i - 1];

            suggestions[(*indexForSuggestions)++] = '|'; // used as terminator for word

            printf("\t\t--> %d: %s\n", (*maxSuggestions), suggestedWord);
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (*maxSuggestions > MAX_SUGGESTIONS - 1)
        {
            // printf("maxSuggestions  %d\n", *maxSuggestions);
            return;
        }

        if (level == strlen(wrongWord) + MAX_EDIT_DISTANCE + 1)
            break;

        if (dict->next[i])
        {
            suggestedWord[level] = i + 'a';
            getSuggestions(dict->next[i], level + 1, wrongWord, suggestedWord, maxSuggestions, suggestions, indexForSuggestions, EditDistance);
        }
    }
    return;
}

/** function to break the misspelled word by adding spaces at appropriate position **/
int wordSegmentation(const char *misspelledWord, int startIndex, int endIndex, char arr[], int *index, int *found)
{
    // if word can be segmented to multiple meaningful words
    if (*found == 1)
        return 1;

    // if word is empty (base case)
    if (strlen(misspelledWord) == 0)
        return 1;

    // if we checked for all possibilites and word cannot be segmented
    if (startIndex > endIndex)
        return 0;

    // here len = actual lenth+1 one extra for adding '\0'
    int len = endIndex - startIndex + 2;
    char word[len];
    for (int i = 0; i < len; i++)
        word[i] = misspelledWord[startIndex + i];
    word[len - 1] = '\0';
    // printf("%s\n", word);

    if (checkWord(&dict, word))
    {
        // if subword is found then storing its endindex into an array
        arr[*index] = endIndex;
        (*index)++;
        // printf("%s\n", word);
        // printf("%s  found\n", word);
        return 1;
    }
    else
    {
        // if subword not found and (index != 0)/ arr is not empty then remove the last element entered from an array arr. This element was an endindex of 1st subword which was found but the 2nd subword not found. So removing it from array arr
        if (*found != 0 && *index)
            (*index)--;
        // printf("not found\n");
        if (*found == 0)
        {
            if ((wordSegmentation(misspelledWord, startIndex, endIndex - 1, arr, index, found) && wordSegmentation(misspelledWord, endIndex, strlen(misspelledWord) - 1, arr, index, found)))
            {
                *found = 1;
                return 1;
            }
            else
            {
                *found = 0;
                return 0;
            }
        }
        else
            return 1;
    }
}
