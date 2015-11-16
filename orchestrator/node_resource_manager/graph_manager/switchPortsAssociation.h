#ifndef SWITCH_PORT_ASSOCIATION_H_
#define SWITCH_PORT_ASSOCIATION_H_ 1

#pragma once

#include <map>
#include <string>
#include <utility>
#include <assert.h>

using namespace std;

class SwitchPortsAssociation
{
friend class GraphManager;

private:
	/**
	*	@brief: this class contains a static map defined as follows
	*
	*		<"name of port on the switch", pair <graph id, network function name>  >
	*/
	static map<string, pair <string, string> > associationportgraphnf;
	
	/**
	*	@brief: mutex to protect the access to the map
	*/
	static pthread_mutex_t association_mutex;

protected:

	/**
	*	@brief: introduce a new association "port, graph ID, network function"
	*
	*	@param: port	name of the port
	*	@param:	graphID	ID of the graph to which the port belongs
	*	@param: nf_name	name of the network function to which the port belong
	*/
	static void setAssociation(string graphID, string port, string nf_name);
	
	/**
	*	@brief: gives the graph ID of the graph to which a port belong
	*
		@param: port	name of the port
	*/
	static string getGraphID(string port);

	/**
	*	@brief: gives the name of the network function to which a port belong
	*
		@param: port	name of the port
	*/
	static string getNfName(string port);
};

class SwitchPortsAssociationException: public exception
{
public:
	virtual const char* what() const throw()
	{
		return "SwitchPortsAssociation";
	}
};


#endif //SWITCH_PORT_ASSOCIATION_H_
