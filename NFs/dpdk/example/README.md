## DPDK-based example VNF

This file contains the instructions to build a simple network function that exchanges packets with the virtual switch
through the DPDK rte_rings.

When the VNF receives a packet on the interface 'i', it destination MAC address is modified and the packet is sent sent
on the interface 'i+1'.

### Creating the binary

In addition to the DPDK library, the DPI requires the pcre library to implements the packet inspections.
The pcre library can be installed with the command:

	$ sudo apt-get install libpcre3 libpcre3-dev

To compile the DPI to be used with xDPd, run the following commands:

	$ export RTE_SDK=absolute_path_dpdk
	; e.g., export RTE_SDK=~/Desktop/dpdk-dev
	$ RTE_TARGET=x86_64-native-linuxapp-gcc
	$ make
	
To compile the DPI to be used with OvS-DPDK, run the following commands:

	$ export RTE_SDK=absolute_path_dpdk
	; e.g., export RTE_SDK=~/Desktop/dpdk-dev
	$ RTE_TARGET=x86_64-ivshmem-linuxapp-gcc
	$ make

**Note:** the network function has been written to work with xDPd. If you want to use it with
OvS-DPDK, please change the name of the rte_rings before compiling, as detailed in `../README.md`.
