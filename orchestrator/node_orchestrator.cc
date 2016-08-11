#define __USE_GNU 1
#define _GNU_SOURCE 1

#include "utils/constants.h"
#include "utils/logger.h"
#include "node_resource_manager/rest_server/rest_server.h"

#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
	#include "node_resource_manager/pub_sub/pub_sub.h"
#endif

#ifdef ENABLE_RESOURCE_MANAGER
	#include "node_resource_manager/resource_manager/resource_manager.h"
#endif

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>
#include "node_resource_manager/database_manager/SQLite/SQLiteManager.h"

#include <INIReader.h>

#include <signal.h>
#include <execinfo.h>
#include <sys/types.h>
#include <ucontext.h>
#ifdef __x86_64__
	#define USE_REG REG_RIP
//#else
//	#define USE_REG REG_EIP
#endif

static const char LOG_MODULE_NAME[] = "Local-Orchestrator";

/**
*	Global variables (defined in ../utils/constants.h)
*/
ofp_version_t OFP_VERSION;

/**
*	Private variables
*/
struct MHD_Daemon *http_daemon = NULL;

/*
*
* Pointer to database class
*
*/
SQLiteManager *dbm = NULL;

/**
*	Private prototypes
*/
bool parse_command_line(int argc, char *argv[],int *core_mask,char **config_file);
bool parse_config_file(char *config_file, int *rest_port, bool *cli_auth,  map<string,string> &boot_graphs, set<string> &physical_ports, char **descr_file_name, char **client_name, char **broker_address, char **key_path, bool *orchestrator_in_band, char **un_interface, char **un_address, char **ipsec_certificate, string &name_resolver_ip, int *name_resolver_port);

bool usage(void);
void printUniversalNodeInfo();
void terminateRestServer(void);

/**
*	Implementations
*/
void signal_handler(int sig, siginfo_t *info, void *secret)
{
	switch(sig)
	{
		case SIGINT:
			ULOG_INFO( "The '%s' is terminating...",MODULE_NAME);

			MHD_stop_daemon(http_daemon);
			terminateRestServer();

			if(dbm != NULL) {
				//dbm->updateDatabase();
				dbm->cleanTables();
			}
#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
			DoubleDeckerClient::terminate();
#endif
			ULOG_INFO( "Bye :D");
			exit(EXIT_SUCCESS);
		break;
#ifdef __x86_64__
		//We print the stack only if the orchestrator is executed on an x86_64 machine
		case SIGSEGV:
		{
			void *trace[16];
			char **messages = (char **)NULL;
			int i, trace_size = 0;
			ucontext_t *uc = (ucontext_t *)secret;
			char *ret;
			ULOG_ERR( "");
			ULOG_ERR( "Got signal %d, faulty address is %p, from %p", sig, info->si_addr, uc->uc_mcontext.gregs[USE_REG]);

			trace_size = backtrace(trace, 16);
			/* overwrite sigaction with caller's address */
			trace[1] = (void *)uc->uc_mcontext.gregs[USE_REG];

			messages = backtrace_symbols(trace, trace_size);
			/* skip first stack frame (points here) */
			ULOG_ERR( "Backtrace -");
			for (i = 1; i < trace_size; ++i)
			{
				ULOG_ERR( "%s ", messages[i]);
				size_t p = 0;
				while (messages[i][p] != '(' && messages[i][p] != ' ' && messages[i][p] != 0)
					++p;
				char syscom[256];
				sprintf(syscom, "addr2line -f -p %p -e %.*s", trace[i], (int)p, messages[i]);

				char *output;
				FILE *fp;
				char path[1035];

				/* Open the command for reading. */
				fp = popen(syscom, "r");
				if (fp == NULL) {
					printf("Failed to run command %s", syscom);
				}
				ret = fgets(path, sizeof(path), fp);
				if (ret != path) {
					exit(EXIT_FAILURE);
				}
				fclose(fp);

				output = strdup(path);

				if (output != NULL)
				{
					ULOG_ERR( "%s", output);
					free(output);
				}
			}
			exit(EXIT_FAILURE);
		}
		break;
#endif
	}
}

