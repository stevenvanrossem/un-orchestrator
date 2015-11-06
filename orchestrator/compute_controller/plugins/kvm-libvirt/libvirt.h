#ifndef LIBVIRT_H_
#define LIBVIRT_H_ 1

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <string.h>
#include <locale>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include "../../../utils/logger.h"
#include "../../../utils/constants.h"

#include "../../nfs_manager.h"
#include "../../startNF_in.h"

#ifdef ENABLE_KVM_IVSHMEM
	#include "ivshmem_cmdline_generator.h"
	#include "../../../utils/sockutils.h"
#endif

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

using namespace std;

class Libvirt : public NFsManager
{
private:

#ifndef ENABLE_KVM_IVSHMEM
	
	/**
	*	@bfief: Connection towards Libvirt
	*/
	static virConnectPtr connection;
#else

	/**
	*	@brief: mutex to protect the selection of the TCP port for the monitor
	*/
	static pthread_mutex_t Libvirt_mutex;

	/**
	*	@brief: TCP port to be assigned to the VM monitor to
	*		the next VM to be executed
	*/
	static unsigned int next_tcp_port;
	
	/**
	*	@brief: The map associates each VNF with the TCP port to
	*		be used to connect to it
	*/
	static map<string,string> monitor;
#endif


#ifndef ENABLE_KVM_IVSHMEM	
	/**
	*	@brief:	Open a connection with QEMU/KVM
	*/
	void connect();
	
	/**
	*	@brief: Disconnect from QEMU/KVM
	*/
	void disconnect();
	
	/**
	*	@brief: Custom error handler
	*/
	static void customErrorFunc(void *userdata, virErrorPtr err);
#else
	
	/**
	*	@brief: Send a given command on given TCP port
	*
	*	@param: tcpport		TCP port to be used to send the command
	*	@param:	command		command to be sent
	*/
	bool sendCommand(string tcpport, char *command);
#endif

public:

	Libvirt();
	~Libvirt();
	
	bool isSupported();
	
	bool startNF(StartNFIn sni);
	bool stopNF(StopNFIn sni);
	
#ifdef ENABLE_DIRECT_VM2VM
	bool executeSpecificCommand(string name, string command);
#endif

};

#endif //LIBVIRT_H_
