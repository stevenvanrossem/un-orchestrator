#ifndef NF_TYPE_H_
#define NF_TYPE_H_ 1

#include <string>
#include <assert.h>

using namespace std;

typedef enum{
	DPDK,
	DOCKER,
	KVM,
	NATIVE,
	INTERNAL
	//[+] Add here other implementations for the execution environment
	}nf_t;

class NFType
{
public:
	static string toString(nf_t type)
	{
#ifdef ENABLE_DPDK_PROCESSES
		if(type == DPDK)
			return string("dpdk");
#endif
#if defined(ENABLE_DOCKER) || defined(VSWITCH_IMPLEMENTATION_XDPD)
		if(type == DOCKER)
			return string("docker");
#endif
#ifdef ENABLE_KVM
		if(type == KVM)
			return string("kvm");
#endif
#ifdef ENABLE_NATIVE
		else if(type == NATIVE)
			return string("native");
#endif		
		else if(type == INTERNAL)
			return string("internal");
		//[+] Add here other implementations for the execution environment

		assert(0);
		return "";
	}
	
	static bool isValid(string type)
	{
#ifdef ENABLE_DPDK_PROCESSES
		if(type == "dpdk")
			return true;
#endif
#ifdef ENABLE_DOCKER		
		if(type == "docker")
			return true;
#endif		
#ifdef ENABLE_KVM
		if(type == "kvm")
			return true;
#endif
#ifdef ENABLE_NATIVE
		if(type == "native")
			return true;
#endif
		if(type == "internal")
			return true;
	
		//[+] Add here other implementations for the execution environment
	
		return false;
	}
};

#endif //NF_TYPE_H_
