#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef uint8_t BYTE;
BYTE bytes[512];      // buffer to store card read data


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
    int test_counter = 0;

    FILE *jpeg = NULL;

    while (test_counter == 0)
    {
        fread(bytes, sizeof(BYTE), 512, card);

        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", jpeg_count);      //creates 00i.jpg in array filename
            jpeg = fopen(filename, "a");                    //opens the above jpeg file
            fwrite(bytes, sizeof(BYTE), 512, jpeg);         //writes to the jpeg file
            jpeg_count++;

            while (true)
            {
                fread(bytes, sizeof(BYTE), 512, card);

                if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
                {
                    fclose(jpeg);
                    sprintf(filename, "%03i.jpg", jpeg_count);
                    jpeg = fopen(filename, "a");
                    jpeg_count++;
                    test_counter++;
                    fwrite(bytes, sizeof(BYTE), 512, jpeg);
                    break;
                }
                else
                {
                    fwrite(bytes, sizeof(BYTE), 512, jpeg);
                }
            }
        }
    }

    while (fread(bytes, sizeof(BYTE), 512, card) != 0)
    {
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            fclose(jpeg);
            sprintf(filename, "%03i.jpg", jpeg_count);
            jpeg = fopen(filename, "a");
            jpeg_count++;
        }

        fwrite(bytes, sizeof(BYTE), 512, jpeg);
    }

    fclose(jpeg);
    return 0;
}


//          check50 cs50/problems/2020/x/recover

//          debug50 recover card.raw

