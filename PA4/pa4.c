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

int create_pixel_type(MPI_Datatype *mpi_pixel);



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


    //setup timer
    double local_start, local_finish, local_elapsed, elapased;
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();
    
    //try to allocate num of rows with respect to world_size
    int row_blocks 		= ceil((float)img_h/world_size);
    int my_starting_row = world_rank*row_blocks;	
    int my_ending_row   = (world_rank+1)*row_blocks;
    size_t my_num_pixels   = img_w*row_blocks;


   	//setup the datatypes we will need to send over 
    MPI_Datatype mpi_unit_pixel;
   	MPI_Datatype mpi_row_pixels;	

   	create_pixel_type(&mpi_unit_pixel);
    MPI_Type_contiguous(my_num_pixels, mpi_unit_pixel, &mpi_row_pixels);
    MPI_Type_commit(&mpi_row_pixels);

   	
    //initialize array for each process to use
    struct Pixel *my_row_values = malloc(sizeof(*my_row_values)*my_num_pixels);
    size_t i;


     for (i = 1	; i < my_num_pixels; i++) {
     	my_row_values[i].x = 0;
    	my_row_values[i].y = 0;		
    	my_row_values[i].r = 0;
    	my_row_values[i].g = 0;		
    	my_row_values[i].b = 0;
    }




    //do the calculations here
    int x = 0, y = 0;
    size_t index = 0;
	for ( y = my_starting_row; y < my_ending_row && y < img_h; y++){
		for ( x = 0; x < img_w; x++){
			my_row_values[index++] =  averagePixels(x,y);	
		}
	}


	// send results
    if (world_rank != 0) {
    	MPI_Ssend(my_row_values, 1, mpi_row_pixels, 0, 0, MPI_COMM_WORLD);
    }	

    else {

    	//update the image with process 0's result, then update each subsequent processes' data
    	int item, proc;
    	for (item = 0; item < my_num_pixels; item++) {
    		update_image(my_row_values[item]);
    	}
    	
    		
    	for (proc = 1; proc < world_size; proc++) {
    		MPI_Recv(my_row_values, 1, mpi_row_pixels, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		for (item = 0; item < my_num_pixels; item++) {
    			update_image(my_row_values[item]);
    		}
    	}
    }			


   




    if (world_rank == 0) {
    	writeoutput();
    	cleanup();
    }			


    local_finish = MPI_Wtime();
    local_elapsed  = local_finish - local_start;
    MPI_Reduce(&local_elapsed, &elapased, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
    	printf("Total execution time %f seconds \tnp = %d \t  file = %s\n", elapased, world_size, input_ppm);
    }

    
    MPI_Type_free(&mpi_unit_pixel);
    MPI_Type_free(&mpi_row_pixels);
    MPI_Finalize();



	return 0;
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

	red = round(red / num_pixels);
	green = round(green / num_pixels);
	blue = round(blue / num_pixels);

	struct Pixel jp;

	jp.x = x;
	jp.y = y;
	jp.r = red;
	jp.b = blue;
	jp.g = green;

	return jp;
}




/* source : http://stackoverflow.com/questions/20228772 */
int create_pixel_type(MPI_Datatype *mpi_pixel) {
	//setup our data types
	int count = 5;
	int blocks[5] = {1, 1, 1, 1, 1};
	MPI_Datatype types[5] = {    
	        MPI_INT,
	        MPI_INT,
	        MPI_UNSIGNED_CHAR,
	        MPI_UNSIGNED_CHAR,
	        MPI_UNSIGNED_CHAR
	};
	MPI_Aint dis[5] = {          
			offsetof(struct Pixel, x),
			offsetof(struct Pixel, y),
	        offsetof(struct Pixel, r),
	        offsetof(struct Pixel, g),
	        offsetof(struct Pixel, b)
	};

	MPI_Type_create_struct(count, blocks, dis, types, mpi_pixel);
	MPI_Type_commit(mpi_pixel);

	return 0;
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