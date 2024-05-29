// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
    bool test;
} node;

// TODO: Choose number of buckets in hash table maybe 26^3?
void free_dictionary(node *d);
const unsigned int N = 101;
int NmbLtr = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // I create a duplicate to avoid altering the original input word
    char *wordDuplicate = malloc(strlen(word) + 1);
    if (wordDuplicate == NULL)
    {
        return false;
    }
    strcpy(wordDuplicate, word);

    // I upper case copy word
    for (int i = 0; wordDuplicate[i]; i++)
    {
        wordDuplicate[i] = toupper(wordDuplicate[i]);
    }

    // I hash the copy word to find which bucket might be inside
    unsigned int i = hash(wordDuplicate);
    // if test == false, means table[i] node never started
    if (table[i]->test == false)
    {
        free(wordDuplicate);
        return false;
    }
    // check first word with copy word
    else if (strcmp(table[i]->word, wordDuplicate) == 0)
    {
        free(wordDuplicate);
        return true;
    }
    // check all other words linking tree by creating a buffer nodeSearch and pointing to next
    else
    {
        node *nodeSearch = table[i]->next;
        // inside this while I enter in a loop until finding word or ending this linking tree
        while (nodeSearch != NULL)
        {
            // if found word return true
            if (strcmp(nodeSearch->word, wordDuplicate) == 0)
            {
                free(wordDuplicate);
                return true;
            }
            // if not search next
            else
            {
                nodeSearch = nodeSearch->next;
            }
        }
    }
    // free copy since was created with malloc
    free(wordDuplicate);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: I considered the size of the word, and for each char I was adding the previous one +
    // value word + the
    // previous value hash * 26. This was my way of trying to normalize the insertion of data
    // then I hash mod 101 so I could have the remainder of (hash/101). This value would be used to
    // allocate to one of the 101 defined buckets
    unsigned int hash = 0;
    int i = strlen(word);

    while (i != 0)
    {
        hash = hash + word[i - 1] + (hash * 26);
        i--;
    }

    hash = hash % 101;
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // I used fopen to open the dictionary (defined already in speller.c)
    // Then I got created the wordD variable to use in fread, the int xLetters to count number of
    // words and previouly I created a global variable int NmbLtr = 0; so then I could call it in
    // size function
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }
    char wordD[LENGTH + 1];
    int xLetters = 0;

    // I go through all buckets and preload every single one to faster later. and change the
    // table->test to false the variable test was added by me in the structure of node I also
    // defined ->next as NULL so at least is would be initialized (this is to avoid errors later in
    // the unload)
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        if (table[i] != NULL)
        {
            table[i]->test = false;
            table[i]->next = NULL;
        }
        else if (table[i] == NULL)
        {
            return false;
        }
    }

    // I scan every word in the file, put every single one UPPERCASE (for standardiztion)
    while (fscanf(file, "%s", wordD) != EOF)
    {
        for (int i = 0; wordD[i]; i++)
        {
            wordD[i] = toupper(wordD[i]);
        }

        // calculdate hash to define which bucket to go
        unsigned int i = hash(wordD);

        // give first values for empty buckets (test == false)
        if (table[i]->test == false)
        {

            strcpy(table[i]->word, wordD);
            table[i]->next = NULL;
            xLetters++;
            table[i]->test = true;
        }
        // initialize nodes if bucket already got words. use malloc to initialize those nodes
        // then allocate by copying string value (not pointer) into new node.
        // point newnode->next to previous value of list
        // point list to newnode, this way Ill keep pushing new values from the left
        // also i keep score of number of words
        else
        {
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            strcpy(n->word, wordD);
            n->next = table[i];
            table[i] = n;
            xLetters++;
        }
    }
    NmbLtr = xLetters;
    fclose(file);
    return true;
}

unsigned int size(void)
{
    // just calling global variable
    return NmbLtr;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // just calling the auxiliary function until end
    for (int i = 0; i < N; i++)
    {
        free_dictionary(table[i]);
    }

    return true;
}

void free_dictionary(node *d)
{
    // TODO: Handle base case

    // TODO: Free node recursively there's no need to check the very first node of each linking
    // tree, because all of them were preloaded. I Check if there is a next. if there is a next then
    // I recursively repeate function until there isnt. if there isnt I free that node and then it
    if (d->next != NULL)
    {
        free_dictionary(d->next);
    }

    free(d);
}