int main(int argc, char *argv[])
{
	//Check for root privileges
	if(geteuid() != 0)
	{
		ULOG_ERR( "Root permissions are required to run %s\n",argv[0]);
		ULOG_ERR( "Cannot start the %s",MODULE_NAME);
		exit(EXIT_FAILURE);
	}

#ifdef VSWITCH_IMPLEMENTATION_ERFS
	OFP_VERSION = OFP_13;
#else
	OFP_VERSION = OFP_12;
#endif

	int core_mask;
	int rest_port, t_rest_port;
	bool cli_auth, t_cli_auth, orchestrator_in_band, t_orchestrator_in_band;
	char *config_file_name = new char[BUFFER_SIZE];
	set<string> physical_ports;
	map<string,string> boot_graphs;
	char *descr_file_name = new char[BUFFER_SIZE], *t_descr_file_name = NULL;
#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
	char *client_name = new char[BUFFER_SIZE];
	char *broker_address = new char[BUFFER_SIZE];
	char *key_path = new char[BUFFER_SIZE];
#endif
	char *t_client_name = NULL, *t_broker_address = NULL, *t_key_path = NULL;
	char *un_interface = new char[BUFFER_SIZE], *t_un_interface = NULL;
	char *un_address = new char[BUFFER_SIZE], *t_un_address = NULL;
	char *ipsec_certificate = new char[BUFFER_SIZE], *t_ipsec_certificate = NULL;

	string name_resolver_ip;
	int name_resolver_port;

	string s_un_address;
	string s_ipsec_certificate;

	strcpy(config_file_name, DEFAULT_FILE);

	if(!parse_command_line(argc,argv,&core_mask,&config_file_name))
	{
		ULOG_ERR( "Cannot start the %s",MODULE_NAME);
		exit(EXIT_FAILURE);
	}

	if(!parse_config_file(config_file_name,&t_rest_port,&t_cli_auth,boot_graphs,physical_ports,&t_descr_file_name,&t_client_name,&t_broker_address,&t_key_path,&t_orchestrator_in_band,&t_un_interface,&t_un_address,&t_ipsec_certificate, name_resolver_ip, &name_resolver_port))
	{
		ULOG_ERR( "Cannot start the %s",MODULE_NAME);
		exit(EXIT_FAILURE);
	}

	if(strcmp(t_descr_file_name, "UNKNOWN") != 0)
		strcpy(descr_file_name, t_descr_file_name);
	else
		descr_file_name = NULL;

#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
	//The following parameters ara mandatory in case of DD connection
	strcpy(client_name, t_client_name);
	strcpy(broker_address, t_broker_address);
	strcpy(key_path, t_key_path);
#endif

	if(strcmp(t_un_interface, "UNKNOWN") != 0)
		strcpy(un_interface, t_un_interface);
	else
		un_interface = "";

	if(strcmp(t_un_address, "UNKNOWN") != 0)
		strcpy(un_address, t_un_address);
	else
		un_address = "";

	if(strcmp(t_ipsec_certificate, "UNKNOWN") != 0)
		strcpy(ipsec_certificate, t_ipsec_certificate);
	else
		ipsec_certificate = "";

	rest_port = t_rest_port;
	cli_auth = t_cli_auth;
	orchestrator_in_band = t_orchestrator_in_band;

	if(!string(un_address).empty())
		s_un_address = string(un_address);
	if(!string(ipsec_certificate).empty())
		s_ipsec_certificate = string(ipsec_certificate);

	if(!string(un_address).empty())
	{
		//remove " character from string
		s_un_address.erase(0,1);
		s_un_address.erase(s_un_address.size()-1,1);
	}

	if(!string(ipsec_certificate).empty())
	{
		s_ipsec_certificate.erase(0,1);
		s_ipsec_certificate.erase(s_ipsec_certificate.size()-1,1);
	}

	if(cli_auth) {
		std::ifstream ifile(DB_NAME);

		if(ifile)
			dbm = new SQLiteManager(DB_NAME);
		else {
			ULOG_ERR( "Database does not exist!");
			ULOG_ERR( "Run 'db_initializer' at first.");
			ULOG_ERR( "Cannot start the %s",MODULE_NAME);
			exit(EXIT_FAILURE);
		}
	}

#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
	if(!DoubleDeckerClient::init(client_name, broker_address, key_path))
	{
		ULOG_ERR( "Cannot start the %s",MODULE_NAME);
		exit(EXIT_FAILURE);
	}
#endif

	if(!RestServer::init(dbm,cli_auth,boot_graphs,core_mask,physical_ports,s_un_address,orchestrator_in_band,un_interface,ipsec_certificate, name_resolver_ip, name_resolver_port))
	{
		ULOG_ERR( "Cannot start the %s",MODULE_NAME);
		exit(EXIT_FAILURE);
	}

#ifdef ENABLE_RESOURCE_MANAGER
	ResourceManager::publishDescriptionFromFile(descr_file_name);
#endif

	http_daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, rest_port, NULL, NULL,&RestServer::answer_to_connection,
		NULL, MHD_OPTION_NOTIFY_COMPLETED, &RestServer::request_completed, NULL,MHD_OPTION_END);

	if (NULL == http_daemon)
	{
		ULOG_ERR( "Cannot start the HTTP deamon. The %s cannot be run.",MODULE_NAME);
		ULOG_ERR( "Please, check that the TCP port %d is not used (use the command \"netstat -a | grep %d\")",rest_port,rest_port);

		terminateRestServer();

		return EXIT_FAILURE;
	}

	// Ignore all signals but SIGSEGV and SIGINT
	sigset_t mask;
	sigfillset(&mask);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	sigset_t unblock;
	sigaddset(&unblock,SIGINT);
