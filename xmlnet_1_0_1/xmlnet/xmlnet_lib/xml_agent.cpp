// xml_agent.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "xml_agent.h"

#ifdef __LINUX__
#include <arpa/inet.h>
#include <sys/poll.h>
#endif

inline u32 generateConnectionUniqueId()
{
	// id=0 is reserved as invalid

	static u32 id = 1;

	id++;

	// high bit is reserved for channel forking
	if (id == U32_MAX) id = 1;

	return id; 
}

//
// bad_request_exception 
//
bad_request_exception::bad_request_exception(const xml_request_message& msg)
	:	_msg(msg)
{}

//
// error_response_exception
//
error_response_exception::error_response_exception(xml_response_message& msg)
:	_msg(msg)
{
	_msg.setReturnValue("InternalError");
}

//
// unknown_request_exception
//
unknown_request_exception::unknown_request_exception(const xml_request_message& msg)
:	bad_request_exception(msg)
{}

//
// bad_params_exception
//
bad_params_exception::bad_params_exception(const xml_request_message& msg)
:	bad_request_exception(msg)
{}

//
// connection_t
//
connection_t::connection_t(net::TCP_socket* sock) :
	_id(0),
	socket(sock)
{
}

connection_t::~connection_t()
{
	delete socket;
}

//
// connections_t
//
connections_t::connections_t()
{
}

connections_t::~connections_t()
{
	clear();
}

void connections_t::add(net::TCP_socket* pSocket)
{
	critical_scope<> guard(_cs);

	connection_t* newConnection = new connection_t(pSocket);
	newConnection->_id = generateConnectionUniqueId();
	
	if (false == _connections_map.insert(connections_map::value_type(newConnection->_id, newConnection)).second)
		throw pruntime_error("xml_agent::run insert to connections map failed");
}

void connections_t::remove(int descr)
{
	critical_scope<> guard(_cs);

	connection_t* pConnection = get(descr);

	delete pConnection;

	_connections_map.erase(descr);
}

void connections_t::clear()
{
	critical_scope<> guard(_cs);
	
	std::for_each(_connections_map.begin(), _connections_map.end(), cleaner_ptr());
	
	_connections_map.clear();
}

connection_t* connections_t::get(int descr)
{
	critical_scope<> guard(_cs);

	connections_map::const_iterator itor = _connections_map.find(descr);
	if (itor == _connections_map.end())
		throw pruntime_error("connections_t::get() - connection not found");

	return itor->second;
}

size_t connections_t::size()
{
	critical_scope<> guard(_cs);

	return _connections_map.size();
}

//
// xml_server
//
void xml_server::run()
{
	while (!isTerminating())
	{
		try
		{
			// accept connections
			net::TCP_socket* newSocket = _serverSocket.accept();
			_connections.add(newSocket);
		}
		catch (net::socket_exception& ex)
		{
			TRACE_ERROR("xml_server::run socket_exception %s", ex.what());
		}
		catch (std::exception& ex)
		{
			TRACE_ERROR("xml_server::run exception %s", ex.what());
		}
	}
}

xml_server::xml_server()
:	thread("xml_server"),
	_sender(_connections),
	_receiver(_connections)
{
}

xml_server::~xml_server()
{
}

bool xml_server::onInitialize()
{	
	_serverSocket.setReuseAddr();
	// TODO hardcode
	_serverSocket.bind(5000);
	_serverSocket.listen(MAX_CONNECTIONS);
	
	_sender.start();
	_receiver.start();

	return true;
}

void xml_server::onUninitialize()
{
	_receiver.stop();
	_sender.stop();

	_serverSocket.close();

	_connections.clear();
}

void xml_server::send(const xml_message& msg)
{
	_sender.enqueue(const_cast<xml_message*> (&msg));
}

class fdset_fill
{
public:
	fdset_fill(fd_set& fdSet, int& maxFd)
		:	_fdSet(fdSet),
		    _maxFd(maxFd)
	{
	}

	template<class _Key, class _Val>
		void operator() (const std::pair<_Key, _Val>& pr)
	{ 
		int sockDescr = pr.second->getSockDescr();
		FD_SET(sockDescr, &_fdSet);
		if (sockDescr > _maxFd)
			_maxFd = sockDescr;
	}

private:
	fd_set& _fdSet;
	int& _maxFd;
};

