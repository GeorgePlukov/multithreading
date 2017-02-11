// This was a collaboration between:
// George Plukov (plukovga, 1316246)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>


// Command line arguments:
//      /pal.x i N L M c0 c1 c2
// i: index of the property Fi which each segment of S needs to satisfy
// N: number of threads
// L: length of each S
// M: number of segments in S
// c0,c1,c2: letters to be used in property check

int i_property = 0;
int N = 3;
int L = 1;
int M = 6; //num seg

// For counting number of each input char in a segment
char c0;
char c1;
char c2;

// // Headers
// void *threadFunc (void* rank);
// void *threadCheck (void* rank);
//
// // A function that determines of a string is numeric
bool isNumeric(const char *str)
{
        while(*str != '\0')
        {
                if(*str < '0' || *str > '9')
                        return false;
                str++;
        }
        return true;
}

// Programs main thread
int main(int argc, char *argv[]) {

        srand(time(NULL));

        // Check for correct number of arguments
        if( argc ==  8) {
                // Confirm inputs are of correct type
                for (int i = 1; i < 8; i++) {
                        // Check if first four are digits
                        if (i <= 4 && !isNumeric(argv[i])) {
                                printf("[ERROR] First four args must be numbers. \n");
                                exit(0);
                        }
                        // Check if next three are alphabet characters
                        // else if (i > 4 && !isalpha(argv[i][0])) {
                        //         printf("[ERROR] Arguments 5,6,7 must be letters. \n" );
                        //         exit(0);
                        // }
                }
        }
        else if(argc > 8) {
                printf("Too many arguments supplied.\n");
        }
        else {
                printf("6 arguments expected.\n");
        }

        // Input varification
        int correct_lower[4] = {0, 3, 0, 0};
        int correct_upper[4] = {3, 8, 1000, 1000};
        // Defaults
        int result[4] = {0,3,1,6};


        // Store integer results into variables
        i_property = atoi(argv[1]);
        N = atoi(argv[2]);;
        L = atoi(argv[3]);;
        M = atoi(argv[4]);;

        c0 = *argv[5];
        c1 = *argv[6];
        c2 = *argv[7];

        return 0;
}
