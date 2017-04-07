#ifndef PA4_H
#define PA4_H



typedef struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};



void cleanup();
int run();
void averagePixels(int x, int y);


#endif