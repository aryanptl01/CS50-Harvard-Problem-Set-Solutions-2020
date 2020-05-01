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
        printf("Usage: ./caesar KEY\n");
        return 1;                       // stopprogrammusingreturn1wheninputinvalid
    }

    if (strlen(argv[1]) != 26)
    {
        printf("KEY must contain 26 letters.\n");
        return 1;
    }

    for (int i = 0; i < 26; i++)
    {
        if (isalpha(argv[1][i]) == 0)           // isalpha returns 1 or 0
        {
            printf("KEY must contain alphabets only.\n");
            return 1;
        }

    }

    char keyletter[26];

    for (int i = 0; i < 26; i++)
    {
        keyletter[i] = argv[1][i];
    }

    for (int i = 0; i < 26; i++)                     //to check repetitions
    {
        for (int j = 1; j < 26 - i; j++)            // fixing one char of key and comparing it to all others
        {
            if (keyletter[i] == keyletter[i + j])
            {
                printf("KEY should not contain repeated characters.\n");
                return 1;
            }
        }

    }

    string p = get_string("plaintext: ");

    int plength = strlen(p);

    char c[plength];

    printf("ciphertext: ");


    for (int i = 0; i < plength; i++)       // enciphering plaintext
    {
        int letter = p[i];

        if (isupper(p[i]))
        {
            c[i] = toupper(argv[1][letter - 65]);       // making sure letter case of key wont matter
            printf("%c", c[i]);
        }

        if (islower(p[i]))
        {
            c[i] = tolower(argv[1][letter - 97]);
            printf("%c", c[i]);
        }

        if (isalpha(p[i]) == 0)                         // non alpha chars remain unaffected
        {
            c[i] = p[i];
            printf("%c", c[i]);
        }
    }
    printf("\n");
    return 0;
}
