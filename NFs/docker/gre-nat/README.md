## Docker-based NAT+GRE VNF

This file contains the instructions to build a Docker network function that provides NAT to traffic coming out of a bunch of GRE tunnels.
NAT is based on iptables.

### Configuring the VNF

The configuration of the has to be embedded in the Docker itself; right now, we cannot change dynamically (at run-time) the configuration.

To change the configuration of the VNF, edit the file `start_nat.sh` and `sysctl.conf`.
This file will be pushed automatically in the Docker image, once created, and will be executed at the startup of the VNF.

### Creating the Docker image

Now you can create the Docker image with the VNF by launching the following command:

    sudo docker build --tag="nat" .

This will create the Docker image starting from the base image specified in `Dockerfile`; the new image is stored in the Docker default folder on your filesystem (localhost).

If you want to be more generic and publish the Docker image in a (public or private) repository, you can use the following command:

    sudo docker build --tag="localhost:5000/nat" .
    docker push localhost:5000/nat

This will register your VNF named `nat` in the local registry (given by the the string `localhost:5000`), which has to be up and running on localhost.


