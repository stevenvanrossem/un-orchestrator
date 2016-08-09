## Native VNF type 5

This file contains instructions to build a native network function that
implements a VNF type 5 based on `openvswitch`.

## Features

The VNF type 5 accomplishes functions directly on an OpenFlow (OF) capable 
switch. In other words, this function is implemented within the switching engine
through OF rules, whereas the VNF is invoked as an explicit element of the
service being instantiated. Nevertheless, the control function for any of the
VNF type 5 functions will need to be deployed as a distinct part of the NF-FG.

## Requirements

This native network function requires the following package:

  * `openvswitch`
  
It can be installed with the command:

	$ sudo apt-get install openvswitch-switch
	
## Implementation details

The VNF type 5 switch establishes an 'out-of-bound' connection to the controller, 
thus the network that this connection traverses is completely separate from the 
one that the switch is controlling. Consequently, the VNF type 5 needs a special
control port through which the switch sends and receives control messages.
In the current implementation, this port is the first port associated to the
VNF, labeled with the id '1'.

As well as all the other VNFs, the number of ports needed by the VNF type 5 must 
be specified in the name-resolver configuration file. For instance, if the OF 
switch needs 3 ports in order to achieve its tasks, the name-resolver entry for
that VNF must have the attribute 'num-ports="4"', since an additional port is
needed for the connection to the controller.

## Deployment example


Among the configuration file examples of the orchestrator you can find the file 
[nf-fg_vnft5_example.json](../../../orchestrator/config/nf-fg_vnft5_example.json) 
that is an example of NF-FG with the deployment of the VNF type 5.
The graph contains two VNFs: an OpenFlow controller called `ctrl` and the VNF
type 5 called `vnft5`. These two VNFs are connected through the rules with id 5 
and 6. Note that the controller has only one port, which is connected to the 
port `vnft5:1`. Moreover, the VNF type 5 is connected to two physical ports.

The name-resolver configuration file must correspondingly contain two entries, 
as in the following extract:

	<network-function name="ctrl" num-ports="1" summary="Controller for VNF type 5">
		<!-- available implementations -->
			...
	</network-function>

	<network-function name="vnft5" num-ports="3" summary="vnft5">
		<native uri="../NFs/native/VNFt5/vnft5.tar.gz" dependencies="openvswitch" location="local"/>
	</network-function>

## Run

Before running this native network function, you must create a tar.gz archive
that contains the files `start`, `stop` and `vswitch.ovsschema` with the
command:

	$ tar -zcf vnft5.tar.gz start stop vswitch.ovsschema

Note that `vnft5.tar.gz` is the name of the file as specified in the uri of the
entry in the name-resolver configuration file.