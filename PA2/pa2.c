// This was a collaboration between:
// George Plukov (plukovga, 1316246)
// Ryan Lambert (lamberrj, 1218407)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
void threadFunc (void);
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
void printArray (char* a){
        for (size_t i = 0; i < sizeof(a) / sizeof(a[0])
             ; i++) {
                printf (a[i]);
        }
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


        // Store integer results into variables
        i_property = atoi(argv[1]);
        N = atoi(argv[2]); // num of threads
        L = atoi(argv[3]);
        M = atoi(argv[4]);

        c0 = *argv[5];
        c1 = *argv[6];
        c2 = *argv[7];

        char S [L*M+1];
        S[L*M + 1] = '\0';
        int curr_char = 0;
        int balance = 0;

        // Start N threads
        # pragma omp parallel num_threads(N) default(none) shared(S,M,L,c0,c1,c2, curr_char) private (i_property)


        for (;; ) {
                // How to make it work by verifying
                // Gen rand 100 - 500
                int r = rand() % 400 +100;
                usleep(r * 1000);
                // printf("TEST\n");
                // Check the length of the string to see if a n   ew char should be appended
                if(curr_char < M*L) {

                        // pthread_mutex_lock(&mutex_S);
                        // updateTotals(i_rank);
                        // Add 97 so we get the char associated with that thread 97=a, 98=b...
                        #pragma omp critical (S)
                        {
                                S[curr_char] = (char) omp_get_thread_num() +97;
                                curr_char++;
                                printf("%c\n", (char) omp_get_thread_num() +97);
                        }
                        // printf("[THREAD %d] thread: %d of %d. Wait:%i miliseconds\n", rank, rank, M, r);

                        // pthread_mutex_unlock(&mutex_S);
                }else {
                        break;
                }
        }

        for (size_t i = 0; i < L*M; i++) {
                printf("%c\n", S[i]);
        }
        return 0;
}
