#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *answer = NULL;
    size_t size = 0;

    printf("What's your name? ");
    getline(&answer, &size, stdin);

    printf("hello, %s", answer);

    // Free the allocated memory
    free(answer);
}