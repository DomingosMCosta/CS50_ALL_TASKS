#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int *extract_values(string word, int n, int extractions[3]);

int main(void)
{
    string text_input = get_string("Text: ");
    int n1;
    n1 = strlen(text_input);
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
}

int *extract_values(string word, int n, int extractions[3])
{

    extractions[0] = 0;
    extractions[1] = 1;
    extractions[2] = 0;

    for (int i = 0; i < n; i++)
    {
        if (isupper(word[i]) || islower(word[i]))
        {
            extractions[0]++;
        }
        if (word[i] == 32)
        {
            extractions[1]++;
        }
        if (word[i] == 63 || word[i] == 33 || word[i] == 46)
        {
            extractions[2]++;
        }
    }
    return extractions;
}
