// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];      // LENGTH = 45 (no word is longer than 45 letters)
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 16000;

int fsize = 0;   // incremented in load, returned in size

node *freem[N];

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hash_code = 0;

    hash_code = hash(word);     // hashing the recieved word

    for (node *tmp = table[hash_code]; tmp != NULL; tmp = tmp->next)      // iterating over the lists
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))        // *str++ is going to the next address in memory, where the next char in the string is stored
    {
        if (isupper(c))
        {
            c = c + 32;
        }

        hash = ((hash << 5) + hash) + c; // hash * 33 + c   // hash << 5 = hash * 2^5
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char words[46];
    int hash_code = 0;

    FILE *infile = fopen(dictionary, "r");
    if (infile == NULL)
    {
        return false;
    }

    int chunk = 1000;
    while (fscanf(infile, "%s", words) != EOF)
    {
        node *m = malloc(sizeof(node) * chunk);
        if (m == NULL)
        {
            return false;
        }

        int s = 0;
        freem[s] = m;
        s++;

        node *f = m;
        strcpy(f->word, words);
        hash_code = hash(f->word);

        if (table[hash_code] == NULL)
        {
            table[hash_code] = f;
            f->next = NULL;
        }
        else
        {
            f->next = table[hash_code];     // adding the new node in the middle of the table and first member
            table[hash_code] = f;
        }
        fsize++;

        for (int i = 1; i < chunk; i++)
        {
            node *n = m + i;
            while (fscanf(infile, "%s", words) != EOF)
            {
                strcpy(n->word, words);
                hash_code = hash(n->word);

                if (table[hash_code] == NULL)
                {
                    table[hash_code] = n;
                    n->next = NULL;
                }
                else
                {
                    n->next = table[hash_code];     // adding the new node in the middle of the table and first member
                    table[hash_code] = n;
                }
                fsize++;
            }
        }
    }

    fclose(infile);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return fsize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp = NULL;
        node *cursor = NULL;

        cursor = table[i];

        while(cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

    }

    for (int i = 0; i < N; i++)
    {
        free(freem[i]);
    }
    return true;
}


//      ~cs50/2019/fall/pset5/speller texts/lalaland.txt (Tests against staff code)
//      ./speller texts/lalaland.txt