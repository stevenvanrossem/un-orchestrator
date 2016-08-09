#include "nf.h"

NF::NF(string name) :
	name(name), selectedDescription(NULL), isRunning(false)
{

}

void NF::addDescription(Description *description)
{
	descriptions.push_back(description);
}

list<Description*> NF::getAvailableDescriptions()
{
	return descriptions;
}

void NF::setSelectedDescription(NFsManager *impl)
{
	selectedDescription = impl;
}

NFsManager *NF::getSelectedDescription()
{
	return selectedDescription;
}

bool NF::getRunning()
{
	return isRunning;
}

void NF::setRunning(bool val)
{
	isRunning = val;
}

string NF::getName()
{
	return name;
}
