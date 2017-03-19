#include <stdio.h>
#include <stdlib.h>
#include "append.h"
#include <sys/socket.h>
#include <ctype.h>


#define BUFLEN 512  //Max length of buffer
#define PORT 8888  //The port on which to listen for incoming data

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
	char* host_name2 = "130.113.68.130";
	printf("%s\n",*args );

	// NOT WORKING! :(
	sscanf(*args, "%d,%d,%d,%d,%c,%c,%c", &F,&N,&L,&M,&c0,&c1,&c2);
	printf("%d,%d,%d,%d,%c,%c,%c\n", F, N,L,M,c0,c1,c2);

	printf("Set up udp server\n");
	init_udp_server(host_name2);

	status = 1;
	return (&status);
}




int s, i, slen;
struct sockaddr_in si_me, si_other, si_other2;
char buf[BUFLEN];
char message[BUFLEN];


// set up the server
int init_udp_server(char * hostname){
	s, slen = sizeof(si_other);

		//create a UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		// socket error on creation
		return 1;
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);

	if (inet_aton(hostname , &si_other.sin_addr) == 0){
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	// if (sendto(s, "test", strlen("test") , 0 , (struct sockaddr *) &si_other, slen)==-1){
	// 	printf("err\n" );
	// }
	// printf("test2\n" );
	//receive a reply and print it
	//clear the buffer by filling null, it might have previously received data
	memset(buf,'\0', BUFLEN);
	puts(buf);


}

int * rpcappend_1_svc(args, req)
	char** args;
	struct svc_req *req;
{
	static int status;
	// 
	// if (sendto(s, "test", strlen("test") , 0 , (struct sockaddr *) &si_other, slen)==-1){
	// 	printf("err\n" );
	// }
	status = -1;

	return (&status);
}
