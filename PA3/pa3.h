#ifndef PA3_H
#define PA3_H

/* constants */
char* OUTPUT_FILE = "out.txt";
int MIN_SLEEP_TIME = 100000000;
int MAX_SLEEP_TIME = 500000000;


/* Organizes segment shared variables and locks */
struct s_segment {
    int next_segment_to_check;
    int segment_length;
    int num_passed_segments;
    int num_segments;
    int segments_per_thread;
};

/* organizes string shared variables and locks */
struct s_string {
    int max_str_size;
    int str_position;
    char* S;
    char c0,c1,c2;
};



/* Declarations */
void init();	
void write_output();
void check_arguments();
void construct_string();
void distribute_segments();
int check_property(int segment);


#endif