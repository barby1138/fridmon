// g_client.h
//
//////////////////////////////////////////////////////////////////////

#include "g_interface.h"
#include "g_messages.h"

class g_client : public IModuleMonitor
{
public:
	virtual int getModuleVersion(/*[out]*/char*& version) const {}

private:
//	xml_agent _xml_agent;
};

class g_clientEvent
{
public:
	g_clientEvent(IModuleMonitorEvent& clientEvent)
		:	_clientEvent(clientEvent)
	{}

//	void process(const xml_onModuleEventNotification_message& msg) {}

private:
	IModuleMonitorEvent& _clientEvent;
//	xml_agent _xml_agent;
};


