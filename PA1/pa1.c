#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
// Command line arguments:
//      /pal.x i N L M c0 c1 c2
// i: index of the property Fi which each segment of S needs to satisfy
// N: number of threads
// L: length of each S
// M: number of segments in S
// c0,c1,c2: letters to be used in property check

// Global thread variables
int thread_count;

int i = 0;
int N = 3;
int L = 1;
int M = 6;
char c0;
char c1;
char c2;

String
// A function that determines of a string is numeric
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
        // Check for correct number of arguments
        if( argc ==  8) {
                // Confirm inputs are of correct type
                for (size_t i = 1; i < 8; i++) {
                        // Check if first four are digits
                        if (i <= 4 && !isNumeric(argv[i])) {
                                printf("[ERROR] First four args must be numbers. \n");
                                exit(0);
                        }
                        // Check if next three are alphabet characters
                        else if (i > 4 && !isalpha(argv[i][0])) {
                                printf("[ERROR] Arguments 5,6,7 must be letters. \n" );
                                exit(0);
                        }
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
        for (size_t i = 1; i <= 4; i++) {
                int curr_arg = atoi (argv[i]);
                if (!(curr_arg >= correct_lower[i-1] && curr_arg <= correct_upper[i-1])) {
                        printf("[ERROR]: Argument %d must be range %d to %d\n", i, correct_lower[i-1], correct_upper[i-1]);
                        exit(0);
                }
                result[i] = curr_arg;
        }

        // Store integer results into variables
        i = results[0];
        N = results[1];
        L = results[2];
        M = results[3];

}
