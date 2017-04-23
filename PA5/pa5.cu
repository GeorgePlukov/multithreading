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


/*
	GPU function for blurring an image of width w and height r
	Blur radius given by r
	in: Pixel values to be used for calculations
	out: Blurred pixel values
*/
__global__ void blur_img(int w, int h,int r, struct Pixel *in, struct Pixel *out) {
	int blockId =  blockIdx.y * gridDim.x + blockIdx.x;
	int threadId = blockId * blockDim.x + threadIdx.x;

	if (blockId < h*w) {
		int minX = monus(in[blockId].x,r);
		int maxX = maxus(in[blockId].x,r,w);
		int minY = monus(in[blockId].y,r);
		int maxY = maxus(in[blockId].y,r,h);
		int red = 0;
		int green = 0;
		int blue = 0;
		int num_pixels = 0;
		int x,y;
		int index = 0;
		unsigned char r = 0, g = 0, b = 0;

		for ( y = minY; y <= maxY; y++ ){
			for (x = minX; x <= maxX; x++){
				index = y*w+x;
				r = in[index + threadId%3].r;
				g = in[index + threadId%3 +1].g;
				b = in[index + threadId%3 +2].b;
				red += (int)r;
				green +=(int) g;
				blue += (int)b;
				num_pixels++;
			}
		}

		red   = floor( (float) red / num_pixels );
		green = floor( (float) green / num_pixels);
		blue  = floor( (float) blue / num_pixels);

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

	run();


	return 0;
}


int init() {

	//Create the two necessary Image objects
	img_in  = ImageRead(input_ppm);
	img_w 	= ImageWidth(img_in);
	img_h 	= ImageHeight(img_in);
	img_out = ImageCreate(img_w, img_h);

	//Setup the grid and block sizes based on image width and height
	block_dim = dim3(3);
	grid_dim = dim3(img_h, img_w);

	return 0;
}




/*
	Setup two copies of pixels array to work on the host and device.
	Call GPU function to blur all the images
	Write the result
*/
int run() {


	int num_pixels = img_w*img_h;

	struct Pixel *pixels_host_in  = (Pixel *) malloc(sizeof(Pixel)*num_pixels);
	struct Pixel *pixels_host_out = (Pixel *) malloc(sizeof(Pixel)*num_pixels);

	struct Pixel *pixel_device_in ;
	struct Pixel *pixel_device_out ;


	cudaMalloc((void **) &pixel_device_in,  (sizeof(Pixel)*num_pixels));
	cudaMalloc((void **) &pixel_device_out, (sizeof(Pixel)*num_pixels));


	// populate pixel_device_in
	int x,y;
	int index = 0;
	for (y = 0; y < img_h; y++) {
		for (x = 0; x < img_w; x++) {
			index = y*img_w + x;
			pixels_host_in[index].x = x;
			pixels_host_in[index].y = y;
			pixels_host_in[index].r = ImageGetPixel(img_in, x, y, 0);
			pixels_host_in[index].g = ImageGetPixel(img_in, x, y, 1);
			pixels_host_in[index].b = ImageGetPixel(img_in, x, y, 2);

		}
	}


	cudaMemcpy(pixel_device_in, (Pixel*)pixels_host_in, sizeof(Pixel)*num_pixels, cudaMemcpyHostToDevice);

	blur_img<<<grid_dim, block_dim>>>(img_w, 
									  img_h, 
									  blur_radius, 
									  pixel_device_in, 
									  pixel_device_out);

	cudaDeviceSynchronize();
	cudaGetLastError();

	cudaMemcpy((Pixel*)pixels_host_out, pixel_device_out, sizeof(Pixel)*num_pixels, cudaMemcpyDeviceToHost);



	//write result back to ppm img
	for (y = 0; y < img_h; y++) {
		for (x = 0; x < img_w; x++) {
			index = y*img_w+x;
			update_image(pixels_host_out[index]);
		}	
	}
	
	

	ImageWrite(img_out, output_ppm);

	cudaFree((void*) pixel_device_in);
	cudaFree((void*) pixel_device_out);

	free(pixels_host_in);
	free(pixels_host_out);
	free(img_in);
	free(img_out);

	printf("Image size is (width: %d, height: %d) \n", img_w, img_h);
	printf("Number of pixels blurred: %d\n", num_pixels );
	printf("Blurred image written to %s\n", output_ppm);

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




//given a pixel, update it on the img_out
void update_image(struct Pixel pixel) {
	ImageSetPixel(img_out, pixel.x, pixel.y, 0, pixel.r);
	ImageSetPixel(img_out, pixel.x, pixel.y, 1, pixel.g);
	ImageSetPixel(img_out, pixel.x, pixel.y, 2, pixel.b);
}




