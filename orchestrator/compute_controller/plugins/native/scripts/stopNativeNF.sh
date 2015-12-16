#!/bin/bash

#Author: Sergio Nuccio
#Date: October 21st 2015
#Brief: stop the execution of a native NF 

#command line: sudo ./nfs_manager/scripts/native/stopNativeNF.sh $1 $2

#$1 LSI ID							(e.g., 2)
#$2 NF name							(e.g., firewall)

#not needed anymore
##$3 number_of_ports						(e.g., 2)
##The next $3 parameters are the names of the port of the NF	(e.g., vEth0 vEth1)

if (( $EUID != 0 )) 
then
    echo "[$0] This script must be executed with ROOT privileges"
    exit 0
fi

tmp_dir=`echo $1"_"$1"_"$2"_tmp_"$2`

#open file that specifies the command that runs the stop script (if it exists)
file="$tmp_dir/$1_$2_stop"

if [ -f $file ]
then
	sudo chmod +x $file
	
	echo [`date`]"[$0] Executing command: '"`cat $file`"'"
	
	#command:= <script_name> <LSI_ID> <NF_NAME> <#PORTS> <PORT_NAMES ...>
	bash $file
	
	# remove files after the execution
	#rm $file
	rm $file
fi

#remove temporary directory
#actually it must exists
if [ -e $tmp_dir ]
then
	rm -r $tmp_dir
fi

#delete virtual interfaces related to the NF => not needed anymore
#current=4
#
#for((c=0; c<$3; c++))
#do
#	ip link del ${!current}
#	current=`expr $current + 1`
#done

echo "[$0] Native NF stopped"

exit 1;
