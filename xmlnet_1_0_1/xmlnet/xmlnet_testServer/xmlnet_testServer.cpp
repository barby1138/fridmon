// xmlnet_testServer.cpp
//

#include "stdafx.h"

struct IModuleMonitor
{
	virtual const char* getModuleVersion() const = 0;
};

class ModuleMonitor :	public IModuleMonitor
{
	virtual const char* getModuleVersion() const { return "1.0"; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

