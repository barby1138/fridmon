// xml_agent.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _XML_AGENT_H_
#define _XML_AGENT_H_

#include "socket.h"
#include "messages.h"
#include "xml_schemes.h"

enum
{
	NOTIFICATION_TO = 5000,
	MAX_CONNECTIONS = 10
};

class xml_agent;

struct connection_t
{
	connection_t(net::TCP_socket* sock);
	~connection_t();
	
	int getSockDescr() const { return socket->getDescr(); }

	u32 _id;
	net::TCP_socket* socket;
	pstring	buffer;
};

class connections_t
{
public:
	typedef pmap<s32, connection_t*> connections_map;

public:
	connections_t();
	~connections_t();

	void add(net::TCP_socket* pSocket);

	connection_t* get(int descr);

	void remove(int descr);
	void clear();
	
	size_t size();

	const connections_map& getConnections() const { return _connections_map; }

	class ct_guard;

	friend class connections_t::ct_guard;

	class ct_guard : public critical_scope<>
	{
	public:
		ct_guard(connections_t& c) 
			: critical_scope<>(c._cs){}
	};

private:
	connections_map _connections_map;
	critical_section _cs;
};

class xml_server_sender : public thread
{
private:
	typedef msg_queue<xml_message*> queue_t;
	enum { select_to = 20000 }; // 20 seconds

public:
	xml_server_sender(connections_t& connections)
		:	_connections(connections)
	{}

	void enqueue(xml_message* pMsg);
	void terminate();
	virtual void run();

private:
	void onMessage(xml_message& msg);
	void onConnectionFailure(const int descr);

private:
	queue_t _queue;
	connections_t& _connections;
};

class xml_server_receiver : public thread//,
					 //public xml_message_dispatcher
{
private:
	enum { SELECT_TO = 1000 };
	enum { BUFF_SIZE = 2048 };

public:
	xml_server_receiver(connections_t& connections)
		:	_connections(connections)
	{}

	virtual void run();

private:
	bool receive(connection_t& conn);

private:
	void onBadParamsRequest(const xml_request_message& msg);
	void onUnknownMessage(const connection_t& conn, const pstring& msgType);
	void onXmlParcerError(const connection_t& conn);
	void onConnectionFailure(const int descr);

private:
	connections_t& _connections;
};

class xml_server : public thread
{	
public:
	xml_server();
	~xml_server();
	
	virtual void run();
	virtual bool onInitialize();
	virtual void onUninitialize();

	void send(const xml_message& msg);

private:
	net::TCP_server_socket _serverSocket;
	connections_t _connections;

	xml_server_sender _sender;
	xml_server_receiver _receiver;

	critical_section _cs;
};

typedef singleton_holder<xml_server, create_using_new> xml_server_sngl;

/////////////////////////////////////////////////////////////////////////////////
// exceptions
/////////////////////////////////////////////////////////////////////////////////

class bad_request_exception : public std::exception
{
public:
	bad_request_exception(const xml_request_message& msg);

	const xml_request_message& getRequest() const { return _msg; }

private:
	const xml_request_message& _msg;
};

class error_response_exception : public std::exception
{
public:
	explicit error_response_exception(xml_response_message& msg);

	xml_response_message& getResponse() const { return _msg; }

private:
	xml_response_message& _msg;
};

class unknown_request_exception : public bad_request_exception
{
public:
	unknown_request_exception(const xml_request_message& msg);
};

class bad_params_exception : public bad_request_exception
{
public:
	bad_params_exception(const xml_request_message& msg);
};

#endif // _XML_AGENT_H_

/*
class notification_subscription_registrar
{
private:
struct notification_subscription_info
{
u32 _connection_id;
u32 _cookie;
};

typedef plist<notification_subscription_info> notification_subscription_t;
typedef pmap<pstring, notification_subscription_t*> notification_subscriptions_t;

public:
void processSubscriptionRequest(xml_reg_unreg_notification_request_message& msg);

private:
bool addSubscription(xml_reg_unreg_notification_request_message& msg);
bool removeSubscription(xml_reg_unreg_notification_request_message& msg);
bool removeSubscription(const pstring& subsType, u32 cookie, u32 connId);

private:
notification_subscriptions_t _subscriptions;
};

void notification_subscription_registrar::processSubscriptionRequest(xml_reg_unreg_notification_request_message& msg)
{
	critical_scope<> guard(_cs);
	
	if (msg.getRegister())
	{
		if (!addSubscription(msg))
			msg.setProcessingResult("Error");
	}
	else
	{
		if (!removeSubscription(msg))
			msg.setProcessingResult("Error");
	}
}

bool notification_subscription_registrar::addSubscription(xml_reg_unreg_notification_request_message& msg)
{
	removeSubscription(msg); // in case such subscription already exists

	notification_subscription_info nsi;
	nsi._cookie = msg.getCookie();
	nsi._connection_id = msg.getConnectionId();

	if (_subscriptions[msg.getNotificationType()] == 0)
	{
		_subscriptions.erase(msg.getNotificationType());
		return false;
	}
	
	_subscriptions[msg.getNotificationType()]->push_back(nsi);

	TRACE_INFO("[conn_id=%d, cookie=%d]::addSubscription: notif_type=%s", msg.getConnectionId(), msg.getCookie(), msg.getNotificationType().c_str());

	return true;
}

bool notification_subscription_registrar::removeSubscription(xml_reg_unreg_notification_request_message& msg)
{
	return removeSubscription(msg.getNotificationType(), msg.getCookie(), msg.getConnectionId());
}

bool notification_subscription_registrar::removeSubscription(const pstring& subsType, u32 cookie, u32 connId)
{	
	notification_subscription_t::iterator iter;
	notification_subscription_t* subscription_list = _subscriptions[subsType];
	if (0 == subscription_list)
	{
		_subscriptions.erase(subsType);
		return false;
	}
	if (subscription_list->empty())
		return false;

	for (iter = subscription_list->begin(); iter != subscription_list->end(); iter++)
	{
		if ((iter->_cookie == cookie) && (iter->_connection_id == connId))
		{
			subscription_list->erase(iter);
			break;
		}
	}

	TRACE_INFO("[conn_id=%d, cookie=%d]::removeSubscription: notif_type=%s", connId, cookie, subsType.c_str());

	return true;
}
*/