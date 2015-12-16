## Native example VNF

This file contains the instructions to build an example of native network function that marks traffic exploiting ebtables.


This function works as follows: when a packet is received on the interface, it 
is marked with a specific value and then it is forwarded to the other interface.


The mark value is an hexadecimal value built as follows:

	0x<lsi_id>cade<port_n>
	
where:  
  * `lsi_id`	is the id of the LSI of the graph in hexadecimal format  
  * `cade`	is a fixed 2 bytes value specific for this network function;
                other network functions can specify different values  
  * `port_n`	is the number of the port from which the packet is received
	

