#include <stdio.h>
#include <stdlib.h>
#include "append.h"

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
	char* host_name2;
	printf("%s\n",*args );

	// NOT WORKING! :(
	//sscanf(args, "%d,%d,%d,%d,%c,%c,%c,%s", F,N,L,M,c0,c1,c2,host_name2);

	// printf("rpcInitAppend received the values\n \
	// 	%d,%d,%d,%d,%c,%c,%c,%s\n",F,N,L,M,c0,c1,c2,host_name2);


	//now we call 

	status = 1;
	return (&status);
}


int * rpcappend_1_svc(args, req)
	char** args;
	struct svc_req *req;

{
	static int status;

	status = 0;
	return (&status);
}