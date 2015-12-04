#include "ivshmem_cmdline_generator.h"

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#include <rte_config.h>
#include <rte_eal.h>
#include <rte_ivshmem.h>
#include <rte_mempool.h>
#include <rte_ring.h>

#define DPDKR_FORMAT "dpdkr%d"
#define DPDKR_TX_FORMAT DPDKR_FORMAT"_tx"
#define DPDKR_RX_FORMAT DPDKR_FORMAT"_rx"

#define MEMPOOL_METADATA_NAME "OVSMEMPOOL"

bool IvshmemCmdLineGenerator::init = false;
pthread_mutex_t IvshmemCmdLineGenerator::IvshmemCmdLineGenerator_mutex = PTHREAD_MUTEX_INITIALIZER;

bool IvshmemCmdLineGenerator::memorypool_generated = false;
pthread_mutex_t IvshmemCmdLineGenerator::memory_pool_mutex = PTHREAD_MUTEX_INITIALIZER;

IvshmemCmdLineGenerator::IvshmemCmdLineGenerator()
{
	//TODO: handle error in initialization
	dpdk_init();
}


bool IvshmemCmdLineGenerator::dpdk_init(void)
{
	cpu_set_t *c;
	int nCores = 0;
	int i;

	pthread_mutex_lock(&IvshmemCmdLineGenerator_mutex);

	/* does not exist a nicer way to do it? */
	/* XXX: why -n is not required? */
	char * arg[] =
	{
		"./something",
		"--proc-type=secondary",
		"-c",
		"0x01",
		"--",
		NULL
	};

	int argv = sizeof(arg)/sizeof(*arg) - 1;

	if(init)
		goto generated;

	/*
	 * XXX: DPDK versions before bb7c5ab does not reset the getopt library before
	 * using it, so if such library has been used before calling rte_eal_init it
	 * could fail.
	 * bb7c5ab solves the issue and should be included in dpdk 2.2.0
	 */
	optind = 1;
	if(rte_eal_init(argv, (char**)arg) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__, "DPDK can not be initialized");
		pthread_mutex_unlock(&IvshmemCmdLineGenerator_mutex);
		return false;
	}

	init = true;

	/*
	 * rte_eal_init changes the core mask of the thread.
	 * Change it back to all cores
	 */
	nCores = sysconf(_SC_NPROCESSORS_ONLN);

	c = CPU_ALLOC(nCores);
	for(i = 0;  i < nCores; i++)
		CPU_SET(i, c);

	sched_setaffinity(0, nCores, c);

generated:
	pthread_mutex_unlock(&IvshmemCmdLineGenerator_mutex);
	return true;
}

bool IvshmemCmdLineGenerator::get_mempool_cmdline(char * cmdline, int size)
{
	struct rte_mempool * packets_pool;

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Generating command line for memory pool");

	/*lazy dpdk initialization */
	if(!init)
		return false;

	pthread_mutex_lock(&memory_pool_mutex);
	if(memorypool_generated)
		goto generate;

	/* look for the memory pool */
	/*
	* XXX: improve the wasy the memory pool is looked, the name could not
	* always be the same
	*/
	packets_pool = rte_mempool_lookup("ovs_mp_1500_0_262144");
	if(packets_pool == NULL)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"OVS packets mempool can not be found");
		goto error;
	}

	if (rte_ivshmem_metadata_create(MEMPOOL_METADATA_NAME) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Metadata file can not be created");
		goto error;
	}

	if(rte_ivshmem_metadata_add_mempool(packets_pool, MEMPOOL_METADATA_NAME) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"OVS packets mempool can not be added to metadatafile");
		goto error;
	}

generate:
	if (rte_ivshmem_metadata_cmdline_generate(cmdline, size, MEMPOOL_METADATA_NAME) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Command line for mempool can not be generated");
		goto error;
	}
	memorypool_generated = true;
	pthread_mutex_unlock(&memory_pool_mutex);
	return true;

error:
	pthread_mutex_unlock(&memory_pool_mutex);;
	return false;
}

bool
IvshmemCmdLineGenerator::get_port_cmdline(const char * port_name, char * cmdline, int size)
{
	char ring_name[20];
	int port_no;
	struct rte_ring * rx;
	struct rte_ring * tx;

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Generating command line for port '%s'", port_name);

	/*lazy dpdk initialization */
	if(!init)
		return false;

	/* it has to read just one integer that is the port name */
	if(sscanf(port_name, DPDKR_FORMAT, &port_no) != 1)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Port (%s) has bad port name format", port_name);
		return false;
	}

	/* look for the transmission ring */
	snprintf(ring_name, 20, DPDKR_TX_FORMAT, port_no);
	tx = rte_ring_lookup(ring_name);
	if(tx == NULL)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"port (%s) can not be found", port_name);
		return false;
	}

	/* look fot the reception ring */
	snprintf(ring_name, 20, DPDKR_RX_FORMAT, port_no);
	rx = rte_ring_lookup(ring_name);
	if(rx == NULL)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"port (%s) can not be found", port_name);
		return false;
	}

	if (rte_ivshmem_metadata_create(port_name) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Metadata file can not be created");
		return false;
	}

	if(rte_ivshmem_metadata_add_ring(tx, port_name) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Port (%s) can not be added to metadata_file", port_name);
		return false;
	}

	if(rte_ivshmem_metadata_add_ring(rx, port_name) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Port (%s) can not be added to metadata_file", port_name);
		return false;
	}

	if (rte_ivshmem_metadata_cmdline_generate(cmdline, size, port_name) < 0)
	{
		logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
				"Command line can not be generated", port_name);
		return false;
	}

	logger(ORCH_DEBUG_INFO, MODULE_NAME, __FILE__, __LINE__,
			"Command line: '%s'", cmdline);

	return true;
}
