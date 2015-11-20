## DPDK-based DPI VNF

This file contains the instructions to build a DPI that exchanges packets with the virtual switch
through the DPDK rte_rings.

The virtual network function analyzes packets coming from the first interfaces and, if a packet is
allowed, forwards it on the second interface. Packets flowing in the opposite direction are instead
forwarded without any analysis.

By default, the DPI drops the HTTP containing the words "porn" and "sex".

### Configuring the DPI

The configuration of the DPI has to be embedded in the source code; right now, we cannot change dynamically (at run-time) the configuration.

To change the configuration of the DPI (e.g., changing the network interfaces it binds to, the rules used to allow/block the traffic, etc.), edit the file `runtime.c`.

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
