#include <stdio.h>
#include <time.h>

#include <stdlib.h>

#include "choose.h"

/* choose - randomly pick from a list
 * Arguments:
 *  choicec - number of choices
 *  choicev - array of percentage chances for each choice (must add up to 100)
 * Returns the index of the selected choice, -1 if the chances don't add up to
 * 100, or -2 if a critical error occurred that should never happen. The
 * function uses random(3) internally, which can be seeded with srandom(3).
 *
 * Example usage:
 *
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <time.h>
 * ...
 * const char *choices = { "Up the Downstair", "The Sky Moves Sideways",
 *                         "Signify", "Stupid Dream" };
 * const int chances[] = { 10, 20, 30, 40 };
 * const int nchoices = sizeof choices / sizeof *choices;
 * int choice;
 * srandom(time(NULL));
 * choice = choose(nchoices, chances);
 * printf("Let's listen to %s.\n", choices[choice]);
 */
int choose(int choicec, const int *choicev) {
    int i;
    long r, sum = 0;

    for(i = 0; i < choicec; i++)
        sum += choicev[i];
    if(sum != 100)
        return -1;

    r = random() % 100;
    for(sum = 0, i = 0; i < choicec; sum += choicev[i], i++)
        if(r < choicev[i] + sum)
            return i;

    /* We should NEVER get here. */
    return -2;
}

#if 0
/* Reference test */
int main() {
    const long rounds = 10000000;
    const int choices[] = { 30, 10, 10, 50 };
    const int nchoices = sizeof choices / sizeof *choices;
    unsigned long count[] = { 0, 0, 0, 0 };
    int c;
    long i;

    srandom(time(NULL));
    for(i = 0; i < rounds; i++) {
        c = choose(nchoices, choices);
        if(c < 0 || c >= nchoices) {
            printf("round %ld: choose(%d, %p) = %d\n", i, nchoices, choices, c);
            return 1;
        }
        count[c]++;
    }

    for(i = 0; i < nchoices; i++)
        printf("Choice %ld: %2lu%% (%7lu/%ld)\n", i, count[i] * 100 / rounds, count[i], rounds);

    return 0;
}
#endif

