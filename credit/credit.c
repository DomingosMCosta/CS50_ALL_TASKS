//#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long digits;
    long n;
    long reduce;
    long sum_numbers = 0;
    int counter_sum = 0;
    long y;
    printf("Number: ");
    scanf("%ld", &n);
    reduce = n;
    // Luhn algorithm loop
    do
    {
        // Save the final 2 digits
        if (reduce / 10 != 0)
        {
            digits = reduce;
        }

        // sum of digits not multiplied by 2
        if (counter_sum % 2 == 0)
        {
            sum_numbers = sum_numbers + (reduce % 10);
        }
        // multiplicatiom of digits by 2
        else
        {
            y = (reduce % 10) * 2;
            // sum of digits from the multiplication by 2
            for (int i = y; i != 0; i = i / 10)
            {
                sum_numbers = sum_numbers + (y % 10);
                y = y / 10;
            }
        }
        reduce = reduce / 10;
        // counter & save total number of digits
        counter_sum++;
    }
    while (reduce != 0);
    
    if (sum_numbers % 10 == 0)
    {
        if (counter_sum == 15 && (digits == 34 || digits == 37))
        {
            printf("AMEX\n");
        }
        else if (counter_sum == 16 && (digits >= 51 && digits <= 55))
        {
            printf("MASTERCARD\n");
        }
        else if ((counter_sum == 16 || counter_sum == 13) && (digits / 10 == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
