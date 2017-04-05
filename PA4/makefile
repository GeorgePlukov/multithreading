LDFLAGS= -lm
CFLAGS=-g -Wall
CC=gcc
MPICC=mpicc

pa4= pa4

all: $(pa4) 



$(pa4): pa4.o ppm.o mpi.o
	$(MPICC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(pa4)