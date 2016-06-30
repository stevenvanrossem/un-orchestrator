# Deployment Scripts (Traffic Generator)
These scripts create a VM that generates traffic to feed to ER.   

## Description of Scripts
There are three scripts intended to perform different task towards the creation of virtual environment for ER-demo in guest virtual machine.

* **Vagrantfile**: It fetches an image of ubuntu/trusty64 from Vagrant registry and triggers the execution deployment scripts in it. This script configures memory and number of cores assigned to guest machine. These parameters can be set through command line arguments as explained later in next section. 
* **provioning/run.sh**: A shell script that creates PCAP files using *traffic generator* and then use them to generate of traffic through `tcpreplay`        
 

## Usage
There are certain command line parameters which help customize the virtual environment to be created. A list along with description is provided below.

* **--vm-memory**: configures RAM (in MegaBytes) for the guest virtual machine. The default value is 512 that accounts for 0.5GB of RAM.
* **--vm-cores**: configures number of CPU cores to be assigned to the guest virtual machine. The default value is 1.
* **--sics-gitlab-username**: used to specify username for login at SICS's gitlab repository *https://ghetto.sics.se/nigsics*. A login is required to download *traffic generator* source code. This is a mandatory argument. 
* **--sics-gitlab-password**: used to specify password corresponding to above username. This is also a mandatory argument.

### Up and Running
The following command is used to start the process of building virtual environment where guest virtual machine is configured to use 1GB RAM and 2 CPU cores.

    $ cd <path/to/deployment_scripts/traffic_generator> 
    $ sudo vagrant ---sics-gitlab-username=myusername --sics-gitlab-password=mypassword --vm-memory=1024 --vm-cores=2 up
please replace *myusername* and *mypassword* with actual username and password (without any quotation marks).

This command makes Vagrant to download Ubuntu (trusty64) image from registry and start the provisioning process. It is the provisioning process that in turn executes script *run.sh*.  

### SSH Access
Once guest virtual machine is up, an SSH access is available with following command:
 
    $ sudo vagrant ssh
This will log you in as user *vagrant*. Use *sudo* to get root access.
