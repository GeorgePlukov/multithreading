// generate S
// check S to be good
// count # of segments in the string and the entire string
// LAST STEP make sure the string doesnt get genereated improperly


#include <pthread.h>
#include <stdio.h>
// #include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <time.h>
#include <string>
#include <cstring>

#include <stdbool.h>

using namespace std;
// Command line arguments:
//      /pal.x i N L M c0 c1 c2
// i: index of the property Fi which each segment of S needs to satisfy
// N: number of threads
// L: length of each S
// M: number of segments in S
// c0,c1,c2: letters to be used in property check

// Global threading variables
int thread_count;
string S = "";
pthread_mutex_t mutex_S;


int i = 0;
int N = 3;
int L = 1;
int M = 6;
char c0;
char c1;
char c2;

void *threadFunc (void* rank);

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
        for (int i = 1; i <= 4; i++) {
                int curr_arg = atoi (argv[i]);
                if (!(curr_arg >= correct_lower[i-1] && curr_arg <= correct_upper[i-1])) {
                        printf("[ERROR]: Argument %d must be range %d to %d\n", i, correct_lower[i-1], correct_upper[i-1]);
                        exit(0);
                }
                result[i] = curr_arg;
        }

        // Store integer results into variables
        i = result[0];
        N = result[1];
        L = result[2];
        M = result[3];

        // Set string size
        printf("Creating %d threads\n", M);
        // MULTI THREADING
        pthread_t* thread_handles;
        thread_handles =(pthread_t*) malloc(M*sizeof(pthread_t));
        for (long thread = 0; thread < M; thread++) {
                pthread_create(&thread_handles[thread], NULL, threadFunc, (void*) thread);
        }

        printf("[MAIN] Just passing through\n");

        for (long thread = 0; i < M; i++) {
                pthread_join(thread_handles[thread], NULL);
        }

        free(thread_handles);

        return 0;
}

void *threadFunc (void* rank){
        int i_rank = (int) ((size_t) rank);
        printf("%i\n", i_rank);
        for (size_t i = 0; i < 100; i++) {
          int r = rand() % 400 +100;
          usleep(r);

          // Check the length of the string to see if a new char should be appended
          if(S.length() < M*L){
            pthread_mutex_lock(&mutex_S);
            // Add 97 so we get the char associated with that thread 97=a, 98=b...
            S += (char) i_rank +97;

            // printf("[THREAD %d] thread: %d of %d. Wait:%i miliseconds\n", rank, rank, M, r);
            printf("%d, %s\n", i_rank, S.c_str());
            pthread_mutex_unlock(&mutex_S);
          }


        }
        return NULL;
        // Wait for a random amount of time
        // printf("[THREAD %d] thread: %d of %d. my num is: %i\n", rank, rank, M, r);
}
