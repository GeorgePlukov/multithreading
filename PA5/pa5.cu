#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include "pa5.h"
#include "ppm.h"




int blur_radius;
int img_w;
int img_h;

char* input_ppm;
char* output_ppm;

struct Image* img_in;
struct Image* img_out;



__global__ void hello() {
	int myId = 1;
	printf("hello from %d \n",  myId);
}


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

	printf("Image size is (width: %d, height: %d) \n", img_w, img_h);

	run();



	return 0;
}


int init() {

	//Create the two necessary Image objects
	img_in  = ImageRead(input_ppm);
	img_w = ImageWidth(img_in);
	img_h = ImageHeight(img_in);
	img_out = ImageCreate(img_w, img_h);

	return 0;
}


int run() {

	hello<<<1,1>>>();
	cudaDeviceSynchronize();

	return 1;
}


int monus (int x, int y) {
	if (x - y < 0){
		return 0;
	}
	return x;
}


int maxus (int x, int y, int max) {
	if (x + y > max){
		return max;
	}
	return x + y;
}


struct Pixel averagePixels(int x, int y) {
	int minX = monus(x,blur_radius);
	int maxX = maxus(x,blur_radius,img_in->width);
	int minY = monus(y,blur_radius);
	int maxY = maxus(y,blur_radius,img_in->height);
	int red = 0;
	int green = 0;
	int blue = 0;
	int num_pixels = 0;
	int i,j;
	unsigned char r = 0, g = 0, b = 0;

	for ( i = minX; i <= maxX; i++){
		for ( j = minY; j <= maxY; j++){
			r = ImageGetPixel(img_in, i, j, 0);
			g = ImageGetPixel(img_in, i, j, 1);
			b = ImageGetPixel(img_in, i, j, 2);
			red += (int)r;
			green +=(int) g;
			blue += (int)b;
			num_pixels++;
		}
	}

	red   = floor(red / num_pixels);
	green = floor(green / num_pixels);
	blue  = floor(blue / num_pixels);

	struct Pixel jp;

	jp.x = x;
	jp.y = y;
	jp.r = red;
	jp.b = blue;
	jp.g = green;

	return jp;
}


//given a pixel, update it on the img_out
void update_image(struct Pixel pixel) {

	ImageSetPixel(img_out, pixel.x, pixel.y, 0, pixel.r);
	ImageSetPixel(img_out, pixel.x, pixel.y, 1, pixel.g);
	ImageSetPixel(img_out, pixel.x, pixel.y, 2, pixel.b);

}

void writeoutput() {
	//now that we built img_out, save it to the desired file
	ImageWrite(img_out, output_ppm);
}





void cleanup() {
	free(img_in);
	free(img_out);
}








