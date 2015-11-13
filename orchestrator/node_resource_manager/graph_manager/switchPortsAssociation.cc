#include "switchPortsAssociation.h"

pthread_mutex_t SwitchPortsAssociation::association_mutex = PTHREAD_MUTEX_INITIALIZER;

map<string, pair <string, string> > SwitchPortsAssociation::associationportgraphnf;

void SwitchPortsAssociation::setAssociation(string graphID, string port, string nf_name)
{
	pthread_mutex_lock(&association_mutex);
	
	pair <string,string> thePair = make_pair(graphID,nf_name);
	associationportgraphnf[port] = thePair;
	
	pthread_mutex_unlock(&association_mutex);
}

string SwitchPortsAssociation::getGraphID(string port)
{
	assert(associationportgraphnf.count(port) == 1);

	return associationportgraphnf[port].first;
}

string SwitchPortsAssociation::getNfName(string port)
{
	assert(associationportgraphnf.count(port) == 1);

	return associationportgraphnf[port].second;
}
