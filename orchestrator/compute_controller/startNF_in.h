#ifndef StartNFIn_H_
#define StartNFIn_ 1

#pragma once

#include <string>
#include <map>
#include <inttypes.h>
#include <list>

/**
* @file StartNFIn_in.h
*
* @brief Parameters to be used to start the network function.
*/

using namespace std;

class StartNFIn
{

friend class ComputeController;

private:

	/**
	*	@brief: identifier of the LSI to which the VNF is connected
	*/
	uint64_t lsiID;
	
	/**
	*	@brief: name of the network function
	*/
	string nf_name;
	
	/**
	*	@brief: list of names of ports of the vSwitch that are associated with the network function
	*/
	list<string> namesOfPortsOnTheSwitch;
		
	/**
	*	@brief: mask of the cores to be assigned to the network functon.
	*			0x0 means that no binding has to be done
	*/
	uint64_t coreMask;

protected:
	StartNFIn(uint64_t lsiID, string nf_name, list<string> namesOfPortsOnTheSwitch, uint64_t coreMask = 0x0)
		: lsiID(lsiID), nf_name(nf_name), namesOfPortsOnTheSwitch(namesOfPortsOnTheSwitch), coreMask(coreMask)
	{
	}
	
public:

	uint64_t getLsiID()
	{
		return lsiID;
	}
	
	string getNfName()
	{
		return nf_name;
	}
	
	list<string> getNamesOfPortsOnTheSwitch()
	{
		return namesOfPortsOnTheSwitch;
	}
	
	uint64_t getCoreMask()
	{
		return coreMask;
	}
};


#endif //StartNFIn_H_
