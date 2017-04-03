#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"
#include "pa4.h"


int blur_radius = 0;
char* input_ppm;
char* output_ppm;

struct Image* img_in;
struct Image* img_out;

int main(int argc, char** argv) {


	if (argc < 3) {
		printf("Not enough arguments\n");
		return 0;
	}


	blur_radius = atoi(argv[1]);
	input_ppm  = argv[2];
	output_ppm = argv[3];


	if (blur_radius < 1) {
		printf("blur radius too small\n");
		return 0;
	}

	init();
	run();
	write_output();
	cleanup();

	return 1;
}





int init() {

	//Create the two necessary Image objects
	img_in  = ImageRead(input_ppm);
	img_out = ImageCreate(ImageWidth(img_in), ImageHeight(img_in));

	return 0;
}


int run() {
	return 1;
}



void write_output() {

	//now that we built img_out, save it to the desired file
	ImageWrite(img_out, output_ppm);
}




void cleanup() {
	free(img_in);
	free(img_out);
}