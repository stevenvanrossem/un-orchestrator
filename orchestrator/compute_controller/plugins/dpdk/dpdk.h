#ifndef DPDK_H_
#define DPDK_H_ 1

#pragma once

#include "../../nfs_manager.h"
#include "dpdk_constants.h"

#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

class Dpdk : public NFsManager
{
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
	bool executeSpecificCommand(uint64_t lsiID, string name, string command);
#endif
};

#endif //DPDK_H_
