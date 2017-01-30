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


int i_property = 0;
int N = 3;
int L = 1;
int M = 6; //num seg

pthread_mutex_t mutex_curr_seg;
int curr_seg = 0;

// For counting number of each input char in a segment
char c0;
char c1;
char c2;
// // int c1_count = 0;
// // int c2_count = 0;
// // int c0_count = 0;
// pthread_mutex_t mutex_c0;
// // pthread_mutex_t mutex_c1;
// // pthread_mutex_t mutex_c2;
// // // total number f characters in a segment
// // int tot_count = 0;
// // pthread_mutex_t mutex_tot;
//
// //Total number of correct segments
int correct_segments = 0;
pthread_mutex_t mutex_correct_segment;
// //Total number of correct segments
int incorrect_segments = 0;
pthread_mutex_t mutex_incorrect_segment;
// Headers
void *threadFunc (void* rank);
void *threadCheck (void* rank);

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


        // Store integer results into variables
        i_property = atoi(argv[1]);
        N = atoi(argv[2]);;
        L = atoi(argv[3]);;
        M = atoi(argv[4]);;

        c0 = *argv[5];
        c1 = *argv[6];
        c2 = *argv[7];
        // Set string size
        // printf("Creating %d threads\n", N);
        // MULTI THREADING
        pthread_t* thread_handles;
        thread_handles =(pthread_t*) malloc(N*sizeof(pthread_t));
        for (long thread = 0; thread < N; thread++) {
                pthread_create(&thread_handles[thread], NULL, threadFunc, (void*) thread);
        }

        // printf("[MAIN] Just passing through\n");

        for (long thread = 0; thread < N; thread++) {
                pthread_join(thread_handles[thread], NULL);
        }
        // printf("String: %s length: %lu\n", S.c_str(), S.length());
        free(thread_handles);



        // Create the threads for all of the checks
        pthread_t* thread_handles_check;
        thread_handles_check =(pthread_t*) malloc(N*sizeof(pthread_t));
        for (long thread = 0; thread < N; thread++) {
                pthread_create(&thread_handles_check[thread], NULL, threadCheck, (void*) thread);
        }

        // printf("[MAIN] Just passing through part 2\n");

        for (long thread = 0; thread < N; thread++) {
                pthread_join(thread_handles_check[thread], NULL);
        }

        // printf("Number of correct segments: %d\n", correct_segments);
        free(thread_handles_check);

        // for (long i = 0; i < S.length(); i += L) {
        //         for (size_t k = 0; k < L; k++) {
        //                 printf("%c", S[i+k]);
        //         }
        //         printf("\n");
        // }

        printf("%s\n", S.c_str());
        printf("%i\n", correct_segments);

        FILE *f = fopen("out.txt", "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }

        // Print to the file
        fprintf(f, "%s\n", S.c_str());
        fprintf(f, "%i", correct_segments);

        fclose(f);
        return 0;
}

// M = num of segments
int segment_number = 0;
pthread_mutex_t mutex_segment_number;
void *threadCheck (void* rank){
        int c0_count = 0;
        int c1_count = 0;
        int c2_count = 0;
        // beginnning index of the sefment we are checking
        int begin_ind = 0;

        // Lock the segment number to make sure our check is done atomically
        pthread_mutex_lock(&mutex_segment_number);
        while (segment_number < M) {
                begin_ind = segment_number * L;
                //consume a segment
                segment_number++;
                pthread_mutex_unlock(&mutex_segment_number);

                for (int i = 0; i < L; i++) {

                        if (S[begin_ind + i ] == c0) {
                                c0_count++;
                        }else if (S[begin_ind + i] == c1) {
                                c1_count++;
                        }else if (S[begin_ind + i ] == c2) {
                                c2_count++;
                        }
                }

                // Check to see if the segment has been satisfied

                if (i_property == 0) {
                        if (c0_count + c1_count == c2_count) {
                                // correct segment
                                pthread_mutex_lock(&mutex_correct_segment);
                                correct_segments++;
                                pthread_mutex_unlock(&mutex_correct_segment);
                                // printf("%s\n", );
                        }else{
                                pthread_mutex_lock(&mutex_incorrect_segment);
                                incorrect_segments++;
                                pthread_mutex_unlock(&mutex_incorrect_segment);
                        }

                } else if (i_property == 1){
                  if (c0_count + 2 * c1_count == c2_count) {
                          // correct segment
                          pthread_mutex_lock(&mutex_correct_segment);
                          correct_segments++;
                          pthread_mutex_unlock(&mutex_correct_segment);
                          // printf("%s\n", );
                  }else{
                          pthread_mutex_lock(&mutex_incorrect_segment);
                          incorrect_segments++;
                          pthread_mutex_unlock(&mutex_incorrect_segment);
                  }
                }else if (i_property == 2){
                  if (c0_count * c1_count == c2_count) {
                          // correct segment
                          pthread_mutex_lock(&mutex_correct_segment);
                          correct_segments++;
                          pthread_mutex_unlock(&mutex_correct_segment);
                          // printf("%s\n", );
                  }else{
                          pthread_mutex_lock(&mutex_incorrect_segment);
                          incorrect_segments++;
                          pthread_mutex_unlock(&mutex_incorrect_segment);
                  }
                }else if (i_property == 3){
                  if (c0_count - c1_count == c2_count) {
                          // correct segment
                          pthread_mutex_lock(&mutex_correct_segment);
                          correct_segments++;
                          pthread_mutex_unlock(&mutex_correct_segment);
                          // printf("%s\n", );
                  }else{
                          pthread_mutex_lock(&mutex_incorrect_segment);
                          incorrect_segments++;
                          pthread_mutex_unlock(&mutex_incorrect_segment);
                  }
                }
                // printf("Segment count: %i %i %i\n", c0_count, c1_count,c2_count);
                c0_count = 0;
                c1_count = 0;
                c2_count = 0;
        }
}
void *threadFunc (void* rank){
        int i_rank = (int) ((size_t) rank);
        // printf("[THREAD %i] \n", i_rank);
        for (;; ) {
                // How to make it work by verifying
                // Gen rand 100 - 500
                int r = rand() % 400 +100;
                usleep(r * 1000);

                // Check the length of the string to see if a new char should be appended
                if(S.length() < M*L) {
                        // if (N >)
                        pthread_mutex_lock(&mutex_S);
                        // updateTotals(i_rank);
                        // Add 97 so we get the char associated with that thread 97=a, 98=b...
                        S += (char) i_rank +97;

                        // printf("[THREAD %d] thread: %d of %d. Wait:%i miliseconds\n", rank, rank, M, r);
                        // printf("%d, %s\n", i_rank, S.c_str());
                        pthread_mutex_unlock(&mutex_S);
                }else {
                        break;
                }
        }
        return NULL;
        // Wait for a random amount of time
        // printf("[THREAD %d] thread: %d of %d. my num is: %i\n", rank, rank, M, r);
}
