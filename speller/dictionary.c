// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

#define L_Average_Size 9
#define M_Average_Size 17
#define B_Size LENGTH + 1

// Represents a node in a hash table
typedef struct node
{
    char *word;
    struct node *next;
} node;

// Start: Hash table constants
const unsigned int N = 0x40000; // 0x40000 = 2^18; 2^17 = 131,072;

// End

// Start: Murmur3_x64_128 constants
const uint64_t c1_mur = 0x87c37b91114253daULL;
const uint64_t c2_mur = 0x4cf5ad432745937fULL;

const int r1_mur = 31;
const int r2_mur = 33;
const int m_mur = 5;
const int64_t n_mur = 0x52dce729b49a7ce3ULL;

// Finalization constants
const uint64_t f1_mur = 0xff51afd7ed558ccdULL;
const uint64_t f2_mur = 0xc4ceb9fe1a85ec53ULL;

// End

// Start: Debug Variables
int debug = 0;

// End

// Hash table
int ht_size = 0;
node *table[N];

// Prototypes
void cp_chars(char *inp, int size, char *out);
void dictionary_append(char *w);
void free_buckets(node *b);
uint64_t fmix64(uint64_t k);
uint64_t murmur3_x64_64(const void *key, const int len, uint32_t seed);
uint64_t rotl64(uint64_t x, int r);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *w = table[hash(word)];

    while (w != NULL)
    {
        if (strcasecmp(w->word, word) == 0)
            return true;

        w = w->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int l = strlen(word);
    char *w = malloc(sizeof(char) * l + 1);
    for (int i = 0; i < l; i++)
    {
        w[i] = tolower(word[i]);
    }
    w[l] = '\0';

    uint64_t h = murmur3_x64_64(w, l, 0);
    free(w);

    return h & (N - 1);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *d = fopen(dictionary, "r");
    if (d == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    char *word = malloc(sizeof(char) * L_Average_Size);
    int w_size = 0, index = 0;
    char c;
    int eof = 1;

    while (fread(&c, sizeof(char), 1, d))
    {
        if (c == '\n' || eof == 0)
        {
            word[index] = '\0';
            dictionary_append(word);
            word = malloc(
                sizeof(char) *
                L_Average_Size); // WARNING: if dic_append is a pointer append: Indispensable line.
            index = 0;
            w_size = 0;
            continue;
        }
        else if (index < L_Average_Size - 1)
        {
            word[index++] = c;
            continue;
        }
        else if (index < M_Average_Size - 1)
        {
            if (w_size < 1)
            {
                char *_ = malloc(sizeof(char) * M_Average_Size);
                cp_chars(word, index, _);
                free(word);
                word = _;
                w_size = 1;
            }

            word[index++] = c;
            continue;
        }
        else if (index < B_Size - 1)
        {
            if (w_size < 2)
            {
                char *_ = malloc(sizeof(char) * B_Size);
                cp_chars(word, index, _);
                free(word);
                word = _;
                w_size = 2;
            }

            word[index++] = c;
            continue;
        }
        else
        {
            printf("SizeOf Error: Word Bigger than 45 characters.");
            return false;
        }
    }

    fclose(d);
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return ht_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            free_buckets(table[i]);
        }
    }

    return true;
}

// Copy the all characters in inp to out.
// *inp: input array of characters. size: size of array.
// *out: buffer to story copy
void cp_chars(char *inp, int size, char *out)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = inp[i];
    }
}

// Free all the memory of a linked-list.
// *b: the first element of a linked-list data-structure.
void free_buckets(node *b)
{
    if (b->next != NULL)
        free_buckets(b->next);

    free(b->word);
    free(b);
}

