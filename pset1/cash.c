#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float d;                  // "f" is the user input which is in dollars (eg. $9.75)
    do
    {
        d = get_float("Change: ");
    }
    while (d < 0);

    int c = round(d * 100);      // "c" is in cents (dollar * 100). Dollars recieved will be rounded up if required

    int t;
    t = c - c % 25;             // "f" is no. of 25 cent coins required
    int f = t / 25;             // "t" is amt. covered using 25 cent coins 

    c = c - f * 25;              // equating "c" to amt. left after using 25 cent coins

    int e;                     // "n" is no. of 10 cent coins required
    e = c - c % 10;             
    int n = e / 10;             // "e" is amt. covered using 25 and 10 cent coins

    c = c - n * 10;              // equating "c" to amt. left after using 10 cent coins


    int v;                      // "r" is no. of 5 cent coins required
    v = c - c % 5;              
    int r = v / 5;               // "v" is amt. covered using 5 cent coins 

    c = c - r * 5;                // equating "c" to amt. left after using 25, 10 and 5 cent coins

    int o;                      // "u" is no. of 1 cent coins required
    o = c - c % 1;              
    int u = o / 1;               // "o" is amt. covered using 1 cent coins 

    c = c - u * 1;                // equating "c" to amt. left after using 25, 10, 5 and 1 cent coins

    int q = r + u + n + f;

    printf("%i", q);

    printf("\n");

}
