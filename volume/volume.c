// Modifies the volume of an audio file
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
char *usage_message = "Usage: ./volume input.wav output.wav factor\n";

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("%s Error: use of wrong number of arguments. %i | expected: 4\n",
        usage_message, argc);
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if ( (i == 1 || i == 2) && strstr(argv[i], ".wav") == NULL )
        {
            printf("%s Error: use of invalid file format.  %s | expected: .wav\n",
                usage_message, argv[i]);
            return 2;
        }
        else if (i == 3)
        {
            for (int j = 0, k = strlen(argv[i]); j < k; j++)
            {
                if (argv[i][j] == '.')
                {
                    break;
                }
                else if (j == k)
                {
                    printf("%s Error: use of a invalid argument. %s | expected: float\n",
                    usage_message, argv[i]);
                    return 3;
                }
            }

        }
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 4;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 5;
    }

    float factor = atof(argv[3]);

    // Coping header
    char header[HEADER_SIZE];

    size_t _ = fread(header, sizeof(char), HEADER_SIZE, input);
    if (_ != HEADER_SIZE)
    {
        printf("Could not read header.\n");
        return 6;
    }
    fwrite(header, sizeof(char), HEADER_SIZE, output);

    // Change volume
    int16_t byte[2];
    while (fread(byte, sizeof(int16_t), 1, input))
    {
        float a = byte[0] * factor;
        float b = byte[1] * factor;

        byte[0] = round(a);
        byte[1] = round(b);
        fwrite(byte, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
