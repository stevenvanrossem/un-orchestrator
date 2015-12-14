#ifndef IVSHMEM_CMDLINE_GENERATOR_H
#define IVSHMEM_CMDLINE_GENERATOR_H

#pragma once

#include <pthread.h>

#include "../../../utils/logger.h"
#include "../../../utils/constants.h"

class IvshmemCmdLineGenerator
{
private:
	static bool memorypool_generated;
	static pthread_mutex_t memory_pool_mutex;
	static char mempool_cmd[512];

public:
	IvshmemCmdLineGenerator();

	/*
	* @brief:	generates the command line for adding a port to a guest
	*
	* @param:	port_name	Name of the port related to the command line to be generated
	* @param:	cmdline		Buffer where the command line will be saved
	* @param:	size		Size of the buffer for the command line
	*/
	bool get_port_cmdline(const char * port_name, char * cmdline, int size);

	/*
	* @brief:	generates the command line for map ovs mempool into a guest
	*
	* @param:	cmdline		Buffer where the command line will be saved
	* @param:	size		Size of the buffer for the command line
	*/
	bool get_mempool_cmdline(char * cmdline, int size);

private:
	bool read_from_pipe(const char *name, char *buf, size_t len);
	bool read_from_file(const char *name, char *buf, size_t len);
};

#endif //IVSHMEM_CMDLINE_GENERATOR_H
