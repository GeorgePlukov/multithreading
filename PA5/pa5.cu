#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include "pa5.h"




__global__ void hello() {

	int myId = 1;
	printf("hello from %d \n",  myId);
}


int main(int argc, char** argv) {

	//cuda hello world
	hello<<<1,10>>>();
	cudaDeviceSynchronize();

	return 0;
}












