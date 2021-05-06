/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include "dictionary.h"

/** function to check for leaf node **/
int isLeafNode(node *Node);

/** functions to get suggestions of words with given prefix if prefix given is valid **/
int getPrefixSuggestions(char *prefixWord);

/** recursive function to get words starting with given prefix provided given prefix is valid **/
void prefixSuggestions(node *Node, char prefixWord[], int *maxSuggestions, int index);
