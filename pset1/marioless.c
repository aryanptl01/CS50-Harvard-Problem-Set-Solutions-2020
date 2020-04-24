#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");  //used for user prompt. Will prompt until no. obtained is in desired range
    }
    while (n < 1 || n > 8);

    for (int i = 0; i < n; i++) // "i" will handle, say, rows 
    {
        for (int j = n - 1; j > i; j--) //"j"here will control the elments inside of the                                             corresponding row (loop)
        {  
            printf(" ");                           
        }

        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
        
}    
