#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
char *candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, char *name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool DFS(int start, int target);

int main(int argc, char *argv[])
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
    int voter_count;
    printf("Number of voters: ");
    if (scanf("%d", &voter_count) != 1 || voter_count < 1)
    {
        printf("Invalid number of voters.\n");
        return 3;
    }
    // Clear the newline character left in the buffer by scanf
    getchar();

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char name[100];
            printf("Rank %i: ", j + 1);
            if (fgets(name, sizeof(name), stdin) == NULL)
            {
                printf("Error reading input.\n");
                return 4;
            }

            // Remove newline character from the end of the input if present
            name[strcspn(name, "\n")] = '\0';

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

// Update ranks given a new vote. I compare the candidate's name with name provided by the user with
// a fgets. If it finds a match it adds that candidate's array position (which will work as ID)
// to the array ranks[respective rank position]. The function is called by using !vote, because when
// it checks for validity, at same time it runs it.
bool vote(int rank, char *name, int ranks[])
{
    for (int n = 0; n < candidate_count; n++)
    {
        if (strcmp(candidates[n], name) == 0)
        {
            ranks[rank] = n;

            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks. This function will add up/increment the value of
// candidate pairs (in preferences[i][j]) where i is the winner and j the loser candidate. In this
// function there's no need to call again the candidates positions as the array ranks already
// registered. For example, ranks[0] will have registered some candidate (i), and since rank[0]
// is the highest possible, that candidate will always be above of those in rank[1+]. Example to
// show how this works in practice is:
// 3 candidates (a, b ,c) and rank[0] = a; rank[1] = b; rank[2] = c. The first two iterations would
// be:
// preferences[rank[0]][rank[1]]++ -> preferences[a][b]++
// preferences[rank[0]][rank[2]]++ -> preferences[a][c]++
// and so on
void record_preferences(int ranks[])
{
    for (int winner = 0; winner < candidate_count - 1; winner++)
    {
        for (int loser = winner + 1; loser < candidate_count; loser++)
        {
            preferences[ranks[winner]][ranks[loser]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other. This function will use the
// values from preferences[][] and create pairs of the highest values. Example by using 3 candidates
// and 2 voters. voter1: a, b, c // voter2: a, c, b
// preferences[a][b] = 2 / preferences[a][c] = 2 / preferences[b][c] = 1 / preferences[c][b] = 1
// first iteration: if preferences[a][b] > preferences[b][a] (translated is if 2 > 0)
// add to first available array pair[pair_count] (the first would be pair[0])
// so pair[0].winner = a and pair[0].loser = b
// continue loop. the preferences[b][c] and preferences[c][b] cant be added because they have same
// value.
void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory. Based on the created pairs and their
// preferences (mentioned in line 122 for this example) In this case method, you start with the most
// left pair and compare it with all ones to its right. first pair[i] is i = 0, so pair[0], we want
// to compare it with the following ones so with pairs[j] where j = i+ (we start at i = j, but the
// pair[i][i] will never exist) therefore the very first iteration is: is strength of pair[1] >
// strength pair[0]. which translated is: is preferences[pairs[1].winner][pairs[1].loser] >
// preferences[pairs[0].winner][pairs[0].loser]  ->>
// ->> is preferences[a][c] (which is 2) > preferences[a][b] (which is also 2), since both have same
// value, they will not switch positions. but if they were to switch we would save the value of the
// lowest strength pair in temp[], then we simply make the value of the most left pair with the now
// the strongest pair value, and make the previous position equal to value of temp[].
void sort_pairs(void)
{
    pair temp;
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] >
                preferences[pairs[i].winner][pairs[i].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles. For this DFS (depth-first
// search) function was used. The first utility of lock_pairs function is to supply DFS with a start
// point and a target point. By using the created pairs (now in order of strength) we start from 0
// to pair_count - 1. The first would be pair[0], therefore we supply DFS with start =
// pairs[0].loser and target pairs[0].winner. In other words, we want to make sure that before
// locking this pair, that there isn't another path already to its loser. Check DFS function extra
// details. If DFS returns false, then we can lock that pair.
void lock_pairs(void)
{
    for (int j = 0; j < pair_count; j++)
    {
        if (DFS(pairs[j].loser, pairs[j].winner) == false)
        {
            locked[pairs[j].winner][pairs[j].loser] = true;
        }
    }
}

// This function will use start and the target points.
// Since these points are in fact the candidates, we use a loop from 0 to the number of candidates
// -1. The logic behind this is the following: we check if we have previously locked a pair where the
// loser has won. If it has won, we then check if the loser from that is the same as the winner
// we are trying to lock (target). If yes, then we can lock the originally locked[target][start]. If
// not, we have to recursively ask the same question, now considering the loser of the loser and so
// on until all possibilities were checked.
bool DFS(int start, int target)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[start][i] == true)
        {
            if (i == target)
            {
                return true;
            }
            else if (DFS(i, target))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election. The logic used in this scenario is, if we were to expose all
// locked_pairs (true and false) in a matrix, we would notice that the source of the graph would be
// the one candidate that was never in a lose position. In other words, we check every lose for
// candidate i (if (locked[j][i] == false) and if the number of falses (meaning the number of non
// loses) is equal to the number of candidates, it means that candidate is the source.
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int counter = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                counter++;
            }
        }
        if (counter == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }

    // TODO
    return;
}