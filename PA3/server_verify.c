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


char ** rpcgetseg_1_svc(args, req)
	int* args;
	struct svc_req *req;

{
	printf("getSeg got %d\n", *args);

	static char* le_segment = "bob";
	return (&le_segment);
}