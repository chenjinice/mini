#!/bin/bash


a=100
b=200

if test $a = $b;then
	echo "a=b"
elif test $a -gt $b ; then
	echo "a>b"
elif test $a -lt $b ; then
	echo "a<b"
fi



