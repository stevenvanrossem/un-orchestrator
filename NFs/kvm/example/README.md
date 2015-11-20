## KVM-based example VNF

This folder contains the libvirt template of a virtual machine that will run a VNF.
In the template it is possible to specify the VM requirements in terms of memory and virtual CPUs,
the devices to be attached to the VM itself (e.g., the disk image), and more.

Note that the template describes **the VM that runs the network function**, and not the network function itself.
In fact, the disk imege to be connected to the VM must be already created and configured to run the desired virtual network function.

### Creating your own template

In the template, you have to specify the `<name>` of the VM, the amount of `<memory>` required and the number of virtual CPUs (`<vcpu>`).
In addition, it is important to indicate the operating system running within the VM (through the `<os>` tag), so that the VM execution can be
optimized for such an operating system.

The `<devices>` element indicates the list of devices to be connected to the virtual machine. The most important one is the *disk*, which
specifies the path of the disk to be connected to the VM. For instance, the following excerpt of template specifies that the disk to be connected to the VM is
in `qcow` format and is at `/var/lib/libvirt/images/ubuntu.qcow`.

	<disk type='file' device='disk'>
		<driver name='qemu' type='qcow2'/>
		<source file='/var/lib/libvirt/images/ubuntu.qcow'/>
		<backingStore/>
		<target dev='vda' bus='virtio'/>
		<alias name='virtio-disk0'/>
		<address type='pci' domain='0x0000' bus='0x00' slot='0x05' function='0x0'/>
	</disk>

It is worth noting that it is not necessary to specify the virtual network interfaces (vNICs). In fact, the number of vNICs will be decided by the
orchestrator, according to the graph to be implemented. Instead, the MAC and IP addresses of the vNICs can be specified through in description of the graph,
while the type of the NIC is selected by the UN.
