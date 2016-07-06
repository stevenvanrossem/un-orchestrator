# Deployment Scripts (Elastic Router and Traffic Generator)
The deployment scripts have been grouped according to their tasks, i.e.

* **elastic_router** : The scripts in this directory automate the creation of virtual environment for ER-demo by deploying all necessary components.
* **traffic_generator** : The scripts in this directory create a VM that generates traffic to test ER functions.

Both of above directories have their own README files to explain usage.

## Prerequisites
A host machine must install following software components to run these scripts.

### Vagrant
Vagrant is free and open-source solution used to create and configure lightweight, reproducible, and portable development environments. Its [official website](https://www.vagrantup.com/downloads.html) offers installation binaries for different host platform. Note that Vagrant needs a virtualizer (VirtualBox is recommended in here) to operate with.  

### Oracle VM VirtualBox
VirtualBox is a free and open-source general-purpose virtualizer for x86 hardware. Download and install instructions are available on [official website](https://www.virtualbox.org/wiki/Downloads).  
