LDFLAGS =-lm
CFLAGS=
NVCC=nvcc
CC=cc

MAIN=pa5.x

all: $(MAIN)

$(MAIN): pa5.o ppm.o
	$(NVCC) -o $@ $^ $(CFLAGS) $(LDFLAGS)  


pa5.o: pa5.cu
	$(NVCC) -x cu -c $^
	
ppm.o: ppm.c
	$(NVCC) -x cu -c $^ 



clean:
	$(RM) *.o *.x
