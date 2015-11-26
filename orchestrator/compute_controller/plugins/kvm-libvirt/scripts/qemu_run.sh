#!/bin/bash

#Author: Ivano Cerrato
#Date: Oct 30th 2015

#$1 network function name
#$2	TCP port to be used for the monitor
#$3 disk path + 'part of the command line related to ivshmem'

if (( $EUID != 0 ))
then
    echo "[$0] This script must be executed with ROOT privileges"
    exit 0
fi

#FIXE: smp and memory should be provided externally

echo "[$0] Executing command 'sudo qemu-system-x86_64 -name $1 -cpu host -smp 1 -machine accel=kvm,usb=off -m 1024 -drive file=$3 -snapshot -daemonize -monitor tcp:127.0.0.1:$2,server,nowait'"

sudo `echo qemu-system-x86_64 -name $1 -cpu host -smp 1 -machine accel=kvm,usb=off -m 1024 -drive file=$3 -snapshot -daemonize -monitor tcp:127.0.0.1:$2,server,nowait`
ret=`echo $?`

if [ $ret -eq 0 ]
then
	exit 1
else
	echo "[$0] Something went wrong while starting the virtual machine"
	exit 0
fi
