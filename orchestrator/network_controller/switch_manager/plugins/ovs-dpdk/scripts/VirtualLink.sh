#!/bin/bash

#Author: UNIFY Consortium
#Date: 2015-09-21
#Brief: Add a Virtual Link between two bridges (using patch ports)

s_br=`echo br_$1`
d_br=`echo br_$2`
s_port_name=`echo VLink_$1-to-$2_$5`
d_port_name=`echo VLink_$2-to-$1_$5`
s_port_id=$3
d_port_id=$4
enable_flooding=$6
ofpversion=$7

if (( $EUID != 0 ))
then
    echo "[$0] This script must be executed with ROOT privileges"
    exit 0
fi

ovs-vsctl add-port $s_br $s_port_name -- set Interface $s_port_name type=patch ofport_request=$s_port_id options:peer=$d_port_name
ovs-vsctl add-port $d_br $d_port_name -- set Interface $d_port_name type=patch ofport_request=$d_port_id options:peer=$s_port_name

if [ $enable_flooding -eq 0 ]
then
    ovs-ofctl mod-port $s_br $s_port_id noflood --protocol=$ofpversion
    ovs-ofctl mod-port $d_br $d_port_id noflood --protocol=$ofpversion
fi

exit 1
