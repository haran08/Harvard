#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *Correct_Usage = "Usage: ./filer memory_card.raw\n";

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: invalid number of arguments. Expected: 1 | Used: %i\n", argc - 1);
        printf("%s", Correct_Usage);
        return 1;
    }

    // Start file
    char *infile_name = argv[1];

    FILE *inptr = fopen(infile_name, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s\n", infile_name);
        return 1;
    }

    // Variables
    uint8_t buffer[512];
    int image_n = -1;
    char *out_name = malloc(sizeof(char) * 8);
    FILE *outfile = NULL;

    int debug_int = 0;

    while (fread(buffer, 1, 512, inptr) == 512)
    {
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF &&
            (buffer[3] & 0xF0) == 0xE0)
        {
            if (outfile != NULL)
            {
                fclose(outfile);
            }

            sprintf(out_name, "%0*i.jpg", 3, ++image_n);
            outfile = fopen(out_name, "a");
            if (outfile == NULL)
            {
                printf("Could not open %s\n", out_name);
                return 2;
            }

            if ((debug_int = fwrite(buffer, 1, 512, outfile)) != 512)
            {
                printf("Error: Could not write all buffer data to outfile.\nBytes readed: %i\n",
                       debug_int);
                return 3;
            }

            continue;
        }

        if (outfile == NULL)
        {
            fseek(inptr, 512, SEEK_CUR);
            continue;
        }

        fwrite(buffer, 1, 512, outfile);
        continue;
    }

    fclose(outfile);
    fclose(inptr);
    free(out_name);

    return 0;
}
