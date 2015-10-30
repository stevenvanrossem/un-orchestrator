#ifndef IVSHMEM_CMDLINE_GENERATOR_H
#define IVSHMEM_CMDLINE_GENERATOR_H

#pragma once

#include <stdio.h>
#include <string.h>

#include "../../../utils/logger.h"
#include "../../../utils/constants.h"

class IvshmemCmdLineGenerator
{
private:
	static bool init;

	static bool dpdk_init(void);

public:
	IvshmemCmdLineGenerator();

	/*
	* @brief:	initializes dpdk as a secondary process using fake arguments
	*
	* @param:	port_name	Name of the port related to the command line to be generated
	* @param:	cmdline		Empty string that will contain the command line after the execution of the function
	* @param:	size		Size of the buffer for the command line
	*/
	bool get_cmdline(const char * port_name, char * cmdline, int size);
};

#endif //IVSHMEM_CMDLINE_GENERATOR_H