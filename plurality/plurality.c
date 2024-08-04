#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    char *name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(char *name);
void print_winner(void);

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count;
    printf("Number of voters: ");
    if (scanf("%d", &voter_count) != 1 || voter_count < 1)
    {
        printf("Invalid number of voters.\n");
        return 3;
    }

    // Clear the newline character left in the input buffer by scanf
    getchar();

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        char name[100];
        printf("Vote: ");
        if (fgets(name, sizeof(name), stdin) == NULL)
        {
            printf("Error reading vote.\n");
            return 4;
        }

        // Remove newline character from the end of the input if present
        name[strcspn(name, "\n")] = '\0';

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(char *name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }

    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int highestvote = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > highestvote)
        {
            highestvote = candidates[i].votes;
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == highestvote)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}