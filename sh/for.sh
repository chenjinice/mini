#!/bin/bash


array=(
	a
	b
	c
	d
	e
	f
	g
)

for var in ${array[*]};do
	echo $var
done

for ((i=0;i<10;i++));do
	echo $i
done