// Create a new node that stores the pointer string, then append the node to the Hash Table using
// the 'hash' function with the pointer string as input. *w: pointer to the first element of a
// string. len: length of the string.
void dictionary_append(char *w)
{
    debug++;
    node *n = calloc(1, sizeof(node));
    unsigned int index = hash(w);

    n->word = w;
    n->next = NULL;

    if (table[index] != NULL)
    {
        node *n1 = table[index];

        while (n1->next != NULL)
        {
            n1 = n1->next;
        }

        n1->next = n;
    }
    else
    {
        table[index] = n;
    }

    ht_size++;
}

// Murmur3_x64 hash function with a return of 64 bit.
// (uses half of the result - h1)
uint64_t murmur3_x64_64(const void *key, const int len, uint32_t seed)
{
    const uint8_t *data = (const uint8_t *) key;
    const int nblocks = len / 16;

    uint64_t h1 = seed;
    uint64_t h2 = seed;

    for (int i = 0; i < nblocks; i++)
    {
        uint64_t k1;
        uint64_t k2;

        memcpy(&k1, data + (i * 16), sizeof(uint64_t));
        memcpy(&k2, data + (i * 16) + sizeof(uint64_t), sizeof(uint64_t));

        k1 *= c1_mur;
        k1 = rotl64(k1, r1_mur);
        k1 *= c2_mur;
        h1 ^= k1;

        h1 = rotl64(h1, r2_mur);
        h1 += h2;
        h1 = h1 * m_mur + n_mur;

        k2 *= c1_mur;
        k2 = rotl64(k2, r1_mur);
        k2 *= c2_mur;
        h2 ^= k2;

        h2 = rotl64(h2, r2_mur);
        h2 += h1;
        h2 = h2 * m_mur + n_mur;
    }

    const uint8_t *tail = data + (nblocks * 16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch (len & 15)
    {
        case 15:
            k2 ^= (uint64_t) tail[14] << 48;
        case 14:
            k2 ^= (uint64_t) tail[13] << 40;
        case 13:
            k2 ^= (uint64_t) tail[12] << 34;
        case 12:
            k2 ^= (uint64_t) tail[11] << 24;
        case 11:
            k2 ^= (uint64_t) tail[10] << 16;
        case 10:
            k2 ^= (uint64_t) tail[9] << 8;
        case 9:
            k2 ^= (uint64_t) tail[8] << 0;
            k2 *= c2_mur;
            k2 = rotl64(k2, r1_mur);
            k2 *= c1_mur;
            h2 ^= k2;

        case 8:
            k1 ^= (uint64_t) tail[7] << 56;
        case 7:
            k1 ^= (uint64_t) tail[6] << 48;
        case 6:
            k1 ^= (uint64_t) tail[5] << 40;
        case 5:
            k1 ^= (uint64_t) tail[4] << 32;
        case 4:
            k1 ^= (uint64_t) tail[3] << 24;
        case 3:
            k1 ^= (uint64_t) tail[2] << 16;
        case 2:
            k1 ^= (uint64_t) tail[1] << 8;
        case 1:
            k1 ^= (uint64_t) tail[0] << 0;
            k1 *= c1_mur;
            k1 = rotl64(k1, r1_mur);
            k1 *= c2_mur;
            h1 ^= k1;
    };

    h1 ^= len;
    h2 ^= len;
    h1 += h2;
    h2 += h1;
    h1 = fmix64(h1);
    h2 = fmix64(h2);
    h1 += h2;
    h2 += h1;

    return h1;
}

// Help functions of murmur3 hash function
// HF murmur3: Rotate a long interge to the left by 'r' houses. (similar to overflow)
// x: long interge to be rotated.
// r: number of houses to move to left.
uint64_t rotl64(uint64_t x, int r)
{
    return (x << r) | (x >> (64 - r));
}

// HF murmur3: Finalization mix for long interge 'k'. Power 'k' by half of himself, then multiply by
// the const f1; Repeat it with the const f2. k: long interge to be mixed.
uint64_t fmix64(uint64_t k)
{
    k ^= k >> 33;
    k *= f1_mur;
    k ^= k >> 33;
    k *= f2_mur;
    k ^= k >> 33;
    return k;
}