#ifdef __x86_64__
	sigaddset(&unblock,SIGSEGV);
#endif
	sigprocmask(SIG_UNBLOCK,&unblock,&mask);

	/* Install signal handlers */
	struct sigaction sa;

	sa.sa_sigaction = &signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;

#ifdef __x86_64__
	sigaction(SIGSEGV, &sa, NULL);
#endif
	sigaction(SIGINT, &sa, NULL);

	printUniversalNodeInfo();
	ULOG_INFO( "The '%s' is started!",MODULE_NAME);
	ULOG_INFO( "Waiting for commands on TCP port \"%d\"",rest_port);
	rofl::cioloop::get_loop().run();

	return 0;
}

bool parse_command_line(int argc, char *argv[],int *core_mask,char **config_file_name)
{
	int opt;
	char **argvopt;
	int option_index;

static struct option lgopts[] = {
		{"c", 1, 0, 0},
		{"d", 1, 0, 0},
		{"h", 0, 0, 0},
		{NULL, 0, 0, 0}
	};

	argvopt = argv;
	uint32_t arg_c = 0;

	*core_mask = CORE_MASK;

	while ((opt = getopt_long(argc, argvopt, "", lgopts, &option_index)) != EOF)
	{
		switch (opt)
		{
			/* long options */
			case 0:
				if (!strcmp(lgopts[option_index].name, "c"))/* core mask for network functions */
				{
					if(arg_c > 0)
					{
						ULOG_ERR( "Argument \"--c\" can appear only once in the command line");
						return usage();
					}
					char *port = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
					strcpy(port,optarg);

					sscanf(port,"%x",&(*core_mask));

					arg_c++;
				}
				else if (!strcmp(lgopts[option_index].name, "d"))/* inserting configuration file */
				{
					if(arg_c > 0)
					{
						ULOG_ERR( "Argument \"--d\" can appear only once in the command line");
						return usage();
					}

					strcpy(*config_file_name,optarg);

					arg_c++;
				}
				else if (!strcmp(lgopts[option_index].name, "h"))/* help */
				{
					return usage();
				}
				else
				{
					ULOG_ERR( "Invalid command line parameter '%s'\n",lgopts[option_index].name);
					return usage();
				}
				break;
			default:
				return usage();
		}
	}

	return true;
}

