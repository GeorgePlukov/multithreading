#ifndef PA4_H
#define PA4_H



struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}, pixel;



void cleanup();
void writeoutput();
int run();
void averagePixels(int x, int y);


#endif