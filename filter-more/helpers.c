#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImage[i][j] = image[i][j];
        }
    }

    uint8_t average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round((float) (copyImage[i][j].rgbtBlue + copyImage[i][j].rgbtGreen +
                                     copyImage[i][j].rgbtRed) /
                            3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        RGBTRIPLE values_row[height][width];
        for (int j = 0; j < width; j++)
        {
            values_row[i][j] = image[i][j];
        }
        int counter = width;
        for (int j = 0; j < width; j++)
        {
            counter--;
            image[i][j] = values_row[i][counter];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImage[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int counter = 0;
            float averageRed = 0;
            float averageGreen = 0;
            float averageBlue = 0;

            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k < height && k >= 0 && l < width && l >= 0)
                    {
                        counter++;
                        averageRed += copyImage[k][l].rgbtRed;
                        averageGreen += copyImage[k][l].rgbtGreen;
                        averageBlue += copyImage[k][l].rgbtBlue;
                    }
                }
            }
            image[i][j].rgbtRed = round((averageRed / counter));
            image[i][j].rgbtGreen = round((averageGreen / counter));
            image[i][j].rgbtBlue = round((averageBlue / counter));
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImage[i][j] = image[i][j];
        }
    }
    int Gx[3][3];
    int Gy[3][3];
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

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float GxRed = 0;
            float GxGreen = 0;
            float GxBlue = 0;
            float GyRed = 0;
            float GyGreen = 0;
            float GyBlue = 0;

            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if ((k < height && k >= 0) && (l < width && l >= 0))
                    {
                        int relk = k - (i - 1);
                        int rell = l - (j - 1);
                        GxRed += (float) Gx[relk][rell] * copyImage[k][l].rgbtRed;
                        GxGreen += (float) Gx[relk][rell] * copyImage[k][l].rgbtGreen;
                        GxBlue += (float) Gx[relk][rell] * copyImage[k][l].rgbtBlue;
                        GyRed += (float) Gy[relk][rell] * copyImage[k][l].rgbtRed;
                        GyGreen += (float) Gy[relk][rell] * copyImage[k][l].rgbtGreen;
                        GyBlue += (float) Gy[relk][rell] * copyImage[k][l].rgbtBlue;
                    }
                }
            }
            image[i][j].rgbtRed = fmin(255, round(sqrt((GxRed * GxRed) + (GyRed * GyRed))));
            image[i][j].rgbtGreen =
                fmin(255, round(sqrt((GxGreen * GxGreen) + (GyGreen * GyGreen))));
            image[i][j].rgbtBlue = fmin(255, round(sqrt((GxBlue * GxBlue) + (GyBlue * GyBlue))));
        }
    }
    return;
}
