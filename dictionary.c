/*
 *
 *   NAME  : TUSHAR CHAKRADHAR PATHADE
 *   MIS   : 111903114 
 *   DIV   : 2
 *   BATCH : S2 
 * 
 */

#include "spell.h"

/*** Returns new node with given letter ***/
node *getNode(const char Letter)
{
    node *nn = (node *)malloc(sizeof(node));

    if (!nn)
    {
        printf("\tOut of Memory. Memory not allocated! (in getNode function) \n");
        exit(0);
    }

    nn->letter = Letter;
    nn->isWordComplete = 0;

    /*****  Initializing each element of array to NULL  *****/
    for (char i = 0; i < SIZE; i++)
        nn->next[i] = NULL;

    return nn;
}

/*** Loads word into the dictionary tree ***/
void insertWord(DICT *dict, const char *word)
{

    node *current = *dict;
    int len = strlen(word), indexForNext;

    // printf("%s\n", word);

    char Letter;

    for (int i = 0; i < len; i++)
    {
        Letter = word[i]; // letter at index i in the word to be iserted

        if (Letter == '\'') // if letter is appostrophy
            indexForNext = 26;

        else
            indexForNext = Letter - 'a'; // index for alphabetical character w.r.to. char 'a'

        if (current->next[indexForNext] == NULL)
            current->next[indexForNext] = getNode(Letter);

        current = current->next[indexForNext];
        // printf("%c", current->letter);
    }

    current->isWordComplete = 1;
    current = NULL;

    // wordCount++;
    return;
}

/*** function to load dictionary data into tree ***/
int LoadData(DICT *dict, char *filepath)
{
    FILE *fp;

    fp = fopen(filepath, "r");

    if (fp == NULL)
    {
        printf("\t%s File could not be opened!\n", filepath);
        exit(0);
    }

    // printf("   File was opened successfully!\n");

    // printf("%c\n", dict->letter);
    node *p = *dict;
    char word_string[MAX_LENGTH] = {'\0'};

    while ((fscanf(fp, "%s", word_string)) != EOF)
    {
        // printf("%s\n", word_string);
        insertWord(&p, word_string);
        wordCount++;
        // printf("%s\n", word_string);
    }

    if (feof(fp))
    {
        // printf("   File has been closed successfully!\n");
    }
    else
    {
        printf("\n\tSomething went wrong. %s File has been closed before end of file reached!\n", filepath);
        exit(0);
    }

    fclose(fp);
    return wordCount;
}

/*** function to check word is present in data loaded or not ***/
int checkWord(DICT *dict, const char *word)
{
    node *current = *dict;
    int len = strlen(word), indexForNext;
    // printf("%s len %d\n", word, len);

    char Letter;

    for (int i = 0; i < len; i++)
    {
        Letter = word[i];   // letter at index of wordIndex in word
        if (Letter == '\'') // if letter is appostrophy
            indexForNext = 26;
        else
            indexForNext = Letter - 'a'; // index for alphabetical character w.r.to. char 'a'

        if (current->next[indexForNext] == NULL)
            return 0;
        current = current->next[indexForNext];
        // printf("letter %c\n", current->letter);
    }
    return (current->isWordComplete ? 1 : 0);
}

/*** Recursive function to unload trie from memory. ***/
void UnloadData(DICT *dict)
{

    // Recursively freeing allocated memory
    for (int i = 0; i < SIZE; i++)
        if ((*dict)->next[i] != NULL)
            UnloadData(&((*dict)->next[i]));

    free(*dict);
    *dict = NULL;
}

/** function to print all words loaded into the trie **/
void traverse(DICT dict, int level, char word[])
{

    if (dict->isWordComplete)
    {
        word[level] = '\0';
        printf("%s", word);
        // printf(" - length %d\n", level);
        cnt++;
    }

    for (int i = 0; i < SIZE; i++)
    {

        if (dict->next[i])
        {
            word[level] = i + 'a';
            traverse(dict->next[i], level + 1, word);
        }
    }
    return;
}
