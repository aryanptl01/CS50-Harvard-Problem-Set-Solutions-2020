#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef uint8_t BYTE;
BYTE bytes[512];      // buffer to store card read data

bool new_jpeg(BYTE array[512]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image.raw\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");

    if (fopen(argv[1], "r") == NULL)
    {
        printf("Invalid File. Bye.\n");
        return 1;
    }

    int jpeg_count = 0;
    char filename[8];       // array used to store the name of the jpeg file includin NULL char
    FILE *test = card;
    int test_counter = 0;

    while(test_counter == 0)
    {
        if (new_jpeg(bytes))
        {
            sprintf(filename, "%03i.jpg", jpeg_count);      //creates 00i.jpg in array filename
            FILE *first_jpeg = fopen(filename, "a");        //opens the above jpeg file
            fwrite(bytes, 512, sizeof(BYTE), first_jpeg);   //writes to the jpeg file
            jpeg_count++;
            *test = *first_jpeg;

            while(true)
            {
                fread(bytes, 512, sizeof(BYTE), card);
                if (!new_jpeg(bytes))
                {
                    fwrite(bytes, 512, sizeof(BYTE), first_jpeg);
                }
                else
                {
                    test_counter++;
                    break;
                }
            }
        }
    }

    int test_counter2 = 0;
    while(test_counter < 51)       //fread == no. of blocks of 512 bytes read
    {
        fread(bytes, 512, sizeof(BYTE), card);
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            fclose (test);
            sprintf(filename, "%03i.jpg", jpeg_count);
            FILE *jpeg = fopen(filename, "a");
            jpeg_count++;
            *test = *jpeg;
        }
            fwrite(bytes, 512, sizeof(BYTE), test);
            test_counter2++

    }
    fclose(test);
    return 1;
}

bool new_jpeg(BYTE array[512])
{
    if (array[0] == 0xff && array[1] == 0xd8 && array[2] == 0xff && (array[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//          check50 cs50/problems/2020/x/recover

//          debug50 test2 card.raw

