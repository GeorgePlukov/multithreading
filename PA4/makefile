MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
MPI_LINK_FLAGS = $(shell mpicc --showme:link)
MPICC=mpicc


MAIN=pa4.x

$(MAIN): pa4.o ppm.o
	$(MPICC) $(MPI_COMPILE_FLAGS)  $^  $(MPI_LINK_FLAGS) -o $@  


clean:
	$(RM) *.o *.x $(MAIN)
