## Native Secure VPN VNF

This file contains the instructions to build a native network function that
realizes a site-to-site secure VPN exploiting IPsec.

The overall service is depicted in the following figure.

![vpnsec](https://raw.githubusercontent.com/netgroup-polito/un-orchestrator/native_function_support/images/vpn_sec_configuration.png)

The public IP addresses `x.x.x.x` and `y.y.y.y` are the end points
of the IPsec tunnel. All traffic sent between the networks `10.0.1.0/24` and
`10.0.2.0/24` is secured through an AES-CBC encryption scheme with a key length of
128 bits.  

## Requirements

This native network function requires the following packages:

  * `ipsec-tools`  
  
It can be installed with the command:

	$ sudo apt-get install ipsec-tools

## Configuration of IPsec

In order to deploy the service, the scripts `start` and `stop` must be
customized with the proper IP addresses (user's next hop, IPsec local end point
and default gateway). The configuration file `setkey.conf` must be modified with
the proper parameters (IP addresses and encryption **keys**) as well, according 
to the actual configuration of the network.

Also a configuration file for the remote network (`remote_setkey.conf`) is
provided. It must be customized and set on the remote end point with the 
following command:

	$ sudo setkey -f  remote_setkey.conf
	
## Run

Before running this native network function you must create a tar.gz archive
that contains the files `start`, `stop` and `setkey.conf` (already customized)
with the command:

	$ tar -zcf vpn_sec.tar.gz start stop setkey.conf
