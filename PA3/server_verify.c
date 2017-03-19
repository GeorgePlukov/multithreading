#include <stdio.h>
#include <stdlib.h>
#include "verify.h"

int * rpcinitverifyserver_1_svc(args, req)
	char** args;
	struct svc_req *req;

{
	static int status;
	printf("verify server called initalized\n");

	//setup socket

	status = 1;
	return (&status);
}


int * rpcgetseg_1_svc(args, req)
	char** args;
	struct svc_req *req;

{
	static int status;

	status = 0;
	return (&status);
}