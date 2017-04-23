#!/bin/bash

declare -a arr=( "snail.ppm" "fox.ppm" "wolf.ppm" "bird.ppm" )
declare -a radius=( 0 10 20 40 )


for img in "${arr[@]}"
do
	for r in "${radius[@]}"
	do
		time  ./pa5.x ${r} ${img} out.ppm			
	done
done
