#!/bin/bash

val=$1
if test ${#val} != 1;then
	echo "length != 1"
	exit 0
fi 

case $val in
1)
	echo "value = 1"
	;;
2)
	echo "value = 2"
	;;
3)	echo "value = 3"
	;;
*)	
	echo "value= "$1",not in 1 2 3"
	;;
esac