bool parse_config_file(char *config_file_name, int *rest_port, bool *cli_auth, map<string,string> &boot_graphs, set<string> &physical_ports, char **descr_file_name, char **client_name, char **broker_address, char **key_path, bool *orchestrator_in_band, char **un_interface, char **un_address, char **ipsec_certificate, string &name_resolver_ip, int *name_resolver_port)
{
	*rest_port = REST_PORT;

	/*
	*
	* Parsing universal node configuration file. Checks about mandatory parameters are done in this functions.
	*
	*/
	INIReader reader(config_file_name);

	if (reader.ParseError() < 0) {
		ULOG_ERR( "Can't load a default-config.ini file");
		return false;
	}

	// ports_name : optional
	char tmp_physical_ports[PATH_MAX];
	strcpy(tmp_physical_ports, (char *)reader.Get("physical ports", "ports_name", "UNKNOWN").c_str());
	if(strcmp(tmp_physical_ports, "UNKNOWN") != 0 && strcmp(tmp_physical_ports, "") != 0)
	{
		ULOG_DBG( "Physical ports read from configuation file: %s",tmp_physical_ports);
		//the string must start and terminate respectively with [ and ]
		if((tmp_physical_ports[strlen(tmp_physical_ports)-1] != ']') || (tmp_physical_ports[0] != '[') )
		{
			ULOG_ERR( "Wrong list of physical ports '%s'. It must be enclosed in '[...]'",tmp_physical_ports);
			return false;
		}
		tmp_physical_ports[strlen(tmp_physical_ports)-1] = '\0';

		//the string just read must be tokenized
		char delimiter[] = " ";
		char * pnt;
		pnt=strtok(tmp_physical_ports + 1, delimiter);
		while(pnt!= NULL)
		{
			ULOG_DBG( "\tphysical port: %s",pnt);
			string s(pnt);
			physical_ports.insert(pnt);
			pnt = strtok( NULL, delimiter );
		}
	}

	// nf-fgs : optional
	string nffgs = reader.Get("initial graphs", "nffgs", "UNKNOWN");
	if(nffgs != "UNKNOWN" && nffgs != "")
	{
		ULOG_DBG( "Initial graphs read from configuation file: %s",nffgs.c_str());
		//the string must start and terminate respectively with [ and ]
		if(nffgs.at(0)!='[' || nffgs.at(nffgs.length()-1)!=']')
		{
			ULOG_ERR( "Wrong list initial graphs '%s'. They must be enclosed in '[...]'",nffgs.c_str());
			return false;
		}
		nffgs=nffgs.substr(1,nffgs.length()-2);

		//the string just read must be tokenized
		istringstream iss(nffgs);
		string graph;
		while (getline(iss, graph, ' '))
		{
			istringstream iss(graph);
			string graphName,graphFile;
			getline(iss, graphName, '=');
			getline(iss, graphFile, '=');
			ULOG_DBG_INFO( "Boot Graph: '%s' - '%s'",graphName.c_str(),graphFile.c_str());
			boot_graphs[graphName]=graphFile;
		}
	}

	// server_port : mandatory
	int temp_rest_port = (int)reader.GetInteger("rest server", "server_port", -1);

	if(temp_rest_port != -1)
		*rest_port = temp_rest_port;
	else
	{
		ULOG_ERR( "Error in configuration file '%'s. Mandatory parameter 'server_port' is missing.",config_file_name);
		return false;
	}

	// user_authentication : optional - false if not specified
	*cli_auth = reader.GetBoolean("user authentication", "user_authentication", false);

	/* description file to export*/
	char *temp_descr = new char[64];
	strcpy(temp_descr, (char *)reader.Get("resource-manager", "description_file", "UNKNOWN").c_str());
	*descr_file_name = temp_descr;
#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
	// client_name : mandatory
	char *temp_cli = new char[64];
	strcpy(temp_cli, (char *)reader.Get("double-decker", "client_name", "UNKNOWN").c_str());
	*client_name = temp_cli;

	// brocker_address : mandatory
	char *temp_dealer = new char[64];
	strcpy(temp_dealer, (char *)reader.Get("double-decker", "broker_address", "UNKNOWN").c_str());
	*broker_address = temp_dealer;

	// key_path : mandatory
	char *temp_key = new char[64];
	strcpy(temp_key, (char *)reader.Get("double-decker", "key_path", "UNKNOWN").c_str());
	*key_path = temp_key;

	if(strcmp(temp_cli, "UNKNOWN") == 0)
	{
		ULOG_ERR( "Error in configuration file '%'s. Mandatory parameter 'client_name' is missing.",config_file_name);
		return false;
	}

	if(strcmp(temp_dealer, "UNKNOWN") == 0)
	{
		ULOG_ERR( "Error in configuration file '%'s. Mandatory parameter 'brocker_address' is missing.",config_file_name);
		return false;
	}

	if(strcmp(temp_key, "UNKNOWN") == 0)
	{
		ULOG_ERR( "Error in configuration file '%'s. Mandatory parameter 'key_path	' is missing.",config_file_name);
		return false;
	}
#endif

	// is_in_bande : optional - false if not specified
	*orchestrator_in_band = reader.GetBoolean("orchestrator", "is_in_band", false);

	/* universal node interface */
	char *temp_ctrl_iface = new char[64];
	strcpy(temp_ctrl_iface, (char *)reader.Get("orchestrator", "un_interface", "UNKNOWN").c_str());
	*un_interface = temp_ctrl_iface;

	/* local ip */
	char *temp_un_address = new char[64];
	strcpy(temp_un_address, (char *)reader.Get("orchestrator", "un_address", "UNKNOWN").c_str());
	*un_address = temp_un_address;

	/* IPsec certificate */
	char *temp_ipsec_certificate = new char[64];
	strcpy(temp_ipsec_certificate, (char *)reader.Get("GRE over IPsec", "certificate", "UNKNOWN").c_str());
	*ipsec_certificate = temp_ipsec_certificate;

	name_resolver_ip = reader.Get("Name resolver", "ip_address", "localhost");

	*name_resolver_port = (int) reader.GetInteger("Name resolver", "port", 2626);

	/* Path of the script file*/
	char script_path[64];
	strcpy(script_path, (char *)reader.Get("misc", "script_path", "./").c_str());
	setenv("un_script_path", script_path, 1);

	return true;
}

