## Docker-based example VNF

This file contains the instructions to build an example of Docker network function.

This function works as follows: when a packet is received on the interface eth0, its
destination MAC address is  changed, and the packet is sent on the interface eth1.

### Creating the Docker image

You can create the Docker image with the network function by launching the following command:

    sudo docker build --tag="example" .

This will create the Docker image starting from the base image specified in `Dockerfile`; the new image is stored in the Docker default folder on your filesystem (localhost).

If you want to be more generic and publish the Docker image in a (public or private) repository, you can use the following command:

    sudo docker build --tag="localhost:5000/example" .
    docker push localhost:5000/example

This will register your VNF named `example` in the local registry (given by the the string `localhost:5000`), which has to be up and running on localhost.


