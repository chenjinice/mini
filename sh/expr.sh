#!/bin/bash


a=10
b=20

echo `expr $b / $a`
echo `expr $a / $b`
echo `expr $a \* $b`
echo `expr $a == $b`
echo `expr $a != $b`


if [ $a == $b ] 
then
	echo 'a == b'
else
	echo "a != b"
fi

if [ $a -lt $b ] 
then
	echo "ccccc"
fi

