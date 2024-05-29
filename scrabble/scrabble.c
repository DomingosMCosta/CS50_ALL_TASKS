#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word, int n);

int main(void)
{
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");
    int n1;
    int n2;
    n1 = strlen(word1);
    n2 = strlen(word2);

    for (int i = 0; i < n1; i++)
    {
        word1[i] = toupper(word1[i]);
    }
    for (int i = 0; i < n2; i++)
    {
        word2[i] = toupper(word2[i]);
    }

    int score1 = compute_score(word1, n1);
    int score2 = compute_score(word2, n2);

    if (score1 > score2)
    {
        printf("Player 1 wins! \n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins! \n");
    }
    else
    {
        printf("Tie! \n");
    }
}
int compute_score(string word, int n)
{
    int score = 0;

    for (int i = 0; i < n; i++)
    {
        if (word[i] - 'A' <= 25 && word[i] - 'A' >= 0)
        {
            score += POINTS[word[i] - 'A'];
        }
    }

    return score;
}
