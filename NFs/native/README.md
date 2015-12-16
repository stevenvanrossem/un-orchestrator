# Native VNF examples

This folder contains some examples of network functions implemented as native
functions.


## How to create your VNFs

Please check individual README's in each sub-package.
Those files will give you the instruction to create scripts with the proper
names for running and stopping the selected VNF. Then the scripts must be
inserted in a tar.gz archive. 
Once you have that archive, you can pass the link to the UN (by writing the
appropriate entry in the name resolver configuration file) in order to
instantiate the native function in your running environment.

Each native NF will use different capabilities (e.g., `iptables`, `ebtables`,
specific hw accelerators, other scripts or executables, etc.) that must be specified
in the `dependencies` attribute of the appropriate entry in the name resolver
configuration file.

In order to execute a native network function, all of its dependencies must be
found within the node and must be specified in the capabilities.xml
configuration file: 
	"orchestrator/compute_controller/plugins/native/Capabilities.xml"

Please note that the command line of native scripts, in order to be managed
through the un-orchestrator, must be the following:

	$sudo ./script_name $lsi_id $nf_name $n_ports $port_name1 ... $port_nameN
	
where:
	
  * `$lsi_id`	indicates the id of the LSI related to the graph
  * `$nf_name`	indicates the name of the network function
  * `$n_ports`	specifies the number of ports of the network function
  * `$port_name1 ... $port_nameN`	are the names of the ports assigned to
			the network function and connected to the LSI
