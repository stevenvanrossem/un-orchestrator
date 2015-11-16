#ifndef CONSTANTS_H_
#define CONSTANTS_H_ 1

#include "logger.h"

#define MODULE_NAME 			"name-resolver"

/*
*	Connections
*/

#define REST_PORT 				2626
#define BASE_URL 				"nfs"
#define REST_URL 				"http://localhost"
#define DIGEST_URL 				"digest"

/*
*	Rest methods
*/
#define GET						"GET"

/*
*	HTTP headers
*/
#define JSON_C_TYPE				"application/json"
#define NO_CACHE				"no-cache"

/*
*	NFs
*/
#define NETWORK_FUNCTIONS_XSD	"./config/network-functions.xsd"

/*
*	NFs configuration file
*/
#define NETWORK_FUNCTION_ELEMENT	"network-function"
#define NAME_ATTRIBUTE				"name"
#define SUMMARY_ATTRIBUTE			"summary"
#define NUM_PORTS_ATTRIBUTE			"num-ports"
#define TYPE_ATTRIBUTE				"type"
#define URI_ATTRIBUTE				"uri"
#define CORES_ATTRIBUTE				"cores"
#define LOCATION_ATTRIBUTE			"location"
#define DEPENDENCIES_ATTRIBUTE		"dependencies"
#define DPDK_DESCRIPTION			"dpdk"
#define DOCKER_DESCRIPTION			"docker"
#define KVM_DESCRIPTION				"kvm"
#define NATIVE_DESCRIPTION			"native"

/*
*	Misc
*/
#define BUFFER_SIZE				1024


#endif //CONSTANTS_H_
