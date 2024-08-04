#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the point values for each letter
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Function prototypes
int compute_score(const char *word, int length);

int main(void)
{
    // Buffer sizes for player inputs
    #define BUFFER_SIZE 100
    
    // Allocate buffers for player inputs
    char word1[BUFFER_SIZE];
    char word2[BUFFER_SIZE];
    
    // Prompt user for input
    printf("Player 1: ");
    if (fgets(word1, sizeof(word1), stdin) == NULL) {
        perror("Error reading input");
        return 1;
    }

    printf("Player 2: ");
    if (fgets(word2, sizeof(word2), stdin) == NULL) {
        perror("Error reading input");
        return 1;
    }

    // Remove newline characters from input
    word1[strcspn(word1, "\n")] = '\0';
    word2[strcspn(word2, "\n")] = '\0';

    // Convert to uppercase
    for (int i = 0; word1[i]; i++) {
        word1[i] = toupper(word1[i]);
    }
    for (int i = 0; word2[i]; i++) {
        word2[i] = toupper(word2[i]);
    }

    // Compute scores
    int score1 = compute_score(word1, strlen(word1));
    int score2 = compute_score(word2, strlen(word2));

    // Print result
    if (score1 > score2) {
        printf("Player 1 wins!\n");
    } else if (score2 > score1) {
        printf("Player 2 wins!\n");
    } else {
        printf("Tie!\n");
    }

    return 0;
}

// Compute the score of a word
int compute_score(const char *word, int length)
{
    int score = 0;

    for (int i = 0; i < length; i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
            score += POINTS[word[i] - 'A'];
        }
    }

    return score;
}