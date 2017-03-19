#include <stdio.h>
#include <stdlib.h>
#include "append.h"


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



	status = 1;
	return (&status);
}


int * rpcappend_1_svc(args, req)
	char** args;
	struct svc_req *req;
{
	static int status;

	status = -1;
	return (&status);
}
