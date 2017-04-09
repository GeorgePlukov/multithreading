#!/bin/bash

declare -a arr=("bird.ppm" "snail.ppm" "fox.ppm" "wolf.ppm")
declare -a arr2=(1 2 4 8 16)
radius=15


for img in "${arr[@]}"
do
	for p in "${arr2[@]}"
	do
		start=`date +%s%3N`
		mpiexec -np ${p} ./pa4.x ${radius} ${img} out.ppm
		end=`date +%s%3N`
		
		runtime=$((end-start))
		echo "${img} took ${runtime} with ${p} processes"
	done
done