//
// xml_server_receiver
//
void xml_server_receiver::run()
{
#if defined (__LINUX__)
	struct pollfd fds[max_connections];
#elif defined (WIN32)
	fd_set fdSet;
	timeval tv;
#endif

	while (!isTerminating())
	{
		try
		{	
			if (_connections.size())
			{
#if defined (__LINUX__)
				int count = 0;
				{
					connections_t::ct_guard guard(_connections);

					const connections_t::connections_map& connections = _connections.getConnections();
					for (connections_t::connections_map::const_iterator itr = connections.begin(); itr != connections.end(); ++itr)
					{
						fds[count].fd = itr->second->getSockDescr();
						fds[count].events = POLLIN;
						fds[count].revents = 0;

						count++;
					}
				}

				int selRes = ::poll(fds, count, select_to);

				switch (selRes)
				{
					case -1:
						TRACE_WARNING("xml_server_receiver::run select error");
						thread::sleep(select_to);
						break;
					case 0:
						// no sockets selected
						break;
					default:
					{	
						connections_t::ct_guard guard(_connections);

						for (int i = 0; i < count; ++i)
						{
							if (fds[i].revents & POLLIN)
							{
								const connections_t::connections_map& connections = _connections.getConnections();
								connections_t::connections_map::const_iterator it = connections.begin();
								while (it != connections.end())
								{
									if((fds[i].fd == it->second->getSockDescr()) && !receive(*it->second))
									{
										TRACE_INFO("xml_server_receiver::run receive() failed - removing connection");
										connections_t::connections_map::const_iterator itDelete = it;
										++it;

										onConnectionFailure(itDelete->first);
										continue;
									}

									++it;
								}
							}
						}
					}
				}
#elif defined (WIN32)
				FD_ZERO(&fdSet);
				int first_conn = 0;

				{
					connections_t::ct_guard guard(_connections);
					const connections_t::connections_map& connections = _connections.getConnections();
					std::for_each(connections.begin(), connections.end(), fdset_fill(fdSet, first_conn));					
				}

				tv.tv_sec = SELECT_TO / 1000;
				tv.tv_usec = (SELECT_TO % 1000) * 1000;
				int selRes = ::select(first_conn + 1, &fdSet, 0, 0, &tv);

				switch (selRes)
				{
					case -1:
						TRACE_WARNING("xml_server_receiver::run select error");
						thread::sleep(SELECT_TO);
						break;
					case 0:
						// no sockets selected
						break;
					default:
					{	
						connections_t::ct_guard guard(_connections);

						const connections_t::connections_map& connections = _connections.getConnections();
						connections_t::connections_map::const_iterator it = connections.begin();
						while (it != connections.end())
						{
							if(FD_ISSET(it->second->getSockDescr(), &fdSet) && !receive(*it->second))
							{
								TRACE_INFO("xml_server_receiver::run receive() failed - removing connection");
								connections_t::connections_map::const_iterator itDelete = it;
								++it;

								onConnectionFailure(itDelete->first);
								continue;
							}

							++it;
						}
					}
				}
#endif
			}
			else
				thread::sleep(SELECT_TO);
		}
		catch (net::socket_exception& ex)
		{
			TRACE_ERROR("xml_server_receiver::run socket_exception %s", ex.what());
			thread::sleep(SELECT_TO);
		}
		catch (std::exception &ex)
		{
			TRACE_ERROR("xml_server_receiver::run exception %s", ex.what());
			thread::sleep(SELECT_TO);
		}
	}
}

/*
const pstring detectRequestType(const char* str, const char*& remainingStr)
{
	prop parser;
	parser.fromString(str);
	if (parser.getParserOffset())
		remainingStr = str + parser.getParserOffset();
	else
		remainingStr = 0;

	return parser._RequestRoot._sub_element_name;
}
*/
bool xml_server_receiver::receive(connection_t& conn)
{
	int readBytes;
	pstring queryType;
	const char* remainingStr = 0;

	pstring::size_type originalLength = conn.buffer.length();
	pstring::size_type startIndex = originalLength;

	conn.buffer.resize(originalLength + BUFF_SIZE);

	try
	{
		readBytes = conn.socket->recv(&conn.buffer.at(startIndex),
										static_cast<int>(conn.buffer.length() - originalLength));
	}
	catch (net::socket_exception& ex)
	{
		TRACE_WARNING("net::socket_exception %s", ex.what());

		return false;
	}
	
	conn.buffer.resize(originalLength + readBytes);

	while (conn.buffer.length())
	{
		try
		{
			// TODO get query type
			queryType = "";
			// TODO review spMsg
			pauto_ptr<xml_message> spMsg; //make_auto_ptr(xml_message_factory_sngl::instance().createObject(queryType));

			TRACE_INFO("xml_server_receiver::receive xml_message [conn_id=%d]:\n%s", conn._id, conn.buffer.c_str());

			spMsg->setConnectionId(conn._id);
			spMsg->fromString(conn.buffer.c_str());
			// TODO dispatch!!!
/*
			if (disp::dispatch(pstring(spMsg->getName()), *spMsg.get()))
			{
				spMsg.release();
			}
			else
			{
				TRACE_WARNING("xml_server_receiver::receive failed to dispatch message");
			}
*/
		}
		catch(bad_params_exception& ex)
		{
			TRACE_ERROR("xml_server_receiver::receive bad_params_exception %s", ex.what());

			onBadParamsRequest(ex.getRequest());
		}
		catch(expat_parse_error& ex)
		{
			TRACE_WARNING("xml_server_receiver::receive expat_parse_error %s (possibly fragmented message)", ex.what());

			return true;
		}
		catch(std::exception& ex)
		{
			TRACE_ERROR("xml_server_receiver::receive exception %s", ex.what());
			
			onUnknownMessage(conn, queryType);
		}
		
		if (remainingStr)
			conn.buffer = remainingStr;
		else
			conn.buffer.resize(0);
	}

	return readBytes > 0;
}

