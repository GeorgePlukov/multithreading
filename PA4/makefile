LDFLAGS =-lm
CFLAGS=-g -Wall
CC=mpicc


MAIN=pa4.x

$(MAIN): pa4.o ppm.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)  


clean:
	$(RM) *.o *.x $(MAIN)
