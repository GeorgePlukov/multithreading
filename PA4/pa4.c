#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "ppm.h"
#include "pa4.h"


int blur_radius;
int img_w;
int img_h;

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
	img_w = ImageWidth(img_in);
	img_h = ImageHeight(img_in);

	img_out = ImageCreate(img_w, img_h);



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



int run() {


	/** Setup MPI **/
	MPI_Init(NULL, NULL);

	// Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",processor_name, world_rank, world_size);


    //try to allocate num of rows with respect to world_size
    int row_blocks = ceil(img_h/world_size);

    int my_starting_row = world_rank*row_blocks;	


    //do the calculations here
    int i,j;

	for ( i = my_starting_row; i < (world_rank+1)*row_blocks; i++){
		for ( j = 0; j < img_h; j++){
			averagePixels(i,j);
		}
	}


    if (world_rank != 0) {
    	printf("all other send result to 0\n");
    }	
    else {
    	printf("rank 0 receive result\n");
    }							


    if (world_rank == 0) {
    	printf("rank 0 write result to output ppm\n");
    }								


    MPI_Finalize();





	// printf("%i\n", img_in->width);
	// printf("%i\n", img_in->height);

	
	// return 1;
}




void averagePixels(int x, int y) {
	// struct Pixel p = {'100','150','100'};
	int minX = monus(x,blur_radius);
	int maxX = maxus(x,blur_radius,img_in->width);
	int minY = monus(y,blur_radius);
	int maxY = maxus(y,blur_radius,img_in->height);
	int red = 0;
	int green = 0;
	int blue = 0;
	int num_pixels = 0;
	int i,j;
	for ( i = minX; i <= maxX; i++){
		for ( j = minY; j <= maxY; j++){
			unsigned char r = ImageGetPixel(img_in, x, j, 0);
			unsigned char g = ImageGetPixel(img_in, x, j, 1);
			unsigned char b = ImageGetPixel(img_in, x, j, 2);
			red += (int)r;
			green +=(int) g;
			blue += (int)b;
			num_pixels++;
		}

	}
	red = round(red / num_pixels);
	green = round(green / num_pixels);
	blue = round(blue / num_pixels);

	ImageSetPixel(img_out, x, y, 0, red);
	ImageSetPixel(img_out, x, y, 1, green);
	ImageSetPixel(img_out, x, y, 2, blue);
}

void write_output() {

	//now that we built img_out, save it to the desired file
	ImageWrite(img_out, output_ppm);
}




void cleanup() {
	free(img_in);
	free(img_out);
}