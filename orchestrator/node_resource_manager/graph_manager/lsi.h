#ifndef LSI_H_
#define LSI_H_ 1

#pragma once

#include "virtual_link.h"
#include "../../compute_controller/nf_type.h"

#include <map>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class VLink;

class LSI
{
//XXX: this class is a mess!
friend class GraphManager;

private:

	/**
	*	@brief: this is the address of the OF controller for this LSI
	*/
	string controllerAddress;
	
	/**
	*	@brief: this is the port used by the OF controller for the LSI
	*/
	string controllerPort;
	
	/**
	*	@brief: data plane identifier
	*/
	uint64_t dpid;
	
	/**
	*	@brief: the pair is <port name, port id>. It contains physical ports
	*/
	map<string,unsigned int> physical_ports;
	
	
	/**
	*	@brief: the pair is <port name, port type>
	*/
	map<string,string> ports_type;
	
	/**
	*	@brief: NFs connected to the LSI.
	*		The map is
	*			<nf name, map <nf port name, id> >
	*/
	map<string,map<string, unsigned int> >  network_functions;
	
	/**
	*	@brief: Names of the ports connected to the LSI and related to network functions
	*		The map is
	*			<nf name, list<nf ports name> >
	*/
    map<string,list<string> > networkFunctionsPortsNameOnSwitch;
	
	/**
	*	@brief: type of the NFs connected to the LSI.
	*		The map is
	*			<nf_name, nf_type>
	*/
	map<string,nf_t>  nf_types;

	/**
	*	@brief: virtual links attached to the LSI
	*	FIXME although supported in this class VLink, the code does not support vlinks connected to multiple LSIs
	*/
	vector<VLink> virtual_links;

	/**
	*	@brief: the map is <nf name, vlink id>
	*		A NF port generates a vlink if it is defined in the action part of a rule
	*/
	map<string, uint64_t> nfs_vlinks;

	/**
	*	@brief: the map is <port name, vlink id>
	*		A physical port generates a vlink if it is defined in the action part of a rule
	*/
	map<string, uint64_t> ports_vlinks;

	/**
	*	@brief: the map is <endpoint name, vlink id>
	*		An endpoint generates a vlink if it is defined in the action part of a rule
	*/
	map<string, uint64_t> endpoints_vlinks;

	/**
	 * 	@brief: the map is <of_bridgeNF name, L3-LSI IS>
	 */
	map<string, uint64_t> ofBridgeIDs;

public:
	LSI(string controllerAddress, string controllerPort, map<string,string> ports,
			map<string, list <unsigned int> > network_functions,vector<VLink> virtual_links,
			map<string,nf_t>  nf_types, map<string, uint64_t> ofBridgeIDs);

	string getControllerAddress();
	string getControllerPort();

	list<string> getPhysicalPortsName();

	set<string> getNetworkFunctionsName();
	list<string> getNetworkFunctionsPortNames(string nf);

	list<uint64_t> getVirtualLinksRemoteLSI();

	uint64_t getDpid();


	map<string,unsigned int> getPhysicalPorts();
	map<string,string> getPortsType();

	map<string,unsigned int> getNetworkFunctionsPorts(string nf);
	list<string> getNetworkFunctionsPortsNameOnSwitch(string nf);

	map<string,nf_t> getNetworkFunctionsType();

	vector<VLink> getVirtualLinks();
	VLink getVirtualLink(uint64_t ID);
	map<string, uint64_t> getNFsVlinks();
	map<string, uint64_t> getPortsVlinks();
	map<string, uint64_t> getEndPointsVlinks();

	/*
	 * Returns the LSI ID of the remote L3-LSI corresponding to the name of the NF
	 *
	 * @param	of_bridgeNFname	name of the NF that implements a of_bridge
	 * @return 					LSI-ID corresponding to that of_bridge
	 */
	uint64_t getOfBridgeID(string of_bridgeNFname);

	/*
	 * Returns LSI IDs of the remote L3-LSIs
	 *
	 * @return 					map of_bridgeNFname->L3-LSI-ID for this LSI
	 */
	map<string, uint64_t> getOFBridgeIDs();

	/*
	 * Returns the LSI ID of the remote L3-LSI corresponding to the name of the NF
	 *
	 * @param	of_bridgeNFname	name of the NF that implements a of_bridge
	 * @param 	LSI-ID corresponding to that of_bridge
	 */
	void addOFBridge(string of_bridgeNFname, uint64_t dpid);



	//FIXME: public is not a good choice
	void setNFsVLinks(map<string, uint64_t> nfs_vlinks);
	void addNFvlink(string NF, uint64_t vlinkID);
	void removeNFvlink(string nf_port);

	void setPortsVLinks(map<string, uint64_t> ports_vlinks);
	void addPortvlink(string port, uint64_t vlinkID);
	void removePortvlink(string port);

	void setEndPointsVLinks(map<string, uint64_t> endpoints_vlinks);
	void addEndpointvlink(string endpoint, uint64_t vlinkID);
	void removeEndPointvlink(string endpoint);

protected:
	void setDpid(uint64_t dpid);
	bool setPhysicalPortID(string port, uint64_t id);
	bool setNfPortsID(string nf, map<string, unsigned int>);
	void setVLinkIDs(unsigned int position, unsigned int localID, unsigned int remoteID);
	
	void setNetworkFunctionsPortsNameOnSwitch(string nf, list<string> names);

	int addVlink(VLink vlink);
	void removeVlink(uint64_t ID);

	void addNF(string name, list< unsigned int> ports, nf_t type);
	void removeNF(string nf);
};

#endif //LSI_H_
