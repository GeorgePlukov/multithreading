#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include "pa5.h"
#include "ppm.h"

__device__ int monus(int x, int y);
__device__ int maxus(int x, int y,int max);

dim3 block_dim, grid_dim;

int blur_radius;
int img_w;
int img_h;

char* input_ppm;
char* output_ppm;

struct Image* img_in;
struct Image* img_out;



__global__ void hello(int w, int h,int r, struct Pixel *in, struct Pixel *out) {
	// int myId = 1;
	int blockId =  blockIdx.y * gridDim.x + blockIdx.x;
	int threadId = blockId * blockDim.x + threadIdx.x;
	int index = 0;
	if (blockId == 1079) {
		printf("hello from blockid: %d threadId: %d \n",  blockId, threadId);
		printf("Pixel (x,y,r,g,b) :  (%d %d %d %d %d) \n",  in[blockId].x,in[blockId].y,in[blockId].r,in[blockId].g,in[blockId].b );


		int minX = monus(in[blockId].x,r);
		int maxX = maxus(in[blockId].x,r,w);
		int minY = monus(in[blockId].y,r);
		int maxY = maxus(in[blockId].y,r,h);
		int red = 0;
		int green = 0;
		int blue = 0;
		int num_pixels = 0;
		int i,j;
		unsigned char r = 0, g = 0, b = 0;

		for ( i = minX; i <= maxX; i++){
			for ( j = minY; j <= maxY; j++){

				r = in[i*w+j + threadId%3].r;
				g = in[i*w+j + threadId%3 +1].g;
				b = in[i*w+j + threadId%3 +2].b;
				red += (int)r;
				green +=(int) g;
				blue += (int)b;
				num_pixels++;
			}
		}

		red   = floor( (float) red / num_pixels );
		green = floor((float) green / num_pixels);
		blue  = floor((float) blue / num_pixels);

		struct Pixel jp;


		jp.x = in[blockId].x;
		jp.y = in[blockId].y;
		jp.r = red;
		jp.b = blue;
		jp.g = green;

		out[blockId] = jp;
	}
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

	block_dim = dim3(3);
	grid_dim = dim3(img_h, img_w);

	return 0;
}


int run() {


	int num_pixels   = img_w*img_h;
	struct Pixel *pixels_host_in  = 	(Pixel *) malloc(sizeof(Pixel *)*num_pixels);
	struct Pixel *pixels_host_out = 	(Pixel *) malloc(sizeof(Pixel *)*num_pixels);

	struct Pixel *pixel_device_in ;
	struct Pixel *pixel_device_out ;


	cudaMalloc((void **) &pixel_device_in,  (sizeof(Pixel *)*num_pixels));
	cudaMalloc((void **) &pixel_device_out,  (sizeof(Pixel *)*num_pixels));


	// populate pixel_device_in
	int x,y;
	int index = 0;
	for (y = 0; y < img_h; y++) {
		for (x = 0; x < img_w; x++) {
			index = y*img_w+x;
			// printf("index = %d\n", index );
			pixels_host_in[index].x = x;
			pixels_host_in[index].y = y;
			pixels_host_in[index].r = ImageGetPixel(img_in, x, y, 0);
			pixels_host_in[index].g = ImageGetPixel(img_in, x, y, 1);
			pixels_host_in[index].b = ImageGetPixel(img_in, x, y, 2);

		}
	}


	cudaMemcpy(pixel_device_in, pixels_host_in, sizeof(Pixel *)*num_pixels, cudaMemcpyHostToDevice);

	hello<<<grid_dim, block_dim>>>(img_w, img_h, blur_radius, pixel_device_in, pixel_device_out);

	cudaDeviceSynchronize();

	cudaMemcpy(pixels_host_out, pixel_device_out, sizeof(Pixel *)*num_pixels, cudaMemcpyDeviceToHost);



	cudaFree((void*) pixel_device_in);
	cudaFree((void*) pixel_device_out);

	free(pixels_host_in);
	free(pixels_host_out);

	printf("num pixels = %d\n", num_pixels );
	printf("Image size is (width: %d, height: %d) \n", img_w, img_h);


	return 1;
}


__device__ int monus (int x, int y) {
	if (x - y < 0){
		return 0;
	}
	return x;
}


__device__ int maxus (int x, int y, int max) {
	if (x + y > max){
		return max;
	}
	return x + y;
}


// __device__ struct Pixel averagePixels(int x, int y, struct Pixel *in) {
// 	int minX = monus(x,blur_radius);
// 	int maxX = maxus(x,blur_radius,img_in->width);
// 	int minY = monus(y,blur_radius);
// 	int maxY = maxus(y,blur_radius,img_in->height);
// 	int red = 0;
// 	int green = 0;
// 	int blue = 0;
// 	int num_pixels = 0;
// 	int i,j;
// 	unsigned char r = 0, g = 0, b = 0;
//
// 	for ( i = minX; i <= maxX; i++){
// 		for ( j = minY; j <= maxY; j++){
//
//
// 			r = ImageGetPixel(img_in, i, j, 0);
// 			g = ImageGetPixel(img_in, i, j, 1);
// 			b = ImageGetPixel(img_in, i, j, 2);
// 			red += (int)r;
// 			green +=(int) g;
// 			blue += (int)b;
// 			num_pixels++;
// 		}
// 	}
//
// 	red   = floor(red / num_pixels);
// 	green = floor(green / num_pixels);
// 	blue  = floor(blue / num_pixels);
//
// 	struct Pixel jp;
//
// 	jp.x = x;
// 	jp.y = y;
// 	jp.r = red;
// 	jp.b = blue;
// 	jp.g = green;
//
// 	return jp;
// }


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
