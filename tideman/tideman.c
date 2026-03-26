#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool pair_circle(int winner, int loser);
bool check_loses(int index);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        int c = ranks[i];

        for (int j = i + 1; j < candidate_count; j++)
            preferences[c][ranks[j]] += 1;
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair p;
    // check each cadidate
    for (int i = 0; i < candidate_count; i++)
    {
        // check each candidate that not theyselfs and
        // a alreadly checked one
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] == preferences[j][i])
                continue;

            if (preferences[i][j] > preferences[j][i])
            {
                p.winner = i;
                p.loser = j;
            }
            else
            {
                p.winner = j;
                p.loser = i;
            }
            pairs[pair_count++] = p;
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 1; i < pair_count; i++)
    {
        int k = i - 1;

        pair p1 = pairs[i];
        int key[2];
        key[0] = pairs[i].winner;
        key[1] = pairs[i].loser;

        pair p2 = pairs[k];
        int var[2];
        var[0] = pairs[k].winner;
        var[1] = pairs[k].loser;

        while (k >= 0 && preferences[var[0]][var[1]] < preferences[key[0]][key[1]])
        {
            pairs[k + 1] = p2;
            k--;

            p2 = pairs[k];
            var[0] = pairs[k].winner;
            var[1] = pairs[k].loser;
        }

        pairs[k + 1] = p1;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (pair_circle(pairs[i].winner, pairs[i].loser))
            continue;

        locked[pairs[i].winner][pairs[i].loser] = true;
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if (locked[i][j] == true)
            {
                if (!check_loses(i))
                {
                    printf("%s\n", candidates[i]);
                    return;
                }
            }
        }
    }
    return;
}

// check if the loser wins agaist someone that wins agaist his winner,
// creating a circle of wins
bool pair_circle(int winner, int loser)
{
    for (int i = 0; i < MAX; i++)
    {
        if (locked[i][winner] == true)
        {
            for (int j = 0; j < MAX; j++)
            {
                if (locked[loser][i] == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool check_loses(int index)
{
    for (int i = 0; i < MAX; i++)
    {
        if (locked[i][index] == true)
            return true;
    }

    return false;
}
