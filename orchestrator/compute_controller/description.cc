#include "description.h"

Description::Description(nf_t type, string uri) : type(type), uri(uri)
{
	supported = false;
}

Description::Description(string type, string uri) : uri(uri) {

	supported = false;

	if(type == "dpdk")
	{
		this->type = DPDK;
		return;
	}
#ifdef ENABLE_DOCKER
	else if(type == "docker")
	{
		this->type = DOCKER;
		return;
	}
#endif	
#ifdef ENABLE_KVM
	else if(type == "kvm")
	{
		this->type = KVM;
		return;
	} 
#endif
#ifdef ENABLE_NATIVE
	else if(type == "native")
	{
		this->type = NATIVE;
		return;
	}
#endif	

	//[+] Add here other implementations for the execution environment

	assert(0);
	return;
}

Description::~Description(){}

nf_t Description::getType()
{
	return type;
}

string Description::getURI()
{
	return uri;
}

bool Description::isSupported() {
	return supported;
}

void Description::setSupported(bool supported) {
	this->supported = supported;
}

