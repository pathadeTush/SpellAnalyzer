/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/time.h>

static int wordCount = 0;
static int cnt = 0;

/*** path to dictionary words data file ***/
#define DATA "data/words.txt"

/*** path to apostrophe words data file ***/
#define ADATA "data/apostropheWords.txt"

/*** Maximum length possible for word in a dictionary ***/
#define MAX_LENGTH 47

/*** SIZE of next array in ADT ***/
#define SIZE 27

/** max Edit distance **/
#define MAX_EDIT_DISTANCE 2

/** max Suggestions **/
#define MAX_SUGGESTIONS 8

/** maximum characters allowed in a line **/
#define MAX_CHARACTERS_IN_LINE 1500

/*** ADT for Loading Dictioanry Data into Trie Data structre ***/

typedef struct node
{
    /*** Stores letter of a word ***/
    char letter;

    /*** 1 if letter is last letter of word / word is complete' else 0 ***/
    int isWordComplete;

    /*** Array of 27 pointers of type node which stores letter 'a' - 'z': 0 - 25  and 26 for appostrophy '  ***/
    struct node *next[SIZE];
} node;

typedef node *DICT;

/*** Returns new node with given letter ***/
node *getNode(const char Letter);

/*** Loads word into the dictionary tree ***/
void insertWord(DICT *dict, const char *word);

/*** function to load dictionary data into tree ***/
int LoadData(DICT *dict, char *filepath);

/*** function to check word is present in data loaded or not ***/
int checkWord(DICT *dict, const char *word);

/*** Recursive function to unload trie from memory. ***/
void UnloadData(DICT *dict);

/** function to print all words loaded into the trie **/
void traverse(DICT dict, int level, char word[]);