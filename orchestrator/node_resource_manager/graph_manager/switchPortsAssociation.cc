#include "switchPortsAssociation.h"

pthread_mutex_t SwitchPortsAssociation::association_mutex = PTHREAD_MUTEX_INITIALIZER;

map<string, pair <string, string> > SwitchPortsAssociation::associationportgraphnf;

void SwitchPortsAssociation::setAssociation(string graphID, string port, string nf_name)
{

}

string SwitchPortsAssociation::getGraphID(string port)
{

}

string SwitchPortsAssociation::getNfName(string port)
{

}
