#include <stdio.h>
#include <stdlib.h>
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
int segment_length;
int num_passed_segments;
int num_segments;

int thread_count;
int property_index;

//temporary. this value would ideally be passed as arg
char* server_moore = "130.113.68.130";
char* server_mills = "130.113.68.9";

//append and verify clients
CLIENT *clnt_app, *clnt_ver;

//will hold all argumests as CSV
char* all_args;



int main(int argc, char ** argv) {


	int *result;

	property_index = atoi(argv[1]);
	thread_count   = atoi(argv[2]);
	num_segments   = atoi(argv[3]);
	segment_length = atoi(argv[4]);
	c0 = argv[5][0];
	c1 = argv[6][0];
	c2 = argv[7][0];
	char* host_name1 = argv[8];
	char* host_name2 = argv[9];

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
	run();

	teardown();


	clnt_destroy(clnt_app);
	clnt_destroy(clnt_ver);
	return 1;
}



void validate_arguments() {
	return;
}

int init() {
	int result;

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
	return 1;
}


void teardown() {
	clnt_destroy(clnt_app);
	clnt_destroy(clnt_ver);
	free(all_args);
}