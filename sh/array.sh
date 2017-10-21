#!/bin/bash


array=(1 2 3 4)
array[4]=5
array[9]=10

echo ${#array[@]}
echo ${#array[*]}
echo ${array[1]}
