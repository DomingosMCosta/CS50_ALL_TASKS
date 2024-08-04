#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // For INT16_MAX and INT16_MIN

const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files
    FILE *input = fopen(argv[1], "rb");  // Read binary
    if (input == NULL)
    {
        printf("Could not open input file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb"); // Write binary
    if (output == NULL)
    {
        printf("Could not open output file.\n");
        fclose(input);
        return 1;
    }

    // Determine scaling factor
    float factor = atof(argv[3]);

    // Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    if (fread(header, HEADER_SIZE, 1, input) != 1)
    {
        printf("Error reading header from input file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    if (fwrite(header, HEADER_SIZE, 1, output) != 1)
    {
        printf("Error writing header to output file.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    // Read and process samples
    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, input) == 1)
    {
        // Scale the sample
        float scaled = buffer * factor;

        // Clip to int16_t range
        if (scaled > INT16_MAX)
            buffer = INT16_MAX;
        else if (scaled < INT16_MIN)
            buffer = INT16_MIN;
        else
            buffer = (int16_t)scaled;

        if (fwrite(&buffer, sizeof(int16_t), 1, output) != 1)
        {
            printf("Error writing sample to output file.\n");
            fclose(input);
            fclose(output);
            return 1;
        }
    }

    if (ferror(input))
    {
        printf("Error reading samples from input file.\n");
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}