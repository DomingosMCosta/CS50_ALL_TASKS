#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc == 1 || argc >= 3)
    {
        printf("Usage: ./substitution key \n");
        return 1;
    }
    else if (!(strlen(argv[1]) == 26))
    {
        printf("Key must contain 26 characters. \n");
        return 1;
    }

    for (size_t i = 0; i < strlen(argv[1]); i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must only contain alphabetic characters. \n");
            return 1;
        }
        for (size_t j = i + 1; j < strlen(argv[1]); j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must not contain repeated characters. \n");
                return 1;
            }
        }
    }

    string plain = get_string("plaintext: ");
    string cipher = malloc(strlen(plain) + 1);
    for (int i = 0; i < strlen(plain); i++)
    {
        if (isupper(plain[i]))
        {
            cipher[i] = toupper(argv[1][plain[i] - 'A']);
        }
        else if (islower(plain[i]))
        {
            cipher[i] = tolower(argv[1][plain[i] - 'a']);
        }
        else
        {
            cipher[i] = plain[i];
        }
    }

    printf("ciphertext: %s\n", cipher);
    return 0;
}
