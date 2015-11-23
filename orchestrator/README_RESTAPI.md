# REST API

The un-orchestrator can either accept a NF-FG from a file, or from its REST interface,
that thanks to a small HTTP server embedded in the UN. The main REST commands to be used
to interact with the un-orchestrator (e.g., deploy a new graph, update an existing graph,
etc.) are detailed in this document.

**WARNING: these commands are not valid if you use the NF-FG library defined in
WP3.**

## Main REST commands accepted by the un-orchestrator

Deploy an NF-FG called ``myGraph'' (the NF-FG description must be based on the
formalism defined in WP5 [README_NF-FG.md](README_NF-FG.md))

    PUT /graph/myGraph HTTP/1.1
    Content-Type : application/json

    {
        "flow-graph":
        {
            "VNFs": [
		    {
				"id": "firewall"
			}
		    ],
		    "flow-rules": [
		    {
			    "id": "00000001",
    			"match":
				{
					"port" : "eth0"
				},
				"action":
				{
					"VNF_id": "firewall:1"
				}
			}
		    ]
    	}
    }

The same message used to create a new graph can be used to add "parts" (i.e.,
network functions and flows) to an existing graph. For instance, it is possible
to add a new flow to the NF-FG called ``myGraph'' as follows

    PUT /graph/myGraph HTTP/1.1
    Content-Type : application/json

    {
        "flow-graph":
        {
            "VNFs": [
		    {
				"id": "firewall"
			}
		    ],
		    "flow-rules": [
			{
				"id": "00000002",
				"match":
				{
					"VNF_id" : "firewall:2"
				},
				"action":
				{
					"port": "eth1"
				}
			}
		    ]
    	}
    }

Retrieve the description of the graph with name "myGraph":

	GET /graph/myGraph HTTP/1.1

Delete the graph with name "myGraph"

	DELETE /graph/myGraph HTTP/1.1

Delete the flow with ID "flow_id" from the graph with name "myGraph":

	DELETE /graph/myGraph/flow_id HTTP/1.1

Retrieve information on the available physical interfaces:

	GET /interfaces HTTP/1.1

## Send commands to the un-orchestrator

In order to interact with the un-orchestrator throug its REST API, you can use
your favourite REST tool (e.g., some nice plugins for Mozilla Firefox). Just in
also use the cURL command line tool, such as in the following example (where the
NF-FG to be instantiated is stored in the file ``myGraph.json''):

	curl -i -H "Content-Type: application/json" -d "@myGraph.json" \
		-X PUT  http://un-orchestrator-address:port/graph/myGraph
		

