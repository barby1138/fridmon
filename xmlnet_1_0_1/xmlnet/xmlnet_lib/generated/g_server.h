// g_server.h
//
//////////////////////////////////////////////////////////////////////

#include "g_interface.h"
#include "g_messages.h"

class g_serverEvent : public IModuleMonitorEvent
{
public:
	virtual void onModuleEvent(/*in*/const char* message) const {}

private:
//	xml_agent _xml_agent;
};

class g_server
{
public:
	g_server(IModuleMonitor& server)
		:	_server(server)
	{}

	void process(const xml_getModuleVersionRequest_message& msg) 
	{
//		char buff[256];
//		_server.getModuleVersion(&buff[0])
	}

private:
	IModuleMonitor& _server;
//	xml_agent _xml_agent;
};

