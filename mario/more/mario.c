#include <cs50.h>
#include <stdio.h>

void Print_Bricks(int n);

void Print_Spaces(int n);

int main(void)
{
    int size;
    do
    {
        size = get_int("Height: ");
    }
    while (size > 8 || size <= 0);

    for (int i = 1; i <= size; i++)
    {
        // print the spaces that cames before the bricks
        Print_Spaces(size - i);
        // print bricks from top to bottom of the left side
        Print_Bricks(i);
        // separate the right and left side of the wall
        printf("  ");
        // print bricks from top to bottom of the right side
        Print_Bricks(i);
        // change line to the next row of the bricks wall
        printf("\n");
    }
}

// print '#' N times; N been the interge input
void Print_Bricks(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}

// print ' ' N times; N been the interge input
void Print_Spaces(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}
