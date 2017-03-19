#include <stdio.h>
#include <stdlib.h>
#include "append.h"
#include <sys/socket.h>
#include <ctype.h>


#define BUFLEN 512  //Max length of buffer
#define PORT 8885  //The port on which to listen for incoming data

int count = 0;
char *str = "this is a string";
char* hn = "130.113.68.130";
int submit = 0;
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
	//"F,N,L,M,c0,c1,c2,host_name2"
	int F, N, L, M;
	char c0, c1, c2;
	printf("%s\n",*args );

	// NOT WORKING! :(
	sscanf(*args, "%d,%d,%d,%d,%c,%c,%c", &F,&N,&L,&M,&c0,&c1,&c2);
	printf("%d,%d,%d,%d,%c,%c,%c\n", F, N,L,M,c0,c1,c2);

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

int * rpcappend_1_svc(args, req)
	char** args;
	struct svc_req *req;
{
	static int status;
	status = -1;
 	// all string building



	// if string is done then setup server and send message
	if (submit == 0){
		submit = 1;
		init_server_and_send_string(str);
	}
	return (&status);
}
