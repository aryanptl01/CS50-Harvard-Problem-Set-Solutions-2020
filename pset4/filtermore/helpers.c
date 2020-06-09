#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for ( int j = 0; j < width; j++)
        {
            int originalcolor = round((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.00);
            image[i][j].rgbtRed = originalcolor;
            image[i][j].rgbtBlue = originalcolor;
            image[i][j].rgbtGreen = originalcolor;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for ( int j = 0; j < width / 2; j++)            // flipping about centre line
        {
            RGBTRIPLE test;

            test.rgbtRed = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtRed = test.rgbtRed;

            test.rgbtBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][width - 1 - j].rgbtBlue = test.rgbtBlue;

            test.rgbtGreen = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][width - 1 - j].rgbtGreen = test.rgbtGreen;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE test[height][width];
    float avg = 0, testRed = 0, testBlue = 0, testGreen = 0;

    for (int i = 0; i < height; i++)
    {
        for ( int j = 0; j < width; j++)
        {
            for ( int k = -1; k < 2; k++)       // k = height
            {
                for ( int l = -1; l < 2; l++)   // l = width
                {
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        testRed += image[i + k][j + l].rgbtRed;
                        testBlue += image[i + k][j + l].rgbtBlue;
                        testGreen += image[i + k][j + l].rgbtGreen;
                        avg++;
                    }
                }
            }
            test[i][j].rgbtRed = round(testRed / avg);
            test[i][j].rgbtBlue = round(testBlue / avg);
            test[i][j].rgbtGreen = round(testGreen / avg);
            avg = 0; testRed = 0; testBlue = 0; testGreen = 0;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = test[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3];
    Gx[0][0] = -1;
    Gx[0][1] = 0;
    Gx[0][2] = 1;
    Gx[1][0] = -2;
    Gx[1][1] = 0;
    Gx[1][2] = 2;
    Gx[2][0] = -1;
    Gx[2][1] = 0;
    Gx[2][2] = 1;

    int Gy[3][3];
    Gy[0][0] = -1;
    Gy[0][1] = -2;
    Gy[0][2] = -1;
    Gy[1][0] = 0;
    Gy[1][1] = 0;
    Gy[1][2] = 0;
    Gy[2][0] = 1;
    Gy[2][1] = 2;
    Gy[2][2] = 1;

    float testRedx = 0, testBluex = 0, testGreenx = 0, testRedy = 0, testBluey = 0, testGreeny = 0;

    RGBTRIPLE image2[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)     // considering pixel " image[i][j] ""
        {
            for ( int k = -1; k < 2; k++)       // k = height
            {
                for ( int l = -1; l < 2; l++)   // l = width
                {
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        testRedx += image[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];     // testColourx = (sum of matrix multiplication values)
                        testBluex += image[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                        testGreenx += image[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];

                        testRedy += image[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                        testBluey += image[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                        testGreeny += image[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                    }
                }
            }

            int a = round(sqrt((pow(testRedx, 2) + pow(testRedy, 2))));
            int b = round(sqrt((pow(testBluex, 2) + pow(testBluey, 2))));
            int c = round(sqrt((pow(testGreenx, 2) + pow(testGreeny, 2))));
            testRedx = 0; testBluex = 0; testGreenx = 0;
            testRedy = 0; testBluey = 0; testGreeny = 0;

            if (a > 255)
            {
                a = 255;
            }

            if (b > 255)
            {
                b = 255;
            }

            if (c > 255)
            {
                c = 255;
            }

            image2[i][j].rgbtRed = a;
            image2[i][j].rgbtBlue = b;
            image2[i][j].rgbtGreen = c;
        }
    }

    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            image[a][b] = image2[a][b];
        }
    }
    return;
}

//          check50 cs50/problems/2020/x/filter/more

//          firstly, i just created a second image array to store the new image, so the edge function won't malfunction
//          second, i don't know if this is a correct fix, but i think that the BYTE has a limit to the value it can store, which is why
//          i didn't use the RGBTRIPLE at all, and i just used a normal variable to do the calculations all in one line, as on line 137-139
//          it does all the calculations in one line so it's compact and doesn't need any other variables. :100:
//          and after you copied my code, i just moved a few pieces of your code to put them into the main loop
//          because i find it less confusing if i put everything into 1 loop
//          so i put all those calculations so the loop will do those calculations for every pixel.
//          you can see up there, i put all the previous code in the j loop, which is the loop which iterates through every pixel
//          that's basically all i did.