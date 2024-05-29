#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string first = "Teste";
    string last = first;

    last[0] = 'P';

    printf("%s", first);
}
