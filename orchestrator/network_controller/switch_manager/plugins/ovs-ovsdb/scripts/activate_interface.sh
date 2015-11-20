#!/bin/bash

#Author: Ivano Cerrato
#Date: Oct 28th 2015
#Brief: activate a port

#command line: sudo ./activate_interface.sh $1 $2 $3 $4 [$5 ...]

# $1: interace name

while :
do
	sudo ifconfig $1 up &> /dev/null
	ret=`echo $?`
	
	if [ $ret == 0 ]
	then
		break
	fi

	sleep 0.5
done

exit 0
