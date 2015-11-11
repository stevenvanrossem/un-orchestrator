#ifndef NF_TYPE_H_
#define NF_TYPE_H_ 1

#include <string>
#include <assert.h>

using namespace std;

//TODO: DPDK should be put optional also in this file

typedef enum{
	DPDK
#if defined(ENABLE_DOCKER) || defined(VSWITCH_IMPLEMENTATION_XDPD)
	,DOCKER
#endif
#ifdef ENABLE_KVM
	,KVM
#endif
#ifdef ENABLE_NATIVE
	,NATIVE
#endif
	,INTERNAL
	//[+] Add here other implementations for the execution environment
	}nf_t;

class NFType
{
public:
	static string toString(nf_t type)
	{
		if(type == DPDK)
			return string("dpdk");
#if defined(ENABLE_DOCKER) || defined(VSWITCH_IMPLEMENTATION_XDPD)
		else if(type == DOCKER)
			return string("docker");
#endif
#ifdef ENABLE_KVM
		else if(type == KVM)
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
	
	static unsigned int toID(nf_t type)
	{
		if(type == DPDK)
			return 0;
#ifdef ENABLE_DOCKER
		else if(type == DOCKER)
			return 1;
#endif
#ifdef ENABLE_KVM
		else if(type == KVM)
			return 2;
#endif
#ifdef ENABLE_NATIVE
		else if(type == NATIVE)
			return 3;
#endif
		else if(type == INTERNAL)
					return 4;

		//[+] Add here other implementations for the execution environment

		assert(0);
		return 0;
	}

	static bool isValid(string type)
	{
		if(type == "dpdk" 
#ifdef ENABLE_DOCKER		
		|| type == "docker"
#endif		
#ifdef ENABLE_KVM
		|| type == "kvm"
#endif
#ifdef ENABLE_NATIVE
		|| type == "native"
#endif
		|| type == "internal"
		)
			return true;
	
		//[+] Add here other implementations for the execution environment
	
		return false;
	}
};

#endif //NF_TYPE_H_
