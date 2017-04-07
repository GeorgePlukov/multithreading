#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "ppm.h"
#include "pa4.h"


int blur_radius = 0;
char* input_ppm;
char* output_ppm;

struct Image* img_in;
struct Image* img_out;
void colorize_row(int row, unsigned char col, int width, int height, Image *img);


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
	
	//Create the two necessary Image objects
	int img_w, img_h;
	img_in  = ImageRead(input_ppm);

	img_w = ImageWidth(img_in);
	img_h = ImageHeight(img_in);
	printf("Dimensions %d x %d\n", img_w, img_h);
	//output image
	img_out = ImageCreate(img_w, img_h);


	//get a number of rows for each processor



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

    size_t i = 0;
    unsigned char my_colour = world_rank % 3;
    for (i = my_starting_row; i < row_blocks*(world_rank+1); i++) {
    	colorize_row(i,my_colour, img_w, img_h, img_out);
    	printf("processor %d colorizing row %d\n", world_rank, i);
    }

    if (world_rank != 0) {
    	MPI_Send(&my_starting_row, 1, MPI_INT,0,0,MPI_COMM_WORLD);
    }
    else {
    	int i;
    	for (i = 1; i < world_size; i++) {
    		MPI_Recv(&my_starting_row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	}
    }


    if (world_rank == 0) {
    	printf("DONE!\n");
		ImageWrite(img_out, output_ppm);
    }









    // Finalize the MPI environment.
    MPI_Finalize();


	write_output();
	cleanup();

	return 1;
}

void colorize_row(int row, unsigned char col,int width, int height, Image *img) {

	int x = 0;
	for (x = 0; x < width; x++) {
		ImageSetPixel(img, x,row, col, (ImageGetPixel(img, x, row, col)+10)%25);
	}
}










void write_output() {

	//now that we built img_out, save it to the desired file
	// ImageWrite(img_out, output_ppm);
}




void cleanup() {
	free(img_in);
	free(img_out);
}