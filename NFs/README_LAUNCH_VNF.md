# How to launch a virtual network function on the Universal Node

This file details how to deploy and run a virtual network function (VNF) on the Universal Node (UN). This requires the execution of the following main steps (detailed in the remainder of the document):
*	create the desired VNF image;
*	register such a VNF in the name-resolver database;
*	send to the un-orchestrator a Network Functions-Forwarding Graph (NF-FG) including the VNF that has to be instantiated.

### Create the VNF image
The universal node currently supports three types of VNFs: VNFs executed as Docker containers, VNFs executed inside KVM-based virtual machines, and VNFs based on the DPDK library (i.e., DPDK processes).
In order to create your own VNF image, please check individual README's in each sub-package.

### Register the VNF in the name-resolver
Once the VNF image is created, such a VNF must be registered in the name-resolver database (to install the name-resolver, please check [name-resolver/README.md](../name-resolver/README.md)).
This operation requires to edit the configuration file of the name-resolver and reboot the name-resolver itself; an example of such a file is available at [name-resolver/config/example.xml](../name-resolver/config/example.xml).

### Provide the graph description to the un-orchestrator
In order to deploy your VNF on the UN, you must provide to the un-orchestration a NF-FG including such a VNF (to compile and then execute the un-orchestrator, please check the files [orchestrator/README_COMPILE.md](../orchestrator/README_COMPILE.md) and [orchestrator/README_RUN.md](../orchestrator/README_RUN.md)).

The un-orchestrator supports two NF-FG versions:
  * the initial JSON-based format defined in WP5;
  * the new XML-based format defined in WP3 that includes both top-down
    communication (for the actual forwarding graph) and bottom-up primitives
    (for resources and capabilities).

The former format is supported natively and it is described in [orchestrator/README_RESTAPI.md](../orchestrator/README_RESTAPI.md), while the other requires setting up an additional library as described in [orchestrator/README_COMPILE.md](../orchestrator/README_COMPILE.md#nf-fg-library).


## An example

This section shows an example in which a  VNF called `dummy` and executed as a Docker container is deployed and then run on the Universal Node.
This VNF is deployed as part of the service shown in the picture:

![service-graph](https://raw.githubusercontent.com/netgroup-polito/un-orchestrator/master/images/service-graph.png)

To create the VNF image and store it in the local file system of the Universal Node, execute the following command in the folder containing the Docker file describing the VNF:

    sudo docker build --tag="dummy" .

Then, register the new VNF in the name-resolver by adding the following piece of XML to the configuration file of the name-resolver itself:

	<network-function name="dummy"  num-ports="2" description="dummy VNF used to show the usage of the Universal Node">
		<implementation type="docker" uri="dummy"/>
	</network-function>

At this point, prepare a NF-FG and pass it to the un-orchestator, which will take care of executing all the operations required to implement the graph. The graph shown in the picture above can be described in the JSON syntax defined in WP5 as follow:


    {
        "flow-graph": {
            "VNFs": [
            {
                "id": "dummy"
            }
            ],
            "flow-rules": [
            {
                "id": "00000001",
                "match":
                {
                    "port": "eth0"
                },
                "action":
                {
                    "VNF_id": "dummy:1"
                }
            },
            {
                "id": "00000002",
                "match":
                {
                    "VNF_id": "dummy:2"
                },
                "action":
                {
                    "port": "eth1"
                }
            },
            {
                "id": "00000003",
                "match":
                {
                    "port": "eth1"
                },
                "action":
                {
                    "VNF_id": "dummy:2"
                }
            },
            {
                "id": "00000004",
                "match":
                {
                    "VNF_id": "dummy:1"
                },
                "action":
                {
                    "port": "eth0"
                }
            }
            ]
        }
    }

This json can be stored in a file (e.g., `nffg.json`) and provided to the un-orchestrator either through the command line at the boot of the un-orchestrator, or through its REST API. In the latter case, the command to be used is the following:

      curl -i -H "Content-Type: application/json" -d "@nffg.json" \
        -X PUT  http://un-orchestrator-address:port/graph/graphid

where the `graphid` is an alphanumeric string that will uniquely identify your graph in the orchestrator.

At this point the un-orchestrator
*   through the *network controller*: it creates a new LSI, inserts the proper Openflow rules in such an LSI in order to steer the traffic among the VNFs of the graph, and inserts the proper Openflow rules in the LSI-0 (which is the only LSI connected to the physical interfaces) in order to inject the proper traffic in the graph, and properly handle the network packets exiting from such a graph;
*   through the *compute controller*: it starts the Docker image implementing the VNF with name *dummy* (the image associated with the name of the VNF is obtained through the name-resolver).

The following picture shows how the NF-FG of the example is actually implemented on the UN; in particular, it depicts the connections among LSIs and the VNF, and the rules in the flow tables of the involved LSIs.

![deployment](https://raw.githubusercontent.com/netgroup-polito/un-orchestrator/master/images/deployment.png)

To conclude, the deployment of a second graph will trigger the creation of a new LSI, again connected with the LSI-0; the LSI-0 will then be instructed to properly dispatch the traffic coming from the physical ports among the deployed NF-FGs, according the the NF-FGs themselves.
