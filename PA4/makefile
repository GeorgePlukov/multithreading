LDFLAGS= -lm
CFLAGS=-g -Wall
CC=gcc
MPICC=mpicc

pa4= pa4

all: $(pa4)


$(pa4): pa4.o ppm.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	./pa4 25 fox.ppm output.ppm

clean:
	$(RM) *.o $(pa4)
