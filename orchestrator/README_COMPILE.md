# How to compile the un-orchestrator

In order to execute the un-orchestrator, we need to setup different components, namely:

  * a set of libraries needed to compile the un-orchestrator code
  * a virtual switch (either xDPd or OpenvSwitch) as a base switch for
    our platform
  * one or more execution environments for virtual network functions, e.g., KVM for
    executing VM, Docker, or other.

## Required libraries

Several libraries are required to compile the un-orchestrator.
In the following we list the steps required on an **Ubuntu 14.04**.

	; Install required libraries
	; - build-essential: it includes GCC, basic libraries, etc
	; - cmake: to create cross-platform makefiles
	; - cmake-curses-gui: nice 'gui' to edit cmake files
	; - libboost-all-dev: nice c++ library with tons of useful functions
	; - libmicrohttpd-dev: embedded micro http server
	; - libxml2-dev: nice library to parse and create xml
	$ sudo apt-get install build-essential cmake cmake-curses-gui libboost-all-dev libmicrohttpd-dev -libxml2-dev

	; Install JSON Spirit (nice library to parse JSON files)
	$ git clone https://github.com/sirikata/json-spirit
	; alternatively, a copy of JSON Spirit is provided in `[un-orchestrator]/contrib/json-spirit.zip`
	$ cd json-spirit/

	; Now install the above library according to the description provided
	; in the cloned folder

	; Install ROFL-common  (library to parse OpenFlow messages)
	$ git clone https://github.com/bisdn/rofl-common  
	; alternatively, a copy of ROFL-common is provided in `[un-orchestrator]/contrib/rofl-common.zip`
	$ cd rofl-common/

	; Now install the above library according to the description provided
	; in the cloned folder

## Install the proper virtual switch

The current un-orchestrator supports different types of virtual switches.
You have to install the one that you want to use, choosing from the
possibilities listed in this section.


### xDPd

In order to install xDPd, you have to follow the steps below.

	$ git clone https://github.com/bisdn/xdpd  
	$ cd xdpd/  

	;Install all the libraries required by the README provided in this folder  
	$ bash autogen  
	$ cd build  
	$ ../configure --with-hw-support=gnu-linux-dpdk --with-plugins="node_orchestrator rest"   
	$ make
	$ sudo make install

**WARNING: Currently, xDPd is not compiling on Linux kernels newer than 3.16.0-30.**

### Open vSwitch (of-config) [DEPRECATED]

Open vSwitch can be installed with either the OVSDB or OF-CONFIG plugin.
Although both protocols allow to control the switch (e.g., create/delete
new bridging instances, create/delete ports, etc), we found out
that OF-CONFIG is rather limited in terms of capabilities. For instance,
it cannot set the type of port configured on the switch (e.g., virtio
or IVSHMEM), requiring the orchestrator to rely on a combination of
OF-CONFIG commands and bash scripts to perform its job.

For this reason we suggest to install Open vSwitch with its native OSVDB 
support (next section); although OVSDB is not standard, it seems that it
does its job better than OF-CONFIG.

In any case, the compilation instruction for setting up Open vSwitch with 
OF-CONFIG are the following (not guaranteed that those are 100% accurate,
as the OF-CONFIG support in Open vSwitch is rather primitive).

OvS with the OFCONFIG support can be installed as follows:

	$ sudo apt-get install autoconf automake gcc libtool libxml2 libxml2-dev m4 make openssl dbus
	
	; Download LIBSSH from 
	;       https://red.libssh.org/projects/libssh/files
	; Now install the above library following the INSTALL file provided in the root directory

	; Clone the libnetconf repository
	$ git clone https://github.com/cesnet/libnetconf
    $ cd libnetconf/
    $ git checkout -b 0.9.x origin/0.9.x

	; Install the libnetconf library by following the instructions in the
    ; INSTALL file contained in the root folder of this library.

    ; Download Open vSwitch from
    ;      from http://openvswitch.org/releases/openvswitch-2.4.0.tar.gz
    $ tar -xf openvswitch-2.4.0.tar.gz
    $ cd openvswitch-2.4.0
    $ ./configure --prefix=/ --datarootdir=/usr/share --with-linux=/lib/modules/$(uname -r)/build
    $ make
    $ sudo make install
	
	; Clone the of-config repository
	$ git clone https://github.com/openvswitch/of-config    

	; Follow the instructions as described in the file INSTALL.md provided in the root folder of that repository.

### Open vSwitch (OVSDB)

At first, download the Open vSwitch source code from:

    http://openvswitch.org/releases/openvswitch-2.4.0.tar.gz

Then execute the following commands:

    $ tar -xf openvswitch-2.4.0.tar.gz
    $ cd openvswitch-2.4.0
    $ ./configure --prefix=/ --datarootdir=/usr/share --with-linux=/lib/modules/$(uname -r)/build
    $ make
    $ sudo make install

### Open vSwitch (OVSDB) with DPDK support

Before installing OvS with DPDK, you must download and compile the DPDK library. At first, download
the source code from:

	http://dpdk.org/browse/dpdk/snapshot/dpdk-2.1.0.tar.gz
	
Then execute the following commands:

    $ tar -xf dpdk-2.1.0.tar.gz
    $ cd dpdk-2.1.0
    $ export DPDK_DIR=\`pwd\`
    ; modify the file `$DPDK_DIR/config/common_linuxapp` so that 
    ; `CONFIG_RTE_BUILD_COMBINE_LIBS=y`
    ; `CONFIG_RTE_LIBRTE_VHOST=y`

To compile OvS with the DPDK support, execute:

	$ make install T=x86_64-ivshmem-linuxapp-gcc
	$ export DPDK_BUILD=$DPDK_DIR/x86_64-ivshmem-linuxapp-gcc/

Details on the DPDK ports, namely `user space vhost` and `ivshmem`, are available
on the [DPDK website](http://dpdk.org/)

Now, download the Open vSwitch source code:

    $ git clone https://github.com/openvswitch/ovs
    
Then execute the following commands:

    $ cd ovs
	$ ./boot.sh
	$ ./configure --with-dpdk=$DPDK_BUILD
	$ make
	$ sudo make install
	
Now create the ovsbd database:	
	
	$ mkdir -p /usr/local/etc/openvswitch
	$ mkdir -p /usr/local/var/run/openvswitch
	$ rm /usr/local/etc/openvswitch/conf.db
	$ sudo ovsdb-tool create /usr/local/etc/openvswitch/conf.db  \
		/usr/local/share/openvswitch/vswitch.ovsschema

## Virtual Execution Environment for network functions

The current un-orchestrator supports different types of execution environments.
You have to install the ones that you want to use, selecting one or more
possibilities from the ones listed in this section.

### Docker

In order to support the Docker execution environment, follow the instructions
provided here:

	http://docs.docker.com/installation/  

### QEMU/KVM/Libvirt

This is needed in order to run network functions in KVM-based virtual machines.
Two flavors of virtual machines are supported:

  * virtual machines that exchange packets with the vSwitch through the `virtio` driver. This configuration allows you to run both traditional processes and DPDK-based processes within the virtual machines. In this case, the host backend for the virtual NICs is implemented through `vhost` in case OvS and xDPd as vSwitches, and through `vhost-user` when OvS-DPDK is used as vSwitch;
  * virtual machines that exchange packets with the vSwitch through shared memory (`ivshmem`). This configuration is oriented to performance, and only supports DPDK-based processes within the virtual machine.
	  
#### Standard QEMU/KVM (without `ivshmem` support)

To install the standard QEMU/KVM/Libvirt execution environment, execute the 
following command:

	$ sudo apt-get install libvirt-dev qemu-kvm libvirt-bin bridge-utils qemu-system  

##### Libvirt with support to `vhost-user` ports

If you intend to use (DPDK) `vhost-user` ports, a recent version of Libvirt must 
be used that supports configuration of this type of ports. You can build it from 
sources using the following commands:

	$ sudo apt-get install libxml-xpath-perl libyajl-dev libdevmapper-dev libpciaccess-dev libnl-dev
	$ git clone git://libvirt.org/libvirt.git
	; select the commit that is known to work and have the necessary support
	$ git checkout f57842ecfda1ece8c59718e62464e17f75a27062
	$ cd libvirt
	$ ./autogen.sh
	$ make
	$ sudo make install

In case you already had libvirt installed on the system, this will install an 
alternative version which must then be used instead of the default one:

	; Stop any running Libvirtd instance and run the alternative version just installed:
	$ sudo service libvirt-bin stop
	$ sudo /usr/local/sbin/libvirtd --daemon

Similarly, if you use virsh, you'd have to use the version from `/usr/local/bin`.

#### QEMU with `ivshmem` support

To compile and install the QEMU/KVM execution environment with the support to `ivshmem`,
further steps are required:

	$ git clone https://github.com/01org/dpdk-ovs
	$ cd dpdk-ovs/qemu
	$ mkdir -p bin/
	$ cd bin
	$ sudo apt-get install libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev
	$ ../configure
	$ make
	$ sudo make install

### DPDK processes

In order to run VNFs implemented as DPDK processes, no further operation is required,
since the DPDK library have already been installed together with the vSwitch.

## NF-FG library

These steps are mandatory only if you plan to use the Network Functions - 
Forwarding Graph (NF-FG) defined in WP3.

	; Retrieve the NF-FG library.
	
	; Copy the library in the un-orchestrator folder
	$ cp [nffg]/virtualizer3.pyc [un-orchestrator]/orchestrator/node_resource_manager/virtualizer      

Finally, remember to select the proper `cmake` option when compiling the `un-orchestrator`.


## Compile the un-orchestrator

We are now ready to compile the un-orchestrator.

	$ cd orchestrator

	; Choose among possible compilation options
	$ ccmake .  

The previous command allows you to select some configuration parameters for the
un-orchestrator, such as the virtual switch used, which kind of execution environment(s)
you want to enable, the NF-FG format to use (the default WP5 one or the one defined
in WP3), etc. When you're finished, exit from
the 'ccmake' interface by *generating the configuration files* (press 'c' and 'g')
and type the following commands:

	; Create makefile scripts based on the previously selected options
	$ cmake .

	; Compile and create the executable
	$ make
