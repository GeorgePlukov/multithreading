LDFLAGS= -lm
CFLAGS=-g -Wall
CC=gcc


pa4= pa4

all: $(pa4) 



$(pa4): pa4.o ppm.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(pa4)