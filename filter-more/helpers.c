#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Prototype
RGBTRIPLE average_pixel(int height, int width, RGBTRIPLE image[height][width], int pixel[2]);
RGBTRIPLE edge_pixel(int height, int width, RGBTRIPLE image[height][width], int pixel[2]);
void index_convert(int pixel[2], int actual_index[2], int C[2]);

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

            image[i][j] = average_pixel(height, width, copy, coordinate);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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

            image[i][j] = edge_pixel(height, width, copy, coordinate);
        }
    }

    free(copy);
    return;
}

RGBTRIPLE average_pixel(int height, int width, RGBTRIPLE image[height][width], int pixel[2])
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

RGBTRIPLE edge_pixel(int height, int width, RGBTRIPLE image[height][width], int pixel[2])
{
    float Red = 0;
    float Green = 0;
    float Blue = 0;
    float Gx_R = 0;
    float Gx_G = 0;
    float Gx_B = 0;
    float Gy_R = 0;
    float Gy_G = 0;
    float Gy_B = 0;
    int (*Gx)[3] = calloc(3, sizeof(int) * 3);
    int (*Gy)[3] = calloc(3, sizeof(int) * 3);
    int currect_index[2];
    RGBTRIPLE _;

    Gx[0][0] = -1;
    Gx[0][1] = 0;
    Gx[0][2] = 1;
    Gx[1][0] = -2;
    Gx[1][1] = 0;
    Gx[1][2] = 2;
    Gx[2][0] = -1;
    Gx[2][1] = 0;
    Gx[2][2] = 1;

    Gy[0][0] = -1;
    Gy[0][1] = -2;
    Gy[0][2] = -1;
    Gy[1][0] = 0;
    Gy[1][1] = 0;
    Gy[1][2] = 0;
    Gy[2][0] = 1;
    Gy[2][1] = 2;
    Gy[2][2] = 1;

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

            currect_index[0] = i;
            currect_index[1] = j;

            // Coordinateds to the Gx and Gy arrays (table)
            int C[2];
            index_convert(pixel, currect_index, C);

            Gx_R += image[i][j].rgbtRed * Gx[C[0]][C[1]];
            Gx_G += image[i][j].rgbtGreen * Gx[C[0]][C[1]];
            Gx_B += image[i][j].rgbtBlue * Gx[C[0]][C[1]];

            Gy_R += image[i][j].rgbtRed * Gy[C[0]][C[1]];
            Gy_G += image[i][j].rgbtGreen * Gy[C[0]][C[1]];
            Gy_B += image[i][j].rgbtBlue * Gy[C[0]][C[1]];
        }
    }

    Red = sqrt(Gx_R * Gx_R + Gy_R * Gy_R);
    Green = sqrt(Gx_G * Gx_G + Gy_G * Gy_G);
    Blue = sqrt(Gx_B * Gx_B + Gy_B * Gy_B);

    if (Red > 255)
    {
        Red = 255;
    }
    else if (Red < 0)
    {
        Red = 0;
    }

    if (Green > 255)
    {
        Green = 255;
    }
    else if (Green < 0)
    {
        Green = 0;
    }

    if (Blue > 255)
    {
        Blue = 255;
    }
    else if (Blue < 0)
    {
        Blue = 0;
    }

    _.rgbtRed = round(Red);
    _.rgbtGreen = round(Green);
    _.rgbtBlue = round(Blue);

    return _;
}

void index_convert(int pixel[2], int actual_index[2], int C[2])
{
    if (pixel[0] - actual_index[0] == 1)
    {
        C[0] = 0;

        switch (pixel[1] - actual_index[1])
        {
            case 1:
                C[1] = 0;
                return;
            case 0:
                C[1] = 1;
                return;
            case -1:
                C[1] = 2;
                return;
            default:
                printf("Error: 10");
                return;
        }
    }

    if (pixel[0] - actual_index[0] == 0)
    {
        C[0] = 1;

        switch (pixel[1] - actual_index[1])
        {
            case 1:
                C[1] = 0;
                return;
            case 0:
                C[1] = 1;
                return;
            case -1:
                C[1] = 2;
                return;
            default:
                printf("Error: 11");
                return;
        }
    }

    if (pixel[0] - actual_index[0] == -1)
    {
        C[0] = 2;

        switch (pixel[1] - actual_index[1])
        {
            case 1:
                C[1] = 0;
                return;
            case 0:
                C[1] = 1;
                return;
            case -1:
                C[1] = 2;
                return;
            default:
                printf("Error: 12");
                return;
        }
    }

    printf("Error: 13");
    return;
}
