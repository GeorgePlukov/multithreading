LDFLAGS= -lm
CFLAGS=-g -Wall
CC=gcc
MPICC=mpicc

pa4= pa4

all: $(pa4)


$(pa4): pa4.o ppm.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	./pa4 1 fox.ppm fox.ppm

clean:
	$(RM) *.o $(pa4)
