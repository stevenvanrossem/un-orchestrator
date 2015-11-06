#ifndef DOCKER_H_
#define DOCKER_H_ 1

#pragma once

#include "../../nfs_manager.h"
#include "docker_constants.h"

#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

class Docker : public NFsManager
{
private:

	/**
	*	@brief: starting from a netmask, returns the /
	*
	*	@param:	netmask	Netmask to be converted
	*/
	unsigned int convertNetmask(string netmask);

public:
	bool isSupported();
	
	bool startNF(StartNFIn sni);
	bool stopNF(StopNFIn sni);

#ifdef ENABLE_DIRECT_VM2VM
	/**
	*	@brief: Execute a given command on the execution environment.
	*
	*	@param: command to be executed.
	*/
	bool executeSpecificCommand(string command);
#endif
};

#endif //DOCKER_H_
