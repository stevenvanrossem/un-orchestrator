#ifndef NFsManager_H_
#define NFsManager_H_ 1

#pragma once

#include "startNF_in.h"
#include "stopNF_in.h"
#include "description.h"

/**
* @file nfs_manager.h
*
* @brief Network functions manager interface. It must be properly implemented for each vSwitch supported by the node.
*/

using namespace std;

class Description;

class NFsManager
{
protected:
	/**
	*	@brief: Description of the network function associated with this manager
	*/
	Description *description;

public:

	virtual ~NFsManager() {}

	/**
	*	@brief: check whether the execution environment is supported or not
	*/
	virtual bool isSupported() = 0;

	/**
	*	@brief:	Retrieve and start the network function
	*
	*	@param: cli	Description of the network function to be started
	*/
	virtual bool startNF(StartNFIn sni) = 0;
	
	/**
	*	@brief: stop the network function
	*
	*	@param: cli	Description of the network function to be stopped
	*/
	virtual bool stopNF(StopNFIn sni) = 0;
	
#ifdef ENABLE_DIRECT_VM2VM
	/**
	*	@brief: Execute a given command, specific for a network function, 
	*		on the execution environment
	*
	*	@paran: lsiID	ID of the lsi to which the network function involved in
	*					the command is attached
	*	@param: name	name of the network function involved in the command
	*	@param: command	command to be executed
	*/
	virtual bool executeSpecificCommand(uint64_t lsiID, string name, string command) = 0;
#endif
		
	/**
	*	@brief: set the description of the network function to be handled by the manager
	*/
	void setDescription(Description *description);
	
	/**
	*	@brief: provide the type of the network function handled by the manager
	*/
	nf_t getNFType();
	
	/**
	*	@brief: returns the number of cores to be associated with the network function
	*			handled by the manager. "" means that no core has to be bound to the
	*			network function.
	*/
	string getCores();
};

class NFsManagerException: public exception
{
public:
	virtual const char* what() const throw()
	{
		return "NFsManagerException";
	}
};

#endif //NFsManager_H_
