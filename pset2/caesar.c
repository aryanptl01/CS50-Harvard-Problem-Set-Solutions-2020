#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int main(int argc, string argv[])
{
    if (argc != 2)                      // getting a valid key at command line
    {
        printf("Usage: ./caesar key\n");
        return 1;                       // stopprogrammusingreturn1wheninputinvalid
    }

    int k = atoi(argv[1]);                // key obtained

    if (k < 0 || k > 1000)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int j = 0; j < strlen(argv[1]); j++)
    {

        if (argv[1][j] > 57 || argv[1][j] < 48)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    string p = get_string("plaintext: ");

    char c[strlen(p)];                           // ciphertext

    printf("ciphertext: ");

    for (int i = 0, n = strlen(p); i < n; i++)
    {

        if (isupper(p[i]))
        {
            p[i] = p[i] - 65;                // converting to alphabetical index
            c[i] = (p[i] + k) % 26 + 65;
            printf("%c", c[i]);
        }

        else if (islower(p[i]))
        {
            p[i] = p[i] - 97;                // converting to alphabetical index
            c[i] = (p[i] + k) % 26 + 97;
            printf("%c", c[i]);
        }

        else
        {
            c[i] = p[i];
            printf("%c", c[i]);
        }
    }

    printf("\n");

}
