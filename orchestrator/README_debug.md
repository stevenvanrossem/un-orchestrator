# Debug the NF-FG

This document is intended to provide some suggestions to debug the UN in case 
the deployed NF-FG does not work.

## Debug OvS

The following commands are useful to check the status of OvS.

    ; Show the bridges created with the respective ports
    $ sudo ovs-vsctl show

    ; Show the OpenFlow identifiers for the ports of a bridge
    $ sudo ovs-ofctl show bridge_name --protocol=Openflow12

    ; Show the forwarding table of a bridge
    $ sudo ovs-ofctl dump-flows bridge_name --protocol=Openflow12

    ; Delete a bridge
    $ sudo ovs-vsctl del-br bridge_name

## Debug OpenFlow

The following commands show how to setup wireshark to debug openflow messages

	; install wireshark
	$ sudo apt-get install wireshark

	; move to wireshark plugin directory
	$ cd /usr/lib/x86_64-linux-gnu/wireshark/libwireshark3/plugins

	; download openflow dissector plugin
	$ wget http://www.projectfloodlight.org/openflow.lua

Openflow messages are now recognized from packet sniffer, just run Wireshark and listen to loopback interface.

## Debug Docker

The following commands are useful to check the status of the Docker environment

    ; Show the running containers
    $ sudo docker ps

    ; Show the containers that are no longer running but that have not been removed
    $ sudo docker ps -a

    ; Execute a command in a runnin container
    $ sudo docker container_id command
    ; The container ID can be retrieved through the first command above

    ; Kill a container (it will not be shown anymore with `docker ps`)
    $ sudo docker kill container_id

    ; Remove a container
    $ sudo docker rm container_id

## Debug KVM

**TODO**
