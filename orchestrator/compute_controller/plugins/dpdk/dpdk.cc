#include "dpdk.h"

bool Dpdk::isSupported()
{
	//TODO: we are assuming that, if dpdk is enabled by compilation,
	//it is supported
	return true;
}

bool Dpdk::startNF(StartNFIn sni)
{
	uint64_t lsiID = sni.getLsiID();
	string nf_name = sni.getNfName();
	uint64_t coreMask = sni.getCoreMask();

	list<string> namesOfPortsOnTheSwitch = sni.getNamesOfPortsOnTheSwitch();
	unsigned int n_ports = namesOfPortsOnTheSwitch.size();

	string uri_image = description->getURI();

	stringstream uri;
	if(description->getLocation() == "local")
		uri << "file://";
	uri << uri_image;

	stringstream command;
	command << PULL_AND_RUN_DPDK_NF << " " << lsiID << " " << nf_name << " " << uri.str() << " " << coreMask <<  " " << NUM_MEMORY_CHANNELS << " " << n_ports;

	for(list<string>::iterator pn = namesOfPortsOnTheSwitch.begin(); pn != namesOfPortsOnTheSwitch.end(); pn++)
		command << " "  << *pn;

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__, "Executing command \"%s\"",command.str().c_str());

	int retVal = system(command.str().c_str());
	retVal = retVal >> 8;

	if(retVal == 0)
		return false;

	return true;
}

bool Dpdk::stopNF(StopNFIn sni)
{
	uint64_t lsiID = sni.getLsiID();
	string nf_name = sni.getNfName();

	stringstream command;

	command << STOP_DPDK_NF << " " << lsiID << " " << nf_name;

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__, "Executing command \"%s\"",command.str().c_str());

	int retVal = system(command.str().c_str());
	retVal = retVal >> 8;

	if(retVal == 0)
		return false;

	return true;

}

#ifdef ENABLE_DIRECT_VM2VM
bool Dpdk::executeSpecificCommand(uint64_t lsiID, string name, string command)
{
	logger(ORCH_ERROR, MODULE_NAME, __FILE__, __LINE__, "It is not possible to send generic commands to the DPDK environment");

	assert(0);

	return false;
}
#endif

