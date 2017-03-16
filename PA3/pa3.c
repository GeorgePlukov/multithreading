
#include <stdio.h>
#include <stdlib.h>
// #include <omp.h>
#include "msg.h"
#include <math.h>
#include "pa3.h"


struct s_string s_str;
struct s_segment s_seg;

int thread_count;
int property_index;
int tot_num_passed_segments;

int nc0 = 0;
int nc1 = 0;
int nc2 = 0;
int currSeg = 0;

char * server = "130.113.68.9";


int main(int argc, char** argv) {


        property_index     = atoi(argv[1]);// F0, F1, F2, or F3
        thread_count       = atoi(argv[2]);// N which is also the size
        s_seg.segment_length = atoi(argv[3]); // L
        s_seg.num_segments   = atoi(argv[4]);// M
        s_str.c0       = argv[5][0];
        s_str.c1       = argv[6][0];
        s_str.c2       = argv[7][0];

        CLIENT* clnt = clnt_create(server, MESSAGEPROG, PRINTMESSAGEVERS, "udp");
        if (clnt == (CLIENT *) NULL) {
                clnt_pcreateerror(server);
                exit(1);
        }
        char * message = "test message";
        int* result = printmessage_1(&message,clnt);
        if(result == (int * ) NULL) {
                clnt_perror(clnt,server);
                exit(1);
        }
        if (*result == 0) {
                fprintf(stderr, "%s\n", "test print 1.2.3.");
                exit(1);
        }
        clnt_destroy(clnt);
        //
        // check_arguments();
        //fi
        // init();
        //
        // write_output();


        free(s_str.S);

        return 0;
}
//
//
// //quick check to see if arguments are valid.
// void check_arguments() {
//
//         int valid = 1;
//         if (thread_count < 3 || thread_count > 8) {
//                 printf("fail one \n");
//                 valid = 0;
//         }
//         if (property_index < 0 || property_index > 3) {
//                 printf("fail two \n");
//                 valid = 0;
//         }
//         if (s_seg.segment_length < 1 || s_seg.num_segments < 1) {
//                 printf("fail 3 \n");
//                 valid = 0;
//         }
//
//         if ( (s_seg.segment_length % 2 == 1) && thread_count == 3 && (property_index == 0 || property_index == 3 )) {
//                 printf("Impossible configuration:  3 letters and odd number of spaces per segment\n");
//                 exit(0);
//         }
//
//         if ((s_seg.segment_length <= 1) && thread_count == 3 && (property_index == 0 || property_index == 1 || property_index == 3 )) {
//                 printf("Impossible configuration\n");
//                 exit(0);
//         }
//         if ( (s_seg.segment_length < 2) &&  property_index == 2 && thread_count == 3) {
//                 printf("Impossible configuration: Property 3 needs segments of at least 2. Segments of 2 need an alphabet greater than 4\n");
//                 exit(0);
//         }
//         if (!valid) {
//                 printf("invalid arguments given\n");
//                 exit(0);
//         }
//
// }
//
//
//
// void init() {
//
//         //for generating random numbers
//         srand(time(NULL));
//
//         s_str.max_str_size = s_seg.segment_length*s_seg.num_segments; // |S| = M*L
//         s_str.S = calloc(s_str.max_str_size + 1, sizeof(char));
//
//         //min number of segments each thread must check
//         s_seg.segments_per_thread = (int) ceil((double)s_seg.num_segments/thread_count);
//
//
//     #pragma omp parallel num_threads(thread_count) reduction(+ : tot_num_passed_segments)
//         {
//                 construct_string();
//                 distribute_segments();
//         }
//
//
// }
//
//
// void write_output() {
//
//         FILE *f = fopen(OUTPUT_FILE, "w");
//         fprintf(f, "%s\n", s_str.S);
//         fprintf(f, "%d\n",tot_num_passed_segments );
//         fclose(f);
//
//         printf("%s\n",s_str.S);
//         printf("number of passed segments: %d \n", tot_num_passed_segments);
// }
//
//
// //given the current character, check if we can add it to the current segment
// int enforce(int index, int seg, char c) {
//
//         int emptySpots = s_seg.segment_length - index;
//
//         //enforce property F0
//         if (property_index == 0) {
//                 return propertyZero(emptySpots,  nc0,  nc1,  nc2, c,  s_str.c0,  s_str.c1,  s_str.c2);
//         }
//
//         //enforce property F1
//         if (property_index == 1) {
//                 return propertyOne(emptySpots, thread_count,s_seg.segment_length,  nc0,  nc1,  nc2, c,  s_str.c0,  s_str.c1,  s_str.c2);
//         }
//
//         //enforce property F2
//         if (property_index == 2) {
//                 return propertyTwo(emptySpots,  nc0,  nc1,  nc2, c,  s_str.c0,  s_str.c1,  s_str.c2);
//         }
//
//         //enforce property F3 by using F0
//         if (property_index == 3) {
//                 return propertyZero(emptySpots,  nc2,  nc1,  nc0, c,  s_str.c2,  s_str.c1,  s_str.c0);
//         }
//
//
//         return -1;
// }
//
//
// //build the string according to the rules given, and check validity of each segment once string is built
// void construct_string() {
//         //assign the designated character to a thread based on it's rank
//         int rank = omp_get_thread_num();
//         char c = 'a' +  rank;
//
//         //position a thread must write  it's  character to.
//         int myPosition;
//         int myIndex;
//         int enforceResponse = 1;
//
//         struct timespec sleepTime;
//         sleepTime.tv_sec = 0;
//
//         while (1) {
//
//
//                 sleepTime.tv_nsec = MIN_SLEEP_TIME + rand() % MAX_SLEEP_TIME;
//                 nanosleep(&sleepTime, NULL);
//
//                 //once a thread acquires the stringLock, give the next open position on the string
//                 //and enforce the property
//         #pragma omp critical
//                 myPosition = s_str.str_position;
//
//                 if (myPosition >= s_str.max_str_size) {
//                         break;
//                 }
//
//
//                 if (s_str.str_position % s_seg.segment_length == 0 && enforceResponse != 0) {
//                         currSeg++;
//
//                 }
//                 enforceResponse = 0;
//
//                 while (enforceResponse == 0) {
//                         enforceResponse = enforce(myPosition % s_seg.segment_length, currSeg,c);
//                 }
//
//                 if (enforceResponse == 1) {
//             #pragma omp critical
//                         {
// //		printf("adding char %c at position %d on string %s.\n", c, myPosition, s_str.S);
//                                 s_str.S[myPosition] = c;
//                                 if (c == s_str.c0) {
//                                         ++nc0;
//                                 }
//                                 if (c == s_str.c1) {
//                                         ++nc1;
//                                 }
//                                 if (c == s_str.c2) {
//                                         ++nc2;
//                                 }
//                                 ++s_str.str_position;
//                         }
//
//                 }
//
//
//                 //once string is full, thread quits loop and starts checking properties
//                 if (myPosition >= s_str.max_str_size) break;
//         }
// }
//
//
// // give out a segment for the thread to check the property
// void distribute_segments() {
//
//         int mySegment;      // the current thread's segment to check
//         int myNumberOfSegmentsChecked = 0; // how many segments has this thread checked
//         int valid_segment = 0;      // acts as a boolean value
//
//         while (1) {
//
//
//       #pragma omp critical
//                 {
//                         mySegment = s_seg.next_segment_to_check;
//                         s_seg.next_segment_to_check++;
//                 }
//
//                 //if the number of threads is less than number of segments,
//                 //it is possible that some threads will check more segments than other threads
//                 //this condition allows us to stop checking segments if we have checked them all (note: first segment starts at 0)
//                 if (mySegment >= s_seg.num_segments) {
//                         break;
//                 }
//
//
//                 valid_segment = check_property(mySegment);
//                 myNumberOfSegmentsChecked += 1;
//
//                 tot_num_passed_segments += valid_segment;
//
//                 //each thread must verify at most M/N segments
//                 if (myNumberOfSegmentsChecked >= s_seg.segments_per_thread ) {
//                         break;
//                 }
//
//
//         }
// }
//
//
// //given a segment index, 0..M-1, check if it satisfies the property
// int check_property(int segment) {
//
//         size_t i;
//         int startPoint = segment*s_seg.segment_length;   // where the current segment starts
//         int endPoint   = (segment + 1)*s_seg.segment_length;// where the current segment ends
//         int numC0 = 0, numC1 = 0, numC2 = 0;
//
//
//         //count how many occurrences of each c0,c1,c2 in the given segment
//     #pragma omp parallel for num_threads(thread_count) \
//         reduction(+: numC0, numC1, numC2) \
//         schedule(static,1)
//         for ( i = startPoint; i < endPoint; i++) {
//                 if (s_str.S[i] == s_str.c0) {
//                         numC0++;
//                 }
//                 else if (s_str.S[i] == s_str.c1) {
//                         numC1++;
//                 }
//                 else if (s_str.S[i] == s_str.c2) {
//                         numC2++;
//                 }
//
//         }
//
//         //check the necessary property condition
//
//         if (property_index == 0 && (numC0 + numC1) == numC2 ) {
//                 return 1;
//         }
//
//         else if (property_index == 1 && (numC0 + 2*numC1) == numC2 ) {
//                 return 1;
//         }
//
//         else if (property_index == 2 && (numC0 * numC1) == numC2 ) {
//                 return 1;
//         }
//
//         else if (property_index == 3 && (numC0 - numC1) == numC2 ) {
//                 return 1;
//         }
//         return 0;
// }