void xml_server_receiver::onBadParamsRequest(const xml_request_message& msg)
{
/*
	xml_failure_message* pMsg = new xml_failure_message();

	pMsg->setConnectionId(msg.getConnectionId());
	pMsg->setVersion(msg.getVersion());
	pMsg->setRequestId(msg.getRequestId());
	
	xml_server_agent_sngl::instance().send(*pMsg);
*/
}

void xml_server_receiver::onUnknownMessage(const connection_t& conn, const pstring& msgType)
{
	xml_unknown_request_response_message* pMsg = new xml_unknown_request_response_message();
	
	pMsg->setConnectionId(conn._id);
	pMsg->setRequestId(0);
	pMsg->setUnknownRequest(msgType);
	
	xml_server_sngl::instance().send(*pMsg);
}

void xml_server_receiver::onXmlParcerError(const connection_t& conn)
{
	xml_parser_error_response_message* pMsg = new xml_parser_error_response_message();

	pMsg->setConnectionId(conn._id);

	xml_server_sngl::instance().send(*pMsg);
}

void xml_server_receiver::onConnectionFailure(const int id)
{	
	_connections.remove(id);
}

//
// xml_server_sender
//
void xml_server_sender::onMessage(xml_message& msg)
{
	pstring str(msg.toString());

	TRACE_INFO("xml_server_sender::onMessage xml_message [conn_id=%d]:\n%s", msg.getConnectionId(), str.c_str());

	connection_t* connection;
	u32 connectionId, connectionDescr;

	{
		connections_t::ct_guard guard(_connections);

		connectionId = msg.getConnectionId();
		connection = _connections.get(connectionId);
		if (0 == connection)
		{
			TRACE_WARNING("xml_server_sender::onMessage connection is closed - discarding message");
			return;
		}

		connectionDescr = connection->getSockDescr();
	}

#if defined(__LINUX__)	
	
	pollfd pollSocket;
	pollSocket.fd = connectionDescr;
	pollSocket.events = POLLOUT;
	pollSocket.revents = 0;

	int pollRes = ::poll(&pollSocket, (unsigned int)1, select_to);

	if ((1 == pollRes) && (pollSocket.events & POLLOUT))
#elif defined(WIN32)

	timeval tv;
	tv.tv_sec = select_to / 1000;
	tv.tv_usec = (select_to % 1000) * 1000;

	fd_set fdSet;
	FD_ZERO(&fdSet);
	FD_SET(connectionDescr, &fdSet);
	
	int selRes = ::select(connectionDescr + 1, 0, &fdSet, 0, &tv);

	if ((1 == selRes) && (FD_ISSET(connectionDescr, &fdSet)))
#endif
	{
		// now lock connections and re-check if the connection is still alive
		connections_t::ct_guard guard(_connections);
		connection = _connections.get(connectionId);
		if (0 == connection)
		{
			TRACE_WARNING("xml_server_sender::onMessage error - connection is closed - discarding message");
			return;
		}

		connection->socket->send(str.c_str(), static_cast<int>(str.size()));
	}
	else
	{
		TRACE_WARNING("xml_server_sender::onMessage select error - removing connection");

		onConnectionFailure(connectionId);
	}
}

void xml_server_sender::onConnectionFailure(const int id)
{	
	_connections.remove(id);
}

void xml_server_sender::enqueue(xml_message* pMsg)
{
	_queue.push(pMsg);
}

void xml_server_sender::terminate()
{
	_queue.push(NULL);
}

void xml_server_sender::run()
{
	while (!isTerminating())
	{
		try
		{
			pauto_ptr<xml_message> spMsg(_queue.get());
			if (NULL != spMsg.get())
				onMessage(*spMsg);
			else	// exit
				break;
		}
		catch (net::socket_exception& ex)
		{
			TRACE_ERROR("xml_server_sender::run socket_exception %s", ex.what());
		}
		catch (std::exception& ex)
		{
			TRACE_ERROR("xml_server_sender::run:exception: %s", ex.what());
		}
	}
}
