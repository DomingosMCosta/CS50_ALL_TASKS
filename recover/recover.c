#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int ImageSize = 512;

int main(int argc, char *argv[])
{
    bool checkstart = false;
    char *filename = malloc(8 * sizeof(char));
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    int counter = 0;
    sprintf(filename, "%03i.jpg", counter);
    FILE *img = fopen(filename, "w");
    if (img == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    fclose(img);

    uint8_t buffer[ImageSize];

    while (fread(buffer, sizeof(buffer), 1, f) != 0)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (counter > 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not open file.\n");
                return 1;
            }

            fwrite(buffer, sizeof(buffer), 1, img);
            counter++;
            checkstart = true;
        }
        else if (checkstart == true)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    fclose(img);
    fclose(f);
    free(filename);
}
