#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rte_config.h>
#include <rte_eal.h>
#include <rte_ivshmem.h>
#include <rte_mempool.h>
#include <rte_ring.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define DPDKR_FORMAT "dpdkr%d"
#define DPDKR_TX_FORMAT DPDKR_FORMAT"_tx"
#define DPDKR_RX_FORMAT DPDKR_FORMAT"_rx"

#define MEMPOOL_METADATA_NAME "OVSMEMPOOL"

int init_dpdk(void);
int get_mempool_cmdline(char * cmdline, int size);
int get_port_cmdline(const char * port_name, char * cmdline, size_t size);
int write_to_pipe(const char * name, char * buf, size_t lenght);
int write_to_file(const char * name, char * buf, size_t lenght);

int init_dpdk(void)
{
	char * arg[6] = {};

	arg[0] = strdup("./something");
	arg[1] = strdup("--proc-type=secondary");
	arg[2] = strdup("-c");
	arg[3] = strdup("0x01");
	arg[4] = strdup("--");
	arg[5] = NULL;

	int argv = sizeof(arg)/sizeof(*arg) - 1;

	/*
	 * XXX: DPDK versions before bb7c5ab does not reset the getopt library before
	 * using it, so if such library has been used before calling rte_eal_init it
	 * could fail.
	 * bb7c5ab solves the issue and should be included in dpdk 2.2.0
	 */
	optind = 1;
	return rte_eal_init(argv, (char**)arg);
}

int get_mempool_cmdline(char * cmdline, int size)
{
	struct rte_mempool * packets_pool;

	/*
	* XXX: improve the wasy the memory pool is looked, the name could not
	* always be the same
	*/
	packets_pool = rte_mempool_lookup("ovs_mp_1500_0_262144");
	if(packets_pool == NULL)
	{
		goto error;
	}

	if (rte_ivshmem_metadata_create(MEMPOOL_METADATA_NAME) < 0)
	{
		goto error;
	}

	if(rte_ivshmem_metadata_add_mempool(packets_pool, MEMPOOL_METADATA_NAME) < 0)
	{
		goto error;
	}

	if (rte_ivshmem_metadata_cmdline_generate(cmdline, size, MEMPOOL_METADATA_NAME) < 0)
	{
		goto error;
	}

	return 0;

error:
	return -1;
}

int get_port_cmdline(const char * port_name, char * cmdline, size_t size)
{
	char ring_name[20];
	int port_no;
	struct rte_ring * rx;
	struct rte_ring * tx;

	/* it has to read just one integer that is the port name */
	if(sscanf(port_name, DPDKR_FORMAT, &port_no) != 1)
	{
		return -1;
	}

	/* look for the transmission ring */
	snprintf(ring_name, 20, DPDKR_TX_FORMAT, port_no);
	tx = rte_ring_lookup(ring_name);
	if(tx == NULL)
	{
		return -1;
	}

	/* look fot the reception ring */
	snprintf(ring_name, 20, DPDKR_RX_FORMAT, port_no);
	rx = rte_ring_lookup(ring_name);
	if(rx == NULL)
	{
		return -1;
	}

	if (rte_ivshmem_metadata_create(port_name) < 0)
	{
		return -1;
	}

	if(rte_ivshmem_metadata_add_ring(tx, port_name) < 0)
	{
		return -1;
	}

	if(rte_ivshmem_metadata_add_ring(rx, port_name) < 0)
	{
		return -1;
	}

	if (rte_ivshmem_metadata_cmdline_generate(cmdline, size, port_name) < 0)
	{
		return -1;
	}

	return 0;
}
/*
int write_to_pipe(const char * name, char * buf, size_t size)
{
	int fd = open(name, O_WRONLY);

	ssize_t n;
	const char *p = buf;
	while (size > 0)
	{
		n = write(fd, p, size);
		if (n <= 0) break;
		p += n;
		size -= n;
	}

	close(fd);

	return (n <= 0) ? -1 : 0;
}
*/
int write_to_file(const char * name, char * buf, size_t size)
{
	(void) size;

	FILE *f = fopen(name, "w");
	if(f == NULL)
		return -1;

	fprintf(f, "%s\n", buf);
	fclose(f);
	return 0;
}

int main(int argc, char * argv[])
{
	char buf[512];

	if(argc < 2)
		return -1;

	if(init_dpdk() < 0)
	{
		printf("Failed to init dpdk\n");
		return -1;
	}

	if(!strcmp("-m", argv[1]))
	{
		if(get_mempool_cmdline(buf, sizeof(buf)) < 0)
		{
			printf("Failed to get mempool commandline\n");
			return -1;
		}

		//if(write_to_pipe(MEMPOOL_METADATA_NAME, buf, sizeof(buf) < 0))
		//	return -1;
		if(write_to_file(MEMPOOL_METADATA_NAME, buf, sizeof(buf) < 0))
			return -1;

	}
	else if(!strcmp("-p", argv[1]))
	{
		if(argc < 3)
			return -1;

		if(get_port_cmdline(argv[2], buf, sizeof(buf)) < 0)
		{
			printf("Failed to get port commandline\n");
			return -1;
		}

		//if(write_to_pipe(argv[2], buf, sizeof(buf) < 0))
		//	return -1;
		if(write_to_file(argv[2], buf, sizeof(buf) < 0))
			return -1;
	}

	printf("Ready----\n");

	return 0;
}
