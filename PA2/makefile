run: pa1.x
	./pa1.x 1 2 3 4 a b c
pa1.x: pa1.cpp
	gcc -g -Wall -fopenmp -o pa1.x pa1.cpp
	
clean:
	- rm *.o
	- rm pa1.x
	- rm out.txt
