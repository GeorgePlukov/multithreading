#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"
// #include "pa4.h"


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
	// write_output();
	// cleanup();

	return 1;
}





int init() {

	//Create the two necessary Image objects
	img_in  = ImageRead(input_ppm);
	img_out = ImageCreate(ImageWidth(img_in), ImageHeight(img_in));

	return 0;
}

int monus (int x, int y){
	if (x - y < 0){
		return 0;
	}
	return x;
}

int maxus (int x, int y, int max){
	if (x + y > max){
		return max;
	}
	return x + y;
}

typedef struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Pixel averagePixels(int minX,int maxX,int minY,int maxY){
	struct Pixel p = {'100','150','100'};
	return p;
}

int run() {
	printf("%i\n", img_in->width);
	printf("%i\n", img_in->height);

	for (int i = 0; i < img_in->width; i++){

		for (int j = 0; j < img_in->height; j++){

			// unsigned char r = ImageGetPixel(img_in, i, j, 0);
			// unsigned char g = ImageGetPixel(img_in, i, j, 1);
			// unsigned char b = ImageGetPixel(img_in, i, j, 2);
			// minx, maxX, miny, maxY
			int minX = monus(i,blur_radius);
			int maxX = maxus(i,blur_radius,img_in->width);
			int minY = monus(j,blur_radius);
			int maxY = maxus(j,blur_radius,img_in->height);


			printf("minx: %i maxx: %i miny: %i maxy: %i\n", minX, maxX,minY,maxY);

			struct Pixel p = averagePixels(minX,maxX,minY,maxY);

			ImageSetPixel(img_out, i, j, 0, p.red);
			ImageSetPixel(img_out, i, j, 1, p.green);
			ImageSetPixel(img_out, i, j, 2, p.blue);

			// printf("rgb(%u,%u,%u)\n", r,g,b);
		}
	}
	return 1;
}


// void write_output() {
//
// 	//now that we built img_out, save it to the desired file
// 	ImageWrite(img_out, output_ppm);
// }
//
//
//
//
// void cleanup() {
// 	free(img_in);
// 	free(img_out);
// }
