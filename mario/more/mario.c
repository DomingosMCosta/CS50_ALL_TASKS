#include <stdio.h>

int main(void)
{
    int n;
    int p;
    int h;
    const int spaces = 2;
    // Prompt user to give a positive integer
    do
    {
        printf("Size: ");
        scanf("%d", &n);
    }
    while (n < 1);

    p = n;
    h = n - (n - 1);

    // Print the pyramid by n height
    for (int i = 0; i < n; i++)
    {
        // Print left side spaces
        for (int j = 0; j < (p - 1); j++)
        {
            printf(" ");
        }
        p--;
        // Print # by rows from left to right
        for (int j = 0; j < h; j++)
        {
            printf("#");
            // Conditional to start printing right side of rows
            if (j == h - 1)
            {
                // Print pyramid middle spaces based on constant
                for (int l = 0; l < spaces; l++)
                {
                    printf(" ");
                }
                // Print # from right side of pyramid
                for (int l = 0; l < h; l++)
                {
                    printf("#");
                }
            }
        }
        h++;
        printf("\n");
    }
}
