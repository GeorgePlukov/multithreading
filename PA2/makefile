run: pa2.x
	./pa2.x 1 2 3 4 a b c
pa2.x: pa2.c
	gcc -g -Wall -fopenmp -o pa2.x pa2.c

clean:
	- rm *.o
	- rm pa2.x
	- rm out.txt
