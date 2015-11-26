CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(node-orchestrator)

# Rather complicated CMake code for setting the logging level
ADD_DEFINITIONS(-DLOGGING_LEVEL=ORCH_DEBUG_INFO)
# End of the rather complicated CMake code for setting the logging level

# Rather complicated CMake code for selecting the Openflow version
ADD_DEFINITIONS(-DOFP_VERSION=OFP_12)
# End of the rather complicated CMake code for setting the Openflow version

# Rather complicated CMake code for selecting the virtual switch implementation
ADD_DEFINITIONS(-DVSWITCH_IMPLEMENTATION_OVSDB)

# End of the rather complicated CMake code for setting the Openflow version

# Set source files
SET(SOURCES
	node_orchestrator.cc
	
	node_resource_manager/graph_manager/graph_manager.h
	node_resource_manager/graph_manager/graph_manager.cc
	node_resource_manager/graph_manager/graph_info.h
	node_resource_manager/graph_manager/graph_info.cc
	node_resource_manager/graph_manager/graph_translator.h
	node_resource_manager/graph_manager/graph_translator.cc
	node_resource_manager/graph_manager/rule_removed_info.h
	node_resource_manager/graph_manager/lsi.h
	node_resource_manager/graph_manager/lsi.cc
	node_resource_manager/graph_manager/virtual_link.h
	node_resource_manager/graph_manager/virtual_link.cc
	node_resource_manager/graph_manager/fileParser.h
	node_resource_manager/graph_manager/fileParser.cc
	
	node_resource_manager/graph/match.h
	node_resource_manager/graph/match.cc
	node_resource_manager/graph/generic_action.h
	node_resource_manager/graph/generic_action.cc
	node_resource_manager/graph/vlan_action.h
	node_resource_manager/graph/vlan_action.cc
	
	node_resource_manager/graph/low_level_graph/output_action.h
	node_resource_manager/graph/low_level_graph/output_action.cc
	node_resource_manager/graph/low_level_graph/graph.h
	node_resource_manager/graph/low_level_graph/graph.cc
	node_resource_manager/graph/low_level_graph/low_level_match.h
	node_resource_manager/graph/low_level_graph/low_level_match.cc
	node_resource_manager/graph/low_level_graph/rule.h
	node_resource_manager/graph/low_level_graph/rule.cc
	
	node_resource_manager/graph/high_level_graph/high_level_output_action.h
	node_resource_manager/graph/high_level_graph/high_level_output_action.cc
	node_resource_manager/graph/high_level_graph/high_level_output_action_nf.h
	node_resource_manager/graph/high_level_graph/high_level_output_action_nf.cc
	node_resource_manager/graph/high_level_graph/high_level_output_action_port.h
	node_resource_manager/graph/high_level_graph/high_level_output_action_port.cc
	node_resource_manager/graph/high_level_graph/high_level_output_action_endpoint.h
	node_resource_manager/graph/high_level_graph/high_level_output_action_endpoint.cc
	node_resource_manager/graph/high_level_graph/high_level_rule.h
	node_resource_manager/graph/high_level_graph/high_level_rule.cc
	node_resource_manager/graph/high_level_graph/high_level_graph.h
	node_resource_manager/graph/high_level_graph/high_level_graph.cc
	node_resource_manager/graph/high_level_graph/high_level_match.h
	node_resource_manager/graph/high_level_graph/high_level_match.cc
	
	network_controller/switch_manager/switch_manager.h		
	network_controller/switch_manager/addNFports_in.h
	network_controller/switch_manager/addNFports_out.h
	network_controller/switch_manager/addVirtualLink_in.h
	network_controller/switch_manager/addVirtualLink_out.h
	network_controller/switch_manager/createLSIout.h
	network_controller/switch_manager/destroyNFports_in.h
	network_controller/switch_manager/destroyVirtualLink_in.h 
	network_controller/switch_manager/checkPhysicalPorts_in.h 
	
	network_controller/openflow_controller/controller.h
	network_controller/openflow_controller/controller.cc
	
	compute_controller/nfs_manager.h
	compute_controller/nfs_manager.cc
	compute_controller/compute_controller.h
	compute_controller/compute_controller.cc
	compute_controller/nf.h
	compute_controller/nf.cc
	compute_controller/nf_type.h
	compute_controller/description.h
	compute_controller/description.cc
	compute_controller/startNF_in.h
	compute_controller/stopNF_in.h
	
	node_resource_manager/rest_server/rest_server.h
	node_resource_manager/rest_server/rest_server.cc
	node_resource_manager/rest_server/match_parser.h
	node_resource_manager/rest_server/match_parser.cc

	utils/logger.h
	utils/logger.c
	utils/constants.h
	utils/sockutils.h
	utils/sockutils.c
)

SET(SOURCES1
	${SOURCES}
		network_controller/switch_manager/plugins/ovs-ovsdb/commands.h
		network_controller/switch_manager/plugins/ovs-ovsdb/commands.cc
		network_controller/switch_manager/plugins/ovs-ovsdb/ovsdb_constants.h
		network_controller/switch_manager/plugins/ovs-ovsdb/ovsdb_manager.h
    	network_controller/switch_manager/plugins/ovs-ovsdb/ovsdb_manager.cc
)

# Create the executable
ADD_EXECUTABLE(
	node-orchestrator
	${SOURCES1}
)

INCLUDE_DIRECTORIES (
	/usr/local/include
	/usr/local/lib
	/usr/include/boost
	/usr/include/libxml2
)

TARGET_LINK_LIBRARIES( node-orchestrator
	libmicrohttpd.so
	libboost_system.so
	libxml2.so
	libpthread.so.1
	librofl_common.so.0
	libjson_spirit.so
	-lrt
)
