/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include "spell.h"

// fucnction to check for a leaf node
int isLeafNode(node *Node)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (Node->next[i])
            return 0;
    }
    return 1;
}

/** functions to get suggestions of words with given prefix if prefix given is valid **/
int getPrefixSuggestions(char *prefixWord)
{
    int len = strlen(prefixWord);
    node *trie = dict;

    for (int i = 0; i < len; i++)
    {
        int index = prefixWord[i] - 'a';

        // if no word in the data set has given prefix
        if (trie->next[index] == NULL)
            return 0;

        trie = trie->next[index];
    }

    int isWord = trie->isWordComplete; // if given prefix is itself a word
    int isLeafnode = isLeafNode(trie); //  if last node of prefix is a leaf node

    // if prefix is itself a word but no other nodes are present below it
    if (isWord && isLeafnode)
    {
        printf("\t\t--> 1: %s\n", prefixWord);
        return 1;
    }

    // if there are nodes present below the last node of given prefix
    int maxSuggestions = 0;
    if (!isLeafnode)
    {
        char *prefix = prefixWord;
        prefixSuggestions(trie, prefix, &maxSuggestions, len);
        return 1;
    }
}

/** recursive function to get words starting with given prefix provided given prefix is valid **/
void prefixSuggestions(node *Node, char prefixWord[], int *maxSuggestions, int index)
{

    // if word with given prefix found
    if (Node->isWordComplete)
    {
        (*maxSuggestions)++;
        prefixWord[index] = '\0';
        // printf("%c\n", prefixWord[index-1]);
        // printf("index %d  ", index);
        printf("\t\t-->%d: %s\n", *maxSuggestions, prefixWord);
    }

    if (isLeafNode(Node))
        return;

    for (int i = 0; i < SIZE; i++)
    {
        // if maxSuggestion limits reached
        if (*maxSuggestions >= MAX_SUGGESTIONS)
            return;

        if (Node->next[i])
        {
            // append character to the prefix word array
            prefixWord[index] = i + 'a';
            // printf("%c\n", prefixWord[index]);

            prefixSuggestions(Node->next[i], prefixWord, maxSuggestions, index + 1);
        }
    }
    return;
}