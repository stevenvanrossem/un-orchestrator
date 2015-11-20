#!/bin/bash

#Author: Ivano Cerrato
#Date: June 16th 2014
#Brief: check if the Docker deamon is running on this machine
#		Docker should be started with the LXC implementation

#command line: sudo ./checkDockerRun.sh

if (( $EUID != 0 ))
then
    echo "[$0] This script must be executed with ROOT privileges"
    exit 0
fi

num=`ps aux | grep "docker" | grep -v "grep" | wc -l`

if [ $num -ge 1 ]
then
	exit 1
fi

echo "[$0] Docker is not running"

exit 0

