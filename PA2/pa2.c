// This was a collaboration between:
// George Plukov (plukovga, 1316246)

// ./pa2.x 0 5 3 3 a b c
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
        // int correct_lower[4] = {0, 3, 0, 0};
        // int correct_upper[4] = {3, 8, 1000, 1000};
        // // Defaults
        // int result[4] = {0,3,1,6};


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
        printf("%i\n", M*L);
        int curr_char = 0;
        int balance = 0;

        // Start N threads
        # pragma omp parallel num_threads(N) default(none) shared(S,M,L,c0,c1,c2, curr_char,balance) private (i_property)
        {

                // int rank = omp_get_thread_num();
                char rank_char = (char) omp_get_thread_num() + 97;

                for (;; ) {

                        // How to make it work by verifying
                        // Gen rand 100 - 500
                        int r = rand() % 400 +100;
                        usleep(r * 1000);
                        // if (i_property == 0) {
                        //         if (rank_char == c0 ) {
                        //                 if (balance > 0) {
                        //                         while(balance > 0) {}
                        //                 }else if (balance < 0) {
                        //
                        //                 }
                        //         } else
                        // }
                        // Check the length of the string to see if a new char should be appended
                        if(curr_char < M*L) {

                                // pthread_mutex_lock(&mutex_S);
                                // updateTotals(i_rank);
                                // Add 97 so we get the char associated with that thread 97=a, 98=b...
                                #pragma omp critical (S)
                                {
                                        S[curr_char] = rank_char;
                                        curr_char++;
                                }
                                // if (i_property == 0) {
                                //         if (rank_char == c0 || rank_char == c1) {
                                //         #pragma omp atomic
                                //                 balance +=1;
                                //         }else if (rank_char == c2) {
                                //         #pragma omp atomic
                                //                 balance -=1;
                                //         }
                                // }

                                // printf("[THREAD %d] thread: %d of %d. Wait:%i miliseconds\n", rank, rank, M, r);

                                // pthread_mutex_unlock(&mutex_S);
                        }else {
                                break;
                        }
                }
        }

        # pragma omp barrier

        for (size_t i = 0; i < L*M; i++) {
                printf("%c", S[i]);
        }
        printf("\n");
        int segment_counter = 0;
        int begin_ind = 0;
        bool loop = true;


        int c0_count= 0;
        int c1_count= 0;
        int c2_count1= 0;


        int correct_segments = 0;
        int test = 0;
        int incorrect_segments = 0;
        // // Verify strings
        # pragma omp parallel num_threads(N) default(none) shared(i_property,S,M,L,c0,c1,c2,segment_counter, correct_segments, incorrect_segments) private (loop, begin_ind, c0_count,c1_count, c2_count1, test)
        {
                int rank = omp_get_thread_num();
                // printf("asdasdasd\n");
                loop = true;
                c0_count= 0;
                c1_count= 0;
                c2_count1= 0;
                begin_ind = 0;
                // run loop while there are segments to consume
                while(loop) {
                        c0_count= 0;
                        c1_count= 0;
                        c2_count1= 0;
                        // printf("[THREAD %i] segment_ counter: %i\n",rank, segment_counter);
                        #pragma omp critical(segment)
                        {
                                if (segment_counter < L*M -1) {

                                        begin_ind = segment_counter;
                                        // printf("[THREAD %i] begin: %i seg:%i \n",rank, begin_ind,segment_counter);
                                        segment_counter += L;
                                }else{
                                        loop = false;
                                }
                        }
                        // If the loop ends dont do it
                        if (loop) {
                                // Count the items in a segment
                                for (int i = 0; i < L; i++) {
                                        if (S[begin_ind + i ] == c0) {
                                                #pragma omp atomic
                                                c0_count++;
                                                // printf("[THREAD %i] curr: %c == %c \n", rank, S[begin_ind +i], c0);

                                        }else if (S[begin_ind + i] == c1) {
                                                #pragma omp atomic
                                                c1_count++;
                                                // printf("[THREAD %i] curr: %c == %c \n", rank, S[begin_ind +i], c1);

                                        }else if (S[begin_ind + i ] == c2) {
                                                #pragma omp atomic
                                                c2_count1++;
                                                // printf("[THREAD %i] curr: %c == %c \n", rank, S[begin_ind +i], c2);

                                        }
                                }

                                // int c2_count1 = 10;
                                // printf("[THREAD %i] c0: %i c1: %i c2: %i \n", rank,c0_count,c1_count,c2_count1);


                                c2_count1 = 10;
                                if (i_property == 0) {
                                        if (c0_count + c1_count == c2_count1) {
                                                // correct segment
                                                #pragma omp atomic
                                                correct_segments++;
                                                // printf("%s\n", );
                                        }else{
                                                # pragma omp atomic
                                                incorrect_segments++;

                                        }

                                } else if (i_property == 1) {
                                        if (c0_count + 2 * c1_count == c2_count1) {

                                                # pragma omp atomic
                                                correct_segments++;
                                        }else{
                                                # pragma omp atomic
                                                incorrect_segments++;
                                        }
                                }else if (i_property == 2) {
                                        if (c0_count * c1_count == c2_count1) {
                                                // correct segment
                                                # pragma omp atomic
                                                correct_segments++;
                                        }else{
                                          # pragma omp atomic
                                                incorrect_segments++;
                                        }
                                }else if (i_property == 3) {
                                        if (c0_count - c1_count == c2_count1) {
                                                // correct segment
                                                # pragma omp atomic
                                                correct_segments++;
                                        }else{
                                                # pragma omp atomic
                                                incorrect_segments++;
                                        }
                                }
                        }
                }
        }
        // printf("verifyied + corrseg: %i + inc: %i\n", correct_segments, incorrect_segments);
// Print string at end
        printf("correct_segments: %i\n", correct_segments);
        printf("cinorrect_segments: %i\n", incorrect_segments);

        for (size_t i = 0; i < L*M; i++) {
                printf("%c", S[i]);
        }
        printf("\n");
        return 0;
}

















// printf("%i\n", c0_count);
// Check to see if the segment has been satisfied

// if (i_property == 0) {
//         if (c0_count + c1_count == c2_count1) {
//                 // correct segment
//                 #pragma omp atomic
//                 correct_segments++;
//                 // printf("%s\n", );
//         }else{
//                 # pragma omp atomic
//                 incorrect_segments++;
//
//         }
//
// } else if (i_property == 1) {
//         if (c0_count + 2 * c1_count == c2_count1) {
//
//                 # pragma omp atomic
//                 correct_segments++;
//         }else{
//                 # pragma omp atomic
//                 incorrect_segments++;
//         }
// }else if (i_property == 2) {
//         if (c0_count * c1_count == c2_count1) {
//                 // correct segment
//                 # pragma omp atomic
//                 correct_segments++;
//         }else{
//           # pragma omp atomic
//                 incorrect_segments++;
//         }
// }else if (i_property == 3) {
//         if (c0_count - c1_count == c2_count1) {
//                 // correct segment
//                 # pragma omp atomic
//                 correct_segments++;
//         }else{
//                 # pragma omp atomic
//                 incorrect_segments++;
//         }
// }
