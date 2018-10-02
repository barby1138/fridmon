// g_interface.h
//
//////////////////////////////////////////////////////////////////////

struct IModuleMonitor
{
	virtual int getModuleVersion(/*[out]*/char*& version) const = 0;
};

struct IModuleMonitorEvent
{
	virtual void onModuleEvent(/*in*/const char* message) const = 0;
};