# DPDK-based VNF examples

This folder contains examples of network functions implemented as DPDK secondary
processes.

## How to install DPDK

DPDK can be installed along with the virtual switch. It is in fact mandatory in case of
xDPd and OvS-DPDK, which are the two vSwitches supporting VNFs executed as DPDK processes.

### xDPd

In order to install xDPd and the DPDK library, you have to follow the steps below.

	$ git clone https://github.com/bisdn/xdpd
	$ cd xdpd/

	;Install all the libraries required by the README provided in this folder
	$ bash autogen
	$ cd build
	$ ../configure --with-hw-support=gnu-linux-dpdk --with-plugins="node_orchestrator rest"
	$ make
	$sudo make install

Now the DPDK library, which is being used by xDPd, must be properly
configured, which can be done by launching a script that allows you to:

  * build the environment x86_64-native-linuxapp-gcc
  * Insert IGB UIO module
  * Insert KNI module
  * Setup hugepage mappings for non-NUMA systems (1000 should be a
    reasonable number)
  * Bind Ethernet device to IGB UIO module (bind all the ethernet
    interfaces that you want to use)

Let's now launch the DPDK setup script:

	$ cd ../libs/dpdk/tools
	$ sudo ./setup.sh
	
Note that DPDK has to be reconfigured at each reboot of the machine

**WARNING: Currently, xDPd is not compiling on Linux kernels newer than 3.16.0-30.**

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

Configure the system (after each reboot of the physical machine):

    $ sudo su
    ; Set the huge pages of 2MB; 4096 huge pages should be reasonable.
    $ echo 4096 > /proc/sys/vm/nr_hugepages
	
    ; Umount previous hugepages dir
    $ umount /dev/hugepages
    $ rm -r /dev/hugepages
	
    ; Mount huge pages directory
    $ mkdir /dev/hugepages
    $ mount -t hugetlbfs nodev /dev/hugepages
	
Set up DPDK (after each reboot of the physical machine):

    $ sudo modprobe uio
    $ sudo insmod [dpdk-folder]/x86_64-ivshmem-linuxapp-gcc/kmod/igb_uio.ko
    ; Bind the physical network device to `igb_uio`. The following row
    ; shows how to bind eth1. Repeat the command for each network interface
    ; you want to bind.
    $ [dpdk-folder]/tools/dpdk_nic_bind.py --bind=igb_uio eth1

## How to create your VNFs

Please check individual README's in each sub-package.
Those files will give you the instruction to create the binary for the selected VNF.
Once you have that binary, you can pass the link to the UN (by writing the appropriate entry in the name resolver configuration file) in order to instantiate it in your running environment.

Please note that the command line of DPDK processes, in order to be managed through the un-orchestrator, must be the following:

	$ sudo ./vnfname -c $coremask -n $memchannels --proc-type=secondary -- p --$port1
		... --p portN --s $s --l $log
		
where:

  * `$coremask` indicates the cores to be assigned to the application (note that this parameter is required by DPDK)
  * `$memchannels` is the number of memory channels (note that this parameter is required by DPDK)
  * `$port1 ... $portN` are the ports to be used by the application
  * `$s` useless. It will be removed soon
  * `$log` name of the file to be used by the application to print log information

**Warning:** xDPd and OvS-DPDK use different names for the rte_rings associated with a port,
hence your application must be properly written according to the vSwitch you plan to use:

  * *xDPd*: $portname`-to-nf` to be used by the application to receive packets,
			and $portname`-to-xdpd` to be used by the application to send packets.
  * *OvS-DPDK*: $portname`_tx` to be used by the application to receive packets,
			and $portname`_rx` to be used by the application to send packets.

