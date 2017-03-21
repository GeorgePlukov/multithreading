#ifndef CLIENT_H
#define CLIENT_H

/* function declarations */
void validate_arguments();
int init();
int run();
void teardown();


const int MIN_SLEEP_TIME = 100000000;
const int MAX_SLEEP_TIME = 500000000;
const char* SEGMENT_FINISH = "-";
const char* SERVER_MOORE_IP = "130.113.68.130";
const char* SERVER_MILLS_IP = "130.113.68.9";
const char* OUTPUT_FILE = "out.txt";

/* globals for the string values */
int max_str_size;
int str_position;
char* S;
char c0,c1,c2;

/* gloabls for the segment values */
int segment_length;
int num_segments;

int thread_count;
int property_index;
int tot_num_passed_segments;
char* host_name1;
char* host_name2;
#endif
