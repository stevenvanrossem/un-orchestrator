# KVM-based VNF examples

This folder contains some examples of network functions implemented as KVM virtual machines.

## How to install KVM

In order to create your own VNF, you need to install KVM, QEMU and libvirt by executing the following command:

	$ sudo apt-get install qemu-kvm libvirt-bin bridge-utils qemu-system

## How to create your VNFs

Please check individual README's in each sub-package.
Those files will give you the instruction to create the libvirt template describing the KVM virtual machine that will run the VNF.
Once you have that template, you can pass the link to the UN (by writing the appropriate entry in the name resolver configuration file) in order to instantiate the described virtual machine.
