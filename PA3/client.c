#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include "append.h"
#include "verify.h"
#include "client.h"


//append and verify clients
CLIENT *clnt_app, *clnt_ver;

//will hold all argumests as CSV
static char* all_args;



int main(int argc, char ** argv) {

	property_index = atoi(argv[1]);
	thread_count   = atoi(argv[2]);
	segment_length = atoi(argv[3]);
	num_segments   = atoi(argv[4]);
	c0 = argv[5][0];
	c1 = argv[6][0];
	c2 = argv[7][0];
	host_name1 = argv[8];
	host_name2 = argv[9];
	max_str_size = num_segments*segment_length;
	all_args = malloc(sizeof(int)*4 + sizeof(char)*3 + sizeof(char*));
	sprintf(all_args, "%d,%d,%d,%d,%c,%c,%c,%s", \
					property_index,thread_count,num_segments,segment_length,c0,c1,c2,host_name2);



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
    	check_segment();
    }

	write_output();

	teardown();

	return 1;
}



//quick check to see if arguments are valid.
void validate_arguments() {

	int valid = 1;
	if (thread_count < 3 || thread_count > 8) {
		valid = 0;
	}
	if (property_index < 0 || property_index > 3) {
		valid = 0;
	}
	if (segment_length < 1 || num_segments < 1) {
		valid = 0;
	}

	if ( (segment_length % 2 == 1) && thread_count == 3 && (property_index == 0 || property_index == 3 )) {
		printf("Impossible configuration:  3 letters and odd number of spaces per segment\n");
		exit(0);
	}

    if ((segment_length <= 1) && thread_count == 3 && (property_index == 0 || property_index == 1 || property_index == 3 )) {
        printf("Impossible configuration\n");
        exit(0);
    }
    if ( (segment_length < 2) &&  property_index == 2 && thread_count == 3){
        printf("Impossible configuration for Property 3\n");
        exit(0);
    }
	if (!valid) {
		printf("Invalid arguments given\n");
		exit(0);
	}

	if ( strcmp(host_name1,SERVER_MOORE_IP) != 0 && strcmp(host_name1,SERVER_MILLS_IP) != 0 ) {
		printf("Invalid host name 1 given. Expected one of:\n\tMoore: %s\n\tMills: %s\n",SERVER_MOORE_IP, SERVER_MILLS_IP );
		exit(0);
	}


	if ( strcmp(host_name2,SERVER_MOORE_IP) != 0 && strcmp(host_name2,SERVER_MILLS_IP) != 0 ) {
		printf("Invalid host name 2 given. Expected one of:\n\tMoore: %s\n\tMills: %s\n",SERVER_MOORE_IP, SERVER_MILLS_IP );
		exit(0);
	}

}


int init() {
	//for generating random numbers
    srand(time(NULL));

    // S is the final generated string
    S = calloc(max_str_size + 1, sizeof(char));

	int *result;

	/****  setup RPC init append server ****/
	clnt_app = clnt_create(host_name1,APPENDPROG,APPENDVERS,"tcp");
	if (clnt_app == (CLIENT *)NULL) {
		clnt_pcreateerror(host_name1);
		return -1;
	}

	result = rpcinitappendserver_1(&all_args, clnt_app);
	if (result == (int *) NULL) {
		clnt_perror(clnt_app,host_name1);
		return -1;
	}



	/****  setup RPC init verify server ****/
	clnt_ver = clnt_create(host_name2,VERIFYPROG,VERIFYVERS,"tcp");
	if (clnt_ver == (CLIENT *)NULL) {
		clnt_pcreateerror(host_name2);
		return -1;
	}

	result = rpcinitverifyserver_1(clnt_ver);
	if (result == (int *) NULL) {
		clnt_perror(clnt_ver,host_name2);
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


	int *success_ret;
	int success = 1;

	/**  begin to attempt adding character c to the string in the Append_Server **/
	while (success >= 0) {
		sleepTime.tv_nsec = MIN_SLEEP_TIME + rand() % MAX_SLEEP_TIME;
		nanosleep(&sleepTime, NULL);

		//RPC to try an append string
		// success == 0, c was added
		// success == 1, c was NOT added
		// success == -1, S is complete

		#pragma omp critical
			success_ret = rpcappend_1(&ptr_c, clnt_app);
		success = *success_ret;
	}


	return 0;
}

/** String in append server is complete, begin to check segments **/
int check_segment() {

	int rank = omp_get_thread_num();
	char* segment = malloc(sizeof(char)*(segment_length+1));
	int seg_index;
	int valid_segment = 0;


	my_struct *response = malloc(sizeof(*response));

	while ( 1 ) {

		#pragma omp critical
			response = rpcgetseg_1(segment_length,clnt_ver);

		char* q = malloc(strlen(response->data));
		q = response->data;
		sscanf(q, "%d,%s", &seg_index, segment);

		//we are done receiving segments
		if (strcmp(segment,SEGMENT_FINISH) == 0) {
			break;
		}


		//verify the segment is valid
		valid_segment = check_property(segment);
		// printf("segment %s is valid = %d. %d\n", segment, valid_segment,tot_num_passed_segments);
		
		//this is a sum reduction each thread partakes
		tot_num_passed_segments += valid_segment;


		//concat the string
		int starting_pos = seg_index*segment_length;
		size_t i,j;
		for (i = starting_pos, j = 0; j < segment_length; i++, j++) {
			S[i] = segment[j];
		}
	}
	return 1;
}






//given a segment of the string, check if it satisfies the property
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



void write_output() {
	printf("%s\n%d\n", S, tot_num_passed_segments);

	//write to out.txt
	FILE *f = fopen(OUTPUT_FILE, "w");
	fprintf(f, "%s\n", S);
	fprintf(f, "%d\n",tot_num_passed_segments );
	fclose(f);
}

void teardown() {
	clnt_destroy(clnt_app);
	clnt_destroy(clnt_ver);
	free(all_args);
	free(S);
}
