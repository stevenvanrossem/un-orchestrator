#ifndef INTERNAL_H_
#define INTERNAL_H_ 1

#include "../../nfs_manager.h"


class Internal : public NFsManager
{
public:
	bool isSupported(Description& descr){return true;};
	bool startNF(StartNFIn sni){return true;};
	bool stopNF(StopNFIn sni){return true;};
};

#endif //INTERNAL_H_
