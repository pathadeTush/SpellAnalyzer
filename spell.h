/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include "prefix.h"

/** function to check ascii value is of small alphabet or not **/
int isSmallAlpha(int ch);

/** function to check ascii value is of large alphabet or not **/
int isLargeAlpha(int ch);

/** function to check end of word **/
int isWordEnd(char ch);

/** function to get minimum of a, b, c **/
int min(int a, int b, int c);

/** function to get editDistance from string s1 and s2  **/
int editDistance(const char *s1, int l1, const char *s2, int l2);

/** function to get suggesstions for misspelled words **/
void getSuggestions(DICT dict, int level, const char *wrongWord, char suggestedWord[], int *maxSuggestions, char suggestions[], int *indexForSuggestions, int EditDistance);

/** function to break the misspelled word by adding spaces at appropriate position **/
int wordSegmentation(const char *misspelledWord, int startIndex, int endIndex, char arr[], int *index, int *found);
