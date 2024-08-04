#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // For malloc and free

int *extract_values(char *word, int n, int extractions[3]);

int main(void)
{
    // Allocate a buffer for user input
    char text_input[1000];
    
    // Prompt user for text input
    printf("Text: ");
    if (fgets(text_input, sizeof(text_input), stdin) == NULL)
    {
        printf("Error reading input.\n");
        return 1;
    }

    // Remove the newline character if present
    text_input[strcspn(text_input, "\n")] = '\0';

    int n1 = strlen(text_input);
    int final[3];
    extract_values(text_input, n1, final);

    float L = (float) final[0] / final[1] * 100.0;
    float S = (float) final[2] / final[1] * 100.0;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %li\n", lround(index));
    }

    return 0;
}

int *extract_values(char *word, int n, int extractions[3])
{
    extractions[0] = 0;  // Letters
    extractions[1] = 0;  // Words
    extractions[2] = 0;  // Sentences

    for (int i = 0; i < n; i++)
    {
        if (isalpha(word[i]))
        {
            extractions[0]++;
        }
        if (isspace(word[i]))
        {
            extractions[1]++;
        }
        if (word[i] == '?' || word[i] == '!' || word[i] == '.')
        {
            extractions[2]++;
        }
    }
    
    // Ensure words count includes the last word
    if (n > 0 && !isspace(word[n - 1]))
    {
        extractions[1]++;
    }

    return extractions;
}