#ifndef PA4_H
#define PA4_H



typedef struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;


struct J_Pixel {
	int x,y;
	unsigned char r,g,b;
};

void cleanup();
void writeoutput();
int run();
struct J_Pixel averagePixels(int x, int y);
void update_image(struct J_Pixel pixel);


#endif