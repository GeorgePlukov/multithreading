#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <ctype.h>
#include <omp.h>
#include "append.h"


#define BUFLEN 512  //Max length of buffer
#define PORT 8884  //The port on which to listen for incoming data

int count = 0;
char *str;
char* hn = "130.113.68.130";
int submit = 1;

int property_index, thread_count, seg_length, num_seg;
char c0, c1, c2;

// rpcinitappendserver_1_svc
// This function is used to initialize the server append function
// It gets teh params, creates a udp link to rpcinitverifyserver_1_svc and
// then returns a value to tell the client it is ready to append
int * rpcinitappendserver_1_svc(args, req)
	char** args;
	struct svc_req *req;
{
	static int status;
	printf("append server initialized\n");

	//split up the args to get the values
	//"property_index,thread_count,seg_length,num_seg,c0,c1,c2,host_name2"

	printf("%s\n",*args );

	// NOT WORKING! :(
	sscanf(*args, "%d,%d,%d,%d,%c,%c,%c", &property_index,&thread_count,&seg_length,&num_seg,&c0,&c1,&c2);
	printf("%d,%d,%d,%d,%c,%c,%c\n", property_index, thread_count,seg_length,num_seg,c0,c1,c2);


	str = calloc( num_seg*seg_length + 1, sizeof(char));

	status = 1;
	return (&status);
}




int s, i, slen;
struct sockaddr_in si_me, si_other, si_other2;
char buf[BUFLEN];
char message[BUFLEN];



void die(char *s)
{
    perror(s);
    exit(1);
}

// set up the server
int init_server_and_send_string(char * str){
	struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
     die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(hn , &si_other.sin_addr) == 0)
    {
     fprintf(stderr, "inet_aton() failed\n");
     exit(1);
    }

				//send the message
				if (sendto(s, str, strlen(str) , 0 , (struct sockaddr *) &si_other, slen)==-1){
    	die("sendto()");
				}

    //receive a reply and print it
    //clear the buffer by filling null, it might have previously received data
    memset(buf,'\0', BUFLEN);

    close(s);
    return 0;
}



static int current_str_pos = 0;
static int curr_seg = -1;
int nc0 = 0;
int nc1 = 0;
int nc2 = 0;
int * rpcappend_1_svc(args, req)
	char** args;
	struct svc_req *req;
{
	static int status;
	status = 1;


 	// all string building
 	char c = malloc(sizeof(char));
 	sscanf(*args, "%c", &c);


 	if (current_str_pos >= num_seg*seg_length) {
 		status = -1;
			submit = 0;
 	}
 	else {
 		str[current_str_pos] = c;
 		current_str_pos++;
 		status = 0;
 	}
 	// if (current_str_pos % seg_length == 0) {
  //   	curr_seg++;
  //   	nc0 = 0;
  //       nc1 = 0;
  //       nc2 = 0;
  //   }

  //   //check enforcement
  //   int can_add = enforce(current_str_pos % seg_length, curr_seg,c);

  //   if (can_add == 1) {
  //   	str[current_str_pos] = c;
  //   	if (c == c0) {
  //   		++nc0;
  //   	}
  //   	if (c == c1) {
  //   		++nc1;
  //   	}
  //   	if (c == c2) {
  //   		++nc2;
  //   	}
  //   	++current_str_pos;
  //   	status = 0;
  //   }

    printf("char %c  on seg %d added to string %s %d\n",c, curr_seg, str, current_str_pos);
    if (current_str_pos >= seg_length*num_seg) {
    	status = -1;
					submit = 0;

    }

	// if string is done then setup server and send message
	if (submit == 0){
		submit = 1;
		init_server_and_send_string(str);
	}
	return (&status);

}


//given the current character, check if we can add it to the current segment
int enforce(int index, int seg, char c) {

	int emptySpots = seg_length - index;

	//enforce property F0
	if (property_index == 0) {
	    return propertyZero(emptySpots,  nc0,  nc1,  nc2, c,  c0,  c1,  c2);
	}

	 //enforce property F1
	if (property_index == 1) {
	    return propertyOne(emptySpots, thread_count, seg_length,  nc0,  nc1,  nc2, c,  c0,  c1,  c2);
	}

    //enforce property F2
	if (property_index == 2) {
	    return propertyTwo(emptySpots, thread_count, seg_length, nc0,  nc1,  nc2, c,  c0,  c1,  c2);
	}

	//enforce property F3 by using F0
	if (property_index == 3) {
	    return propertyZero(emptySpots,  nc2,  nc1,  nc0, c,  c2,  c1,  c0);
	}


	return -1;
}
