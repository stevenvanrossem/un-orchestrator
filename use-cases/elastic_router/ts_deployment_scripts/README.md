# Deployment scripts for the troubleshooting VM of the ER demo

Please, read the normal [README](../deployment_scripts/README.md) first.

# Arguments

* **--vm-memory**: configures RAM (in MegaBytes) for the guest virtual machine. The default value is 1024 that accounts for 1GB of RAM.
* **--vm-cores**: configures number of CPU cores to be assigned to the guest virtual machine. The default value is 1.
* **--docker-registry-username**: used to specify username for login at Acreo's docker registry *gitlab.testbed.se:5000/*. A login is required to pull docker images for certain components. This is a mandatory argument. 
* **--docker-registry-password**: used to specify password corresponding to above username for login at Acreo's docker registry *gitlab.testbed.se:5000/*. This is also a mandatory argument.
