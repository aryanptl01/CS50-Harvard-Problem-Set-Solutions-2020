#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


int main(void)
{
    int letters = 0;
    int words = 0;
    int sentences = 0;

    string text = get_string("Text to be analysed: \n");

    int n = strlen(text);

    // for count of letters
    for (int i = 0; i <= n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    // for words
    for (int i = 0; i <= n; i++)
    {
        if ((isspace(text[i])))
        {
            words++;
        }
    }
    words++;     // as condition is space after word which is not the case for the last word

    // for sentences
    for (int i = 0; i <= n; i++)                    // for accessing each element of an element of the array
    {
        if ((text[i] == 46) || (text[i] == 33) || (text[i] == 63))   // space - 32, ! - 33, ? - 63, . - 46
        {
            sentences++;
        }
    }

    float L = ((letters * 100.00) / words);
    float S = ((sentences * 100.00) / words);

    float answer = 0.0588 * L - 0.296 * S - 15.8;

    answer = round(answer);
    int final = answer;

    if (answer < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (answer > 16 || answer == 16)
    {
        printf("Grade 16+\n");
    }

    else if (answer >= 1 || answer < 16)
    {
        printf("Grade %i\n", final);
    }
}
