#ifndef PA4_H
#define PA4_H





struct Pixel {
	int x,y;
	unsigned char r,g,b;
};

int run();
int init();
void cleanup();
void writeoutput();
void update_image(struct Pixel pixel);
struct Pixel averagePixels(int x, int y);

#endif