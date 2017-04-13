#!/bin/bash

declare -a arr=("bird.ppm" )
declare -a arr2=(1 2 4 8 16 32)
radius=15


for img in "${arr[@]}"
do
	for p in "${arr2[@]}"
	do
		mpiexec -np ${p} ./pa4.x ${radius} ${img} out.ppm		
	done
done
