#ifndef AddEndpointOut_H_
#define AddEndpointOut_ 1

#pragma once

#include <string>
#include <list>

/**
* @file addEndpoint_out.h
*
* @brief Description of endpoint.
*/

using namespace std;

class AddEndpointOut
{

friend class GraphManager;

private:

	/**
	*	@brief: name of the endpoint that have been connected to the lsi
	*/
	string ep_name;

	/**
	*	@brief: id of the endpoint
	*/
	unsigned int ep_id;

protected:

	string getEPname()
	{
		return ep_name;
	}

	unsigned int getEPid()
	{
		return ep_id;
	}

public:
	AddEndpointOut(string ep_name,unsigned int ep_id)
		: ep_name(ep_name), ep_id(ep_id)
	{
	}

};


#endif //AddEndpointOut_H_
