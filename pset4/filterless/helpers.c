#include "helpers.h"


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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for ( int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalBlue = image[i][j].rgbtBlue;
            int originalGreen = image[i][j].rgbtGreen;

            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtBlue = round(sepiaBlue);
            image[i][j].rgbtGreen = round(sepiaGreen);
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
//          check50 cs50/problems/2020/x/filter/less
