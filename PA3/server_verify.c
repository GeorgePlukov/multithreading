#include <stdio.h>
#include <stdlib.h>
#include "verify.h"
#include <sys/socket.h>
#include <ctype.h>
#include <pthread.h>

#define BUFLEN 512 //Max length of buffer
#define PORT 8887 //The port on which to listen for incoming data

void * listen_socket (void*);

struct sockaddr_in si_me, si_other;
int s, i, slen = sizeof(si_other), recv_len;
char buf[BUFLEN];

int * rpcinitverifyserver_1_svc(args, req)
char * * args;
struct svc_req * req;
{
  static int status;
  printf("[STATUS] Init server called\n");

  //setup socket


  //create a UDP socket
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    // die("socket");
				printf("[ERROR] Socket connection could not be created");

				perror("test");
  }


  // zero out the structure
  memset((char * ) & si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  //bind socket to port
  if (bind(s, (struct sockaddr * ) & si_me, sizeof(si_me)) == -1) {
			printf("[ERROR] Socket could not be bound to port: %d\n", PORT);
  }


		pthread_t thread;
		pthread_create(&thread, NULL,listen_socket, NULL);
  //keep listening for data


  status = 1;
  return (&status);
}

void * listen_socket (void* r){
	int recv_len;
	while (1) {
			printf("Waiting for data...");
			fflush(stdout);
			//try to receive some data, this is a blocking call
			if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr * ) & si_other, & slen)) == -1) {
				printf("[ERROR] Message not properly received\n");
				// return 1;
			}else{
				break;
			}

			//print details of the client/peer and the data received
			printf("Received from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
			printf("Data: %s\n", buf);

	}
	close(s);

}

char ** rpcgetseg_1_svc(args, req)
int * args;
struct svc_req * req;
{
		static char * str = "asd";

  return &str;
}
