#include <stdio.h>
#include <stdlib.h>
#include "verify.h"
#include <sys/socket.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFLEN 512 //Max length of buffer
#define PORT 8884 //The port on which to listen for incoming data

void * listen_socket (void*);
char * str;
int currentSegment= 0;

int * rpcinitverifyserver_1_svc(args, req)
char * * args;
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

								// print the data that came in
        printf("Data: %s\n" , buf);

								// allocate new memory for array
								size_t len = strlen(buf);
								printf("%d\n", len);

								str = calloc(len+1,  sizeof(char));

								int d = 0;
								printf("%s\n" );
								for (d = 0; d < len; d++) {
									str[d] = buf[d];
								}

								//print stored string val
								for (d = 0; d < len; d++) {
									printf("%c", str[d]);
								}
								printf("\n");


    close(s);
				return;
}

char ** rpcgetseg_1_svc(args, req)
int * args;
struct svc_req * req;
{
		// n is the number of characters in a segment
		int * n = args;
		int len = strlen(str);


		// printf("%d ... %d\n",*args, len-1);
		//
		// int length = 1;
		// while ( currentSegment /= 10 )
  //  length++;

		// float test = (float) len / (float) *n;
		// printf("1 %f\n", test);
		// int test2 = floor(test);
		// printf("2 %f\n", test);
		//
		// char* r_string = calloc(test + length, sizeof(char));
		//
		// // if there is still segments to give
		// if (currentSegment < len / n -1){
		// 	sprintf(str,"%ld", currentSegment);
		// 	for (d = 0; d < len; d++) {
		// 		printf("%c", str[d]);
		// 	}
		//
		// 	segment_counter += L;
		// }
		// // last segment
		// else{
		// 	int ind;
		// 	for (ind = 0; ind< n; ind++){
		// 		printf("%c", str[ind]);
		// 	}
		// 	printf("\n");
		// 	free (str);
		// }


		// returns the location of the first item in regards to where the string is "," the segment
  return &str;
}
