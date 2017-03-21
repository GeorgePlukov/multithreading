#include <stdio.h>
#include <stdlib.h>
#include "verify.h"
#include <sys/socket.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#define BUFLEN 512 //Max length of buffer
#define PORT 8884 //The port on which to listen for incoming data

void * listen_socket (void*);
char * str;
int currentSegment= 0;

int * rpcinitverifyserver_1_svc(req)
struct svc_req * req;
{
  	static int status;
  	printf("[STATUS] Init server called\n");

	//setup socket
	pthread_t thread;
	pthread_create(&thread, NULL,listen_socket, NULL);
  	//keep listening for data


	status = 1;
	return (&status);
}

void * listen_socket (void* r){
	struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
		printf("[ERROR] Socket not properly created\n");
		exit(1);
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1){
		printf("[ERROR] Socket not properly bound\n");
		exit(1);
	}

    //keep listening for data

        printf("Waiting for data...");
        fflush(stdout);

								// Listen for data coming into the port
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
			printf("[ERROR] Error receiving data from socket\n");
			exit(1);
        }

		// allocate new memory for array
		str = calloc(recv_len+1, sizeof(char));

		int d;
		for (d = 0; d < recv_len; d++) {
			str[d] = buf[d];
		}
		str[d] = '\0';
		printf("str = %s.\n", str);

    close(s);
				return;
}


int current_index = 0;

my_struct * rpcgetseg_1_svc(args, req)
int  args;
struct svc_req * req;
{
	int seg_len = args;


	static my_struct response;                                                 
	static char text[255]; 
	memset(&response, '\0', sizeof(my_struct));                                
	memset(text, '\0', sizeof(text));  

	int max_seg = strlen(str)/seg_len;
	if (current_index < max_seg) {

		char* seg = malloc(sizeof(char)*(seg_len+1));

		//where in the string 'str' should we starty copying
		int starting_position = current_index*seg_len;
		
		int i;
		for (i = 0; i < seg_len; i++) {
			seg[i] = str[starting_position++];
		}
		seg[i] = '\0';

		//setup the response text <segIndex,segStr>
		sprintf(text,"%d,%s", current_index, seg);
		current_index+=1;
		free(seg);
	}

	else {
		//if no more segments to send, reply with <segIndex,'-'>
		sprintf(text,"%d,%s%c",current_index,"-",'\0');
	}

	// printf("Sending over: %s.\n",text);
	response.data = text; 

	return (&response);
}
