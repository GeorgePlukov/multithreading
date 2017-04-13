LDFLAGS =-lm
CFLAGS=-g
NVCC=nvcc


MAIN=pa5.x

$(MAIN): pa5.cu
	$(NVCC) -o $@ $^ $(CFLAGS) $(LDFLAGS)  


clean:
	$(RM) *.o *.x $(MAIN)
