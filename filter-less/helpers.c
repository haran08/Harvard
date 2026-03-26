#include "helpers.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Prototype
RGBTRIPLE get_surrounding_pixels(int height, int width, RGBTRIPLE image[height][width],
                                 int pixel[2]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE red;
    BYTE green;
    BYTE blue;
    BYTE average;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;

            average = round((red + green + blue) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE red;
    BYTE green;
    BYTE blue;
    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;

            sepiaRed = round(.393 * red + .769 * green + .189 * blue);
            sepiaGreen = round(.349 * red + .686 * green + .168 * blue);
            sepiaBlue = round(.272 * red + .534 * green + .131 * blue);

            if (sepiaRed > 255)
                sepiaRed = 255;
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            if (sepiaBlue > 255)
                sepiaBlue = 255;

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel_1;
    RGBTRIPLE pixel_2;
    int last_pixel;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            last_pixel = (width - 1) - j;

            if (j == last_pixel || j > last_pixel)
                break;

            pixel_1 = image[i][j];
            pixel_2 = image[i][last_pixel];

            image[i][j] = pixel_2;
            image[i][last_pixel] = pixel_1;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int coordinate[2];
    RGBTRIPLE(*copy)[height] = calloc(height, sizeof(RGBTRIPLE) * width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            coordinate[0] = i;
            coordinate[1] = j;

            image[i][j] = get_surrounding_pixels(height, width, copy, coordinate);
        }
    }

    return;
}

RGBTRIPLE get_surrounding_pixels(int height, int width, RGBTRIPLE image[height][width],
                                 int pixel[2])
{
    float red = 0;
    float green = 0;
    float blue = 0;
    BYTE count = 0;
    RGBTRIPLE _;

    for (int i = pixel[0] - 1; i <= pixel[0] + 1; i++)
    {
        if (i < 0)
        {
            continue;
        }
        else if (i == height)
        {
            continue;
        }

        for (int j = pixel[1] - 1; j <= pixel[1] + 1; j++)
        {
            if (j < 0)
            {
                continue;
            }
            else if (j == width)
            {
                continue;
            }

            red += image[i][j].rgbtRed;
            green += image[i][j].rgbtGreen;
            blue += image[i][j].rgbtBlue;
            count++;
        }
    }

    red = red / count;
    green = green / count;
    blue = blue / count;

    _.rgbtRed = round(red);
    _.rgbtGreen = round(green);
    _.rgbtBlue = round(blue);

    return _;
}
