#!/bin/bash

#Author: UNIFY Consortium
#Date: 2015-10-29
#Brief: Destroy a bridge

#$1 LSI id

bridgeName=`echo br_$1`

if (( $EUID != 0 ))
then
    echo "[$0] This script must be executed with ROOT privileges"
    exit 0
fi


echo "[$0] Destroyng bridge $bridgeName"
ovs-vsctl --no-wait del-br $bridgeName

exit 1

