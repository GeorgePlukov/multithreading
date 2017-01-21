C = g++
pa1: pa1.o
	$(CC)  -o pa1.x pa1.o -pthread
pa1.o: pa1.c
	$(CC)  -c pa1.c -pthread

clean:
	- rm *.o
	- rm example
