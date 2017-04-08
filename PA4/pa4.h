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

int run();
void cleanup();
void writeoutput();
void update_image(struct J_Pixel pixel);
struct J_Pixel averagePixels(int x, int y);

#endif