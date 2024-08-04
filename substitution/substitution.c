#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

int main(int argc, char *argv[])
{
    // Check for correct number of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    // Check key length
    if (strlen(argv[1]) != ALPHABET_SIZE)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    
    // Check key for alphabetic characters only and for duplicate characters
    for (size_t i = 0; i < strlen(argv[1]); i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
        for (size_t j = i + 1; j < strlen(argv[1]); j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    // Prompt for plaintext
    printf("plaintext: ");
    char plain[256];
    if (fgets(plain, sizeof(plain), stdin) == NULL)
    {
        printf("Error reading plaintext.\n");
        return 1;
    }

    // Remove newline character if present
    size_t len = strlen(plain);
    if (len > 0 && plain[len - 1] == '\n')
    {
        plain[len - 1] = '\0';
    }

    // Allocate memory for cipher
    char *cipher = malloc(strlen(plain) + 1);
    if (cipher == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Encrypt the plaintext
    for (size_t i = 0; i < strlen(plain); i++)
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
    cipher[strlen(plain)] = '\0'; // Null-terminate the cipher text

    // Print the ciphertext
    printf("ciphertext: %s\n", cipher);

    // Free allocated memory
    free(cipher);

    return 0;
}