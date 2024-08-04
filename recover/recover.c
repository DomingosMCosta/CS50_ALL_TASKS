#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512

bool is_jpeg_header(uint8_t *buffer) {
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        perror("Could not open input file");
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];
    size_t bytes_read;
    int counter = 0;
    FILE *outfile = NULL;
    char outfilename[16];

    while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, infile)) > 0) {
        if (is_jpeg_header(buffer)) {
            if (outfile != NULL) {
                fclose(outfile);
            }
            sprintf(outfilename, "%03d.jpg", counter++);
            outfile = fopen(outfilename, "wb");
            if (outfile == NULL) {
                perror("Could not open output file");
                fclose(infile);
                return 1;
            }
        }

        if (outfile != NULL) {
            fwrite(buffer, 1, bytes_read, outfile);
        }
    }

    if (outfile != NULL) {
        fclose(outfile);
    }
    fclose(infile);

    return 0;
}