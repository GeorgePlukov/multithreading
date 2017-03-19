#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include "append.h"
#include "verify.h"


/* function declarations */
void validate_arguments();
int init();
int run();
void teardown();



/* globals for the string values */
int max_str_size;
int str_position;
char* S;
char c0,c1,c2;

/* gloabls for the segment values */
const char* SEGMENT_FINISH = "-";
int segment_length;
int num_passed_segments;
int num_segments;

int thread_count;
int property_index;
int tot_num_passed_segments;
int MIN_SLEEP_TIME = 100000000;
int MAX_SLEEP_TIME = 500000000;

//temporary. this value would ideally be passed as arg
char* server_moore = "130.113.68.130";
char* server_mills = "130.113.68.9";

//append and verify clients
CLIENT *clnt_app, *clnt_ver;

//will hold all argumests as CSV
char* all_args;



int main(int argc, char ** argv) {

	property_index = atoi(argv[1]);
	thread_count   = atoi(argv[2]);
	num_segments   = atoi(argv[3]);
	segment_length = atoi(argv[4]);
	c0 = argv[5][0];
	c1 = argv[6][0];
	c2 = argv[7][0];
	char* host_name1 = argv[8];
	char* host_name2 = argv[9];
	max_str_size = num_segments*segment_length;
	all_args = malloc(sizeof(int)*4 + sizeof(char)*3 + sizeof(char*));
	sprintf(all_args, "%d,%d,%d,%d,%c,%c,%c,%s", \
					property_index,thread_count,num_segments,segment_length,c0,c1,c2,host_name2);


	//ideally we would want to validate the arguments here
	validate_arguments();
	

	//now we can initialize the program.
	if (init() == -1) {
		printf("program failed to initialize\n");
		return 1;
	}

	//start the program
	#pragma omp parallel num_threads(thread_count) reduction(+ : tot_num_passed_segments)
    {
    	run();
    }
	

	teardown();


	clnt_destroy(clnt_app);
	clnt_destroy(clnt_ver);
	return 1;
}



void validate_arguments() {
	return;
}

int init() {

	//for generating random numbers
    srand(time(NULL));

    // S is the final generated string
    S = calloc(max_str_size + 1, sizeof(char));

	int *result;

	/****  setup RPC init append server ****/
	clnt_app = clnt_create(server_moore,APPENDPROG,APPENDVERS,"udp");
	if (clnt_app == (CLIENT *)NULL) {
		clnt_pcreateerror(server_moore);
		return -1;
	}
	
	result = rpcinitappendserver_1(&all_args, clnt_app);
	if (result == (int *) NULL) {
		clnt_perror(clnt_app,server_moore);
		return -1;
	}


	/****  setup RPC init verify server ****/
	clnt_ver = clnt_create(server_moore,VERIFYPROG,VERIFYVERS,"udp");
	if (clnt_ver == (CLIENT *)NULL) {
		clnt_pcreateerror(server_moore);
		return -1;
	}

	result = rpcinitverifyserver_1(&all_args, clnt_ver);
	if (result == (int *) NULL) {
		clnt_perror(clnt_ver,server_moore);
		return -1;
	}



	return 1;
}

int run() {

	int rank = omp_get_thread_num();
	char c = 'a' +  rank;
	char* ptr_c = &c;

	struct timespec sleepTime;
	sleepTime.tv_sec = 0;

	int success = 1;

	/**  begin to attempt adding character c to the string in the Append_Server **/
	while (success >= 0) {	
		sleepTime.tv_nsec = MIN_SLEEP_TIME + rand() % MAX_SLEEP_TIME;
		nanosleep(&sleepTime, NULL);

		//RPC to try an append string
		// success == 0, c was added
		// success == 1, c was NOT added
		// success == -1, S is complete
		success = (int*) rpcappend_1(&ptr_c, clnt_app);

		printf("thread %d received success %d\n", rank, success );
	}


	/** String in append server is complete, begin to check segments **/

	char* segment = "";
	int seg_index = -1;
	int valid_segment = 0;


	while ( strcmp(segment,SEGMENT_FINISH) != 0 ) {

		//rpcgetseg retuns "i,<seg>" where i is the index of seg
		sscanf((char*)rpcgetseg_1(&rank,clnt_ver),"%d,%s",&seg_index, &segment);
		printf("thread %d got segment %s with index %d\n",rank, segment, seg_index);
		
		//verify the segment is valid 
		valid_segment =  check_property(segment);

		if (valid_segment) {
			tot_num_passed_segments += valid_segment;
			strcat(S,segment);
		}
	}

	return 1;
}

















//given a segment index, 0..M-1, check if it satisfies the property
int check_property(char* seg) {

    size_t i;
    int num_c0 = 0, num_c1 = 0, num_c2 = 0;	


    //count how many occurrences of each c0,c1,c2 in the given segment
    #pragma omp parallel for num_threads(thread_count) \
    	reduction(+: num_c0, num_c1, num_c2) \
    	schedule(static,1)
    for ( i = 0; i < segment_length; i++) {
    	if (seg[i] == c0) {
    		num_c0++;
    	}
    	else if (seg[i] == c1) {
    		num_c1++;
    	}
    	else if (seg[i] == c2) {
    		num_c2++;
    	}


    }

    //check the necessary property condition

    if (property_index == 0 && (num_c0 + num_c1) == num_c2 ) {
    	return 1;
    }

    else if (property_index == 1 && (num_c0 + 2*num_c1) == num_c2 ) {
    	return 1;
    }

    else if (property_index == 2 && (num_c0 * num_c1) == num_c2 ) {
    	return 1;
    }

    else if (property_index == 3 && (num_c0 - num_c1) == num_c2 ) {
    	return 1;
    }
    return 0;
}

void teardown() {
	clnt_destroy(clnt_app);
	clnt_destroy(clnt_ver);
	free(all_args);
	free(S);
}