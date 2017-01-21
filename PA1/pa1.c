#include <pthread.h>
#include <stdio.h>

// Command line arguments:
//      /pal.x i N L M c0 c1 c2
// i: index of the property Fi which each segment of S needs to satisfy
// N: number of threads
// L: length of each S
// M: number of segments in S
// c0,c1,c2: letters to be used in property check

// Programs main thread
int main(int argc, char *argv[]) {

        if( argc ==  8) {
                // Confirm inputs are of correct type
                for (size_t i = 1; i < 8; i++) {
                        // Check
                        if (i <= 4) {
                                if (!isdigit(argv[i][0])) {
                                        printf("First four args must be numbers. \n");
                                        break;
                                }
                        } else {
                                if (!isalpha(argv[i][0])) {
                                        printf("Arguments 5,6,7 must be letters. \n" );
                                        break;
                                }
                        }
                        // printf("The argument supplied is %s\n", argv[i]);
                }

        }
        else if( argc > 7 ) {
                printf("Too many arguments supplied.\n");
        }
        else {
                printf("6 arguments expected.\n");
        }
}