bool usage(void)
{
	stringstream message;

	message << "Usage:                                                                        \n" \
	"  sudo ./name-orchestrator --d configuration_file [options]     						  \n" \
	"                                                                                         \n" \
	"Parameters:                                                                              \n" \
	"  --d configuration_file                                                                 \n" \
	"        File that specifies some parameters such as rest port, physical port file,       \n" \
	"        NF-FG to deploy at the boot, and if client authentication is required            \n" \
	"                                                                                         \n" \
	"Options:                                                                                 \n" \
	"  --c core_mask                                                                           \n" \
	"        Mask that specifies which cores must be used for DPDK network functions. These   \n" \
	"        cores will be allocated to the DPDK network functions in a round robin fashion   \n" \
	"        (default is 0x2)                                                                 \n" \
	"  --h                                                                                    \n" \
	"        Print this help.                                                                 \n" \
	"                                                                                         \n" \
	"Example:                                                                                 \n" \
	"  sudo ./node-orchestrator --d config/default-config.ini	  							  \n";

	ULOG_INFO( "\n\n%s",message.str().c_str());

	return false;
}

/**
*	Prints information about the vSwitch configured and the execution environments supported
*/
void printUniversalNodeInfo()
{

ULOG_INFO( "************************************");

#ifdef __x86_64__
	ULOG_INFO( "The %s is executed on an x86_64 machine",MODULE_NAME);
#endif

#ifdef VSWITCH_IMPLEMENTATION_XDPD
	string vswitch = "xDPd";
#endif
#ifdef VSWITCH_IMPLEMENTATION_OVSDB
	stringstream ssvswitch;
	ssvswitch << "OvS with OVSDB protocol";
#ifdef ENABLE_OVSDB_DPDK
	ssvswitch << " (DPDK support enabled)";
#endif
	string vswitch = ssvswitch.str();
#endif
#ifdef VSWITCH_IMPLEMENTATION_ERFS
	string vswitch = "ERFS";
#endif
	ULOG_INFO( "* Virtual switch used: '%s'", vswitch.c_str());

	list<string> executionenvironment;
#ifdef ENABLE_KVM
	executionenvironment.push_back("virtual machines");
#endif
#ifdef ENABLE_DOCKER
	executionenvironment.push_back("Docker containers");
#endif
#ifdef ENABLE_DPDK_PROCESSES
	executionenvironment.push_back("DPDK processes");
#endif
#ifdef ENABLE_NATIVE
	executionenvironment.push_back("native functions");
#endif
	ULOG_INFO( "* Execution environments supported:");
	for(list<string>::iterator ee = executionenvironment.begin(); ee != executionenvironment.end(); ee++)
		ULOG_INFO( "* \t'%s'",ee->c_str());

#ifdef ENABLE_DOUBLE_DECKER_CONNECTION
	ULOG_INFO( "* Double Decker connection is enabled");
#endif

ULOG_INFO( "************************************");
}

void terminateRestServer() {
	try {
		RestServer::terminate();
	} catch(...) {
		//Do nothing, since the program is terminating
	}
}
