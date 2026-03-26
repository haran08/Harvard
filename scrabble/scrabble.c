#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Constants
const int NUMBER_POINT = 10;

// the points of each letter are equal to they array index + 1
const string POINT_TABLE[NUMBER_POINT] = {
    [0] = "aeilnorstu",
    [1] = "dg",
    [2] = "bcmp",
    [3] = "fhvwy",
    [4] = "k",
    [7] = "jx",
    [9] = "qz"
};

const string POINT_TABLE_NOT_USED_INDEX = "568";

// Prototypes
int char_score(char letter);

bool is_index_unused(int index);

bool str_contains_chr(string str, char letter);

int str_score(string str);

// Main
int main(void)
{
    string player1 = get_string("Player 1: ");
    string player2 = get_string("Player 2: ");

    int p1_Score = str_score(player1);
    int p2_Score = str_score(player2);

    if (p1_Score > p2_Score)
        printf("Player 1 wins!\n");

    else if (p1_Score < p2_Score)
        printf("Player 2 wins!\n");

    else
        printf("Tie!\n");

    return 0;
}

int char_score(char letter)
{
    for (int i = 0; i < NUMBER_POINT; i++)
    {
        if (is_index_unused(i))
            continue;

        if (str_contains_chr(POINT_TABLE[i], letter))
            return i + 1;
    }

    return 0;
}

bool is_index_unused(int index)
{
    for (int i = 0, l = strlen(POINT_TABLE_NOT_USED_INDEX); i < l; i++)
    {
        if (index == POINT_TABLE_NOT_USED_INDEX[i] - '0')
            return true;
    }

    return false;
}

bool str_contains_chr(string str, char letter)
{
    for (int i = 0, l = strlen(str); i < l; i++)
    {
        if (str[i] == letter)
            return true;
    }

    return false;
}

int str_score(string str)
{
    int score = 0;

    for (int i = 0, l = strlen(str); i < l; i++)
    {
        char c = tolower(str[i]);
        score += char_score(c);
    }

    return score;
}
