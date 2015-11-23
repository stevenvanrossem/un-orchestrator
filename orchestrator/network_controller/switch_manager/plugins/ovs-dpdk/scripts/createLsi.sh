#!/bin/bash

#Author: UNIFY Consortium
#Date: 2015-07-02
#Brief: Create a bridge

#$1 LSI id
#$2 controller ip
#$3 controller port

bridgeName=`echo br_$1`
controller_ip=$2
controller_port=$3
ofp_version=$4

if (( $EUID != 0 ))
then
    echo "[$0] This script must be executed with ROOT privileges"
    exit 0
fi


echo "[$0] Creating bridge $bridgeName"
ovs-vsctl --no-wait add-br $bridgeName
ovs-vsctl --no-wait set bridge $bridgeName datapath_type=netdev
ovs-vsctl --no-wait set bridge $bridgeName protocols=$ofp_version
ovs-vsctl --no-wait set-controller $bridgeName tcp:$controller_ip:$controller_port

#XXX The two following commands are used to remove in-band controller, which causes
#loops in particular configurations
ovs-vsctl --no-wait set controller $bridgeName connection-mode=out-of-band
ovs-vsctl --no-wait set $bridgeName other-config:disable-in-band=true

exit 1

