#include "ivshmem_cmdline_generator.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MEMPOOL_METADATA_NAME "OVSMEMPOOL"

bool IvshmemCmdLineGenerator::memorypool_generated = false;
pthread_mutex_t IvshmemCmdLineGenerator::memory_pool_mutex = PTHREAD_MUTEX_INITIALIZER;

char IvshmemCmdLineGenerator::mempool_cmd[512];

IvshmemCmdLineGenerator::IvshmemCmdLineGenerator()
{

}

bool IvshmemCmdLineGenerator::get_mempool_cmdline(char * cmdline, int size)
{
	int r;

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Generating command line for memory pool");

	pthread_mutex_lock(&memory_pool_mutex);
	if(memorypool_generated)
		goto ready;

	//unlink(MEMPOOL_METADATA_NAME);
	//if(mkfifo(MEMPOOL_METADATA_NAME, 0666) == -1)
	//{
	//	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
	//		"Error creating named pipe: %s", strerror(errno));
	//	goto error;
	//}

	r = system("./compute_controller/plugins/kvm-libvirt/cmdline_generator/build/cmdline_generator -m");
	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Command line executed");
	if(r == -1 || WEXITSTATUS(r) == -1)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Error executing command line generator");
		//unlink(MEMPOOL_METADATA_NAME);
		goto error;
	}

	if(!read_from_file(MEMPOOL_METADATA_NAME, mempool_cmd, sizeof(mempool_cmd)))
	{
		//unlink(MEMPOOL_METADATA_NAME);
		goto error;
	}

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Command line ready");


	unlink(MEMPOOL_METADATA_NAME);

	memorypool_generated = true;

ready:
	strncpy(cmdline, mempool_cmd, size);
	pthread_mutex_unlock(&memory_pool_mutex);
	return true;

error:
	pthread_mutex_unlock(&memory_pool_mutex);;
	return false;
}

bool
IvshmemCmdLineGenerator::get_port_cmdline(const char * port_name, char * cmdline, int size)
{
	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Generating command line for port '%s'", port_name);

	char c[100];
	int r;

	sprintf(c, "./compute_controller/plugins/kvm-libvirt/cmdline_generator/build/cmdline_generator -p %s", port_name);

	//if(mkfifo(port_name, 0666) == -1)
	//{
	//	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
	//		"Error creating named pipe");
	//	goto error;
	//}

	r = system(c);
	if(r == -1 || WEXITSTATUS(r) == -1)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Error executing command line generator");
		//unlink(MEMPOOL_METADATA_NAME);
		goto error;
	}

	if(!read_from_file(port_name, cmdline, size))
	{
		//unlink(port_name);
		goto error;
	}

	unlink(port_name);

	return true;
error:
	return false;
}

/*
bool IvshmemCmdLineGenerator::read_from_pipe(const char *name, char *buf, size_t len)
{
	int fd = open(name, O_RDONLY);
	if(fd == -1)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Error opening pipe");
		return false;
	}

	int n;
	int total = 0;
	char * p = buf;

	do
	{
		n = read(fd, p, len - total);
		if(n == -1)
		{
			logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Error reading from pipe");
			return false;
		}

		total += n;
		p += n;
	} while(n != 0);

	return true;
}
*/
bool IvshmemCmdLineGenerator::read_from_file(const char *name, char *buf, size_t len)
{
	FILE * f = fopen(name, "r");
	if(f == NULL)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Error opening file");
		return false;
	}

	fgets(buf, len, f);

	return true;
}

