#ifndef VLAN_ACTION_H_
#define VLAN_ACTION_H_ 1

#define __STDC_FORMAT_MACROS
#include "generic_action.h"

#include <inttypes.h>
#include <iostream>
#include <sstream>

#include <rofl/common/protocols/fvlanframe.h>

//Tthe "ENDPOINT" values are different with the normal one in the generation of the
//json associated with the action
enum vlan_action_t 
{
	ACTION_VLAN_PUSH,
	ACTION_VLAN_POP,
	ACTION_ENDPOINT_VLAN_PUSH,
	ACTION_ENDPOINT_VLAN_POP
};

class VlanAction : public GenericAction
{
private:
	vlan_action_t type;
	string vlan_endpoint;
	uint16_t label;

public:
	VlanAction(vlan_action_t type, string vlan_endpoint, uint16_t label = 0);
	~VlanAction();

	void toJSON(Object &json);

	/**
	*	@brief: insert the generic action into a flowmod message
	*
	*	@param: message		flowmod message
	*	@param: position	position, in the flowmod, in which the action must be inserted
	*/
	void fillFlowmodMessage(rofl::openflow::cofflowmod &message, unsigned int *position);

	vlan_action_t getType();
	void print();
	string prettyPrint();
};


#endif //VLAN_ACTION_H_
