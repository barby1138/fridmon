/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "stdafx.h"

#include "socket.h"

#ifdef WIN32
  #include <winsock.h>         // For socket(), connect(), send(), and recv()
  typedef int socklen_t;
  typedef char raw_type;       // Type used for raw data on this platform
#else
  #include <sys/types.h>       // For data types
  #include <sys/socket.h>      // For socket(), connect(), send(), and recv()
  #include <netdb.h>           // For gethostbyname()
  #include <arpa/inet.h>       // For inet_addr()
  #include <unistd.h>          // For close()
  #include <netinet/in.h>      // For sockaddr_in
  typedef void raw_type;       // Type used for raw data on this platform
#endif

#include <errno.h>             // For errno

using namespace net;

#ifdef WIN32
static bool initialized = false;
#endif

// socket_exception code

socket_exception::socket_exception(const pstring& message, bool inclSysMsg) throw() 
:	userMessage(message) 
{
  if (inclSysMsg) 
  {
    userMessage.append(": ");
    userMessage.append(strerror(errno));
  }
}

socket_exception::~socket_exception() throw() 
{
}

const char* socket_exception::what() const throw() 
{
  return userMessage.c_str();
}

// Function to fill in address structure given an address and port
static void fillAddr(const pstring& address, unsigned short port, sockaddr_in& addr)
{
  memset(&addr, 0, sizeof(addr));  // Zero out address structure
  addr.sin_family = AF_INET;       // Internet address

  hostent* host;  // Resolve name
  unsigned long host_bin = inet_addr(address.c_str());
  if (host_bin == INADDR_NONE)
  {
	  if ((host = gethostbyname(address.c_str())) == NULL) 
	  {
		  // strerror() will not work for gethostbyname() and hstrerror() 
		  // is supposedly obsolete
		  throw socket_exception("Failed to resolve name (gethostbyname())");
	  }
	  addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
  }
  else
  {
	  addr.sin_addr.s_addr = host_bin;
  }
  addr.sin_port = htons(port);     // Assign port in network byte order
}

// base_socket code

base_socket::base_socket(int type, int protocol) throw(socket_exception) 
:	_sockDesc(-1)
{
#ifdef WIN32
    if (!initialized) 
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		
		wVersionRequested = MAKEWORD(2, 0);              // Request WinSock v2.0
		if (WSAStartup(wVersionRequested, &wsaData) != 0) 
		{  // Load WinSock DLL
			throw socket_exception("Unable to load WinSock DLL");
		}
		initialized = true;
    }
#endif
	// Make a new socket
	if ((_sockDesc = ::socket(PF_INET, type, protocol)) < 0) 
	{
		throw socket_exception("Socket creation failed (socket())", true);
	}
}

base_socket::base_socket(int sockDesc) 
{
  _sockDesc = sockDesc;
}

void base_socket::close() 
{
#ifdef WIN32
    ::closesocket(_sockDesc);
#else
    ::close(_sockDesc);
#endif
	
	_sockDesc = -1;
}


base_socket::~base_socket() 
{
	close();
}

pstring base_socket::getLocalAddress() throw(socket_exception) 
{
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);
	
	if (getsockname(_sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) 
	{
		throw socket_exception("Fetch of local address failed (getsockname())", true);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short base_socket::getLocalPort() throw(socket_exception) 
{
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);
	
	if (getsockname(_sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) 
	{
		throw socket_exception("Fetch of local port failed (getsockname())", true);
	}
	return ntohs(addr.sin_port);
}

void base_socket::setLocalPort(unsigned short localPort) throw(socket_exception) 
{
	// Bind the socket to its port
	sockaddr_in localAddr;
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(localPort);
	
	if (bind(_sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) 
	{
		throw socket_exception("Set of local port failed (bind())", true);
	}
}

void base_socket::setLocalAddressAndPort(const pstring &localAddress, unsigned short localPort) throw(socket_exception) 
{
	// Get the address of the requested host
	sockaddr_in localAddr;
	fillAddr(localAddress, localPort, localAddr);
	
	if (bind(_sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) 
	{
		throw socket_exception("Set of local address and port failed (bind())", true);
	}
}

void base_socket::cleanUp() throw(socket_exception) 
{
#ifdef WIN32
    if (WSACleanup() != 0) 
	{
		throw socket_exception("WSACleanup() failed");
    }
#endif
}

unsigned short base_socket::resolveService(const pstring& service, const pstring& protocol)
{
	struct servent *serv;        /* Structure containing service information */
	
	if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
		return atoi(service.c_str());  /* Service is port number */
	else 
		return ntohs(serv->s_port);    /* Found port (network byte order) by name */
}

int base_socket::getDescr() const 
{ 
	return _sockDesc; 
}

// communicating_socket code

communicating_socket::communicating_socket(int type, int protocol) throw(socket_exception)
:	base_socket(type, protocol) 
{
}

communicating_socket::communicating_socket(int newConnSD) : base_socket(newConnSD) 
{
}

void communicating_socket::connect(const pstring &foreignAddress,
								   unsigned short foreignPort) throw(socket_exception) {
	// Get the address of the requested host
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);
	
	// Try to connect to the given port
	if (::connect(_sockDesc, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
		throw socket_exception("Connect failed (connect())", true);
	}
}

void communicating_socket::send(const void *buffer, int bufferLen) throw(socket_exception) 
{
	if (::send(_sockDesc, (raw_type *) buffer, bufferLen, 0) < 0) 
	{
		throw socket_exception("Send failed (send())", true);
	}
}

int communicating_socket::recv(void *buffer, int bufferLen) throw(socket_exception) 
{
	int rtn;
	if ((rtn = ::recv(_sockDesc, (raw_type *) buffer, bufferLen, 0)) < 0) 
	{
		throw socket_exception("Received failed (recv())", true);
	}
	
	return rtn;
}

pstring communicating_socket::getForeignAddress() throw(socket_exception) 
{
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);
	
	if (getpeername(_sockDesc, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) 
	{
		throw socket_exception("Fetch of foreign address failed (getpeername())", true);
	}
	return inet_ntoa(addr.sin_addr);
}

unsigned short communicating_socket::getForeignPort() throw(socket_exception)
{
	sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);
	
	if (getpeername(_sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) 
	{
		throw socket_exception("Fetch of foreign port failed (getpeername())", true);
	}
	return ntohs(addr.sin_port);
}

// TCP_socket code

TCP_socket::TCP_socket() throw(socket_exception)
:	communicating_socket(SOCK_STREAM, IPPROTO_TCP) 
{
}

TCP_socket::TCP_socket(const pstring &foreignAddress, unsigned short foreignPort) throw(socket_exception)
:	communicating_socket(SOCK_STREAM, IPPROTO_TCP) 
{
	connect(foreignAddress, foreignPort);
}

TCP_socket::TCP_socket(int newConnSD)
:	communicating_socket(newConnSD) 
{
}

// TCP_server_socket code

TCP_server_socket::TCP_server_socket() throw(socket_exception) 
	:	base_socket(SOCK_STREAM, IPPROTO_TCP) 
{
}

void TCP_server_socket::bind(unsigned short localPort) throw(socket_exception) 
{
	setLocalPort(localPort);
}

void TCP_server_socket::bind(const pstring& localAddress, unsigned short localPort) throw(socket_exception) 
{
	setLocalAddressAndPort(localAddress, localPort);
}

TCP_socket* TCP_server_socket::accept() throw(socket_exception) 
{
	int newConnSD;
	if ((newConnSD = ::accept(_sockDesc, NULL, 0)) < 0) 
	{
		throw socket_exception("Accept failed (accept())", true);
	}
	
	return new TCP_socket(newConnSD);
}

void TCP_server_socket::listen(int queueLen) throw(socket_exception) 
{
	if (::listen(_sockDesc, queueLen) < 0) 
	{
		throw socket_exception("Set listening socket failed (listen())", true);
	}
}

void TCP_server_socket::setReuseAddr() throw(socket_exception)
{
	int on = 1;
	setsockopt(_sockDesc, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on));
}

// UDP_socket code

UDP_socket::UDP_socket() throw(socket_exception) 
:	communicating_socket(SOCK_DGRAM, IPPROTO_UDP) 
{
	setBroadcast();
}

UDP_socket::UDP_socket(unsigned short localPort)  throw(socket_exception)
:	communicating_socket(SOCK_DGRAM, IPPROTO_UDP) 
{
	setLocalPort(localPort);
	setBroadcast();
}

UDP_socket::UDP_socket(const pstring& localAddress, unsigned short localPort) throw(socket_exception)
:	communicating_socket(SOCK_DGRAM, IPPROTO_UDP) 
{
	setLocalAddressAndPort(localAddress, localPort);
	setBroadcast();
}

void UDP_socket::setBroadcast() 
{
	// If this fails, we'll hear about it when we try to send.  This will allow 
	// system that cannot broadcast to continue if they don't plan to broadcast
	int broadcastPermission = 1;
	setsockopt(_sockDesc, SOL_SOCKET, SO_BROADCAST, (raw_type *) &broadcastPermission, sizeof(broadcastPermission));
}

void UDP_socket::disconnect() throw(socket_exception) 
{
	sockaddr_in nullAddr;
	memset(&nullAddr, 0, sizeof(nullAddr));
	nullAddr.sin_family = AF_UNSPEC;
	
	// Try to disconnect
	if (::connect(_sockDesc, (sockaddr *) &nullAddr, sizeof(nullAddr)) < 0) 
	{
#ifdef WIN32
		if (errno != WSAEAFNOSUPPORT) 
		{
#else
			if (errno != EAFNOSUPPORT) 
			{
#endif
				throw socket_exception("Disconnect failed (connect())", true);
		}
	}
}
	
void UDP_socket::sendTo(const void* buffer, int bufferLen, const pstring &foreignAddress, unsigned short foreignPort) throw(socket_exception) 
{
	sockaddr_in destAddr;
	fillAddr(foreignAddress, foreignPort, destAddr);
		
	// Write out the whole buffer as a single message.
	if (sendto(_sockDesc, (raw_type *) buffer, bufferLen, 0, (sockaddr *) &destAddr, sizeof(destAddr)) != bufferLen) 
	{
		throw socket_exception("Send failed (sendto())", true);
	}
}

int UDP_socket::recvFrom(void *buffer, int bufferLen, pstring &sourceAddress, unsigned short &sourcePort) throw(socket_exception)
{
	sockaddr_in clntAddr;
	socklen_t addrLen = sizeof(clntAddr);
	int rtn;
	if ((rtn = recvfrom(_sockDesc, (raw_type *) buffer, bufferLen, 0, (sockaddr *) &clntAddr, (socklen_t *) &addrLen)) < 0)   
	{
		throw socket_exception("Recieve failed (recvFrom())", true); 
	}
	sourceAddress = inet_ntoa(clntAddr.sin_addr);
	sourcePort = ntohs(clntAddr.sin_port);
	
	return rtn;
}

void UDP_socket::setMulticastTTL(unsigned char multicastTTL) throw(socket_exception) 
{
	if (setsockopt(_sockDesc, IPPROTO_IP, IP_MULTICAST_TTL, (raw_type *) &multicastTTL, sizeof(multicastTTL)) < 0) 
	{
		throw socket_exception("Multicast TTL set failed (setsockopt())", true);
	}
}

void UDP_socket::joinGroup(const pstring &multicastGroup) throw(socket_exception) 
{
	struct ip_mreq multicastRequest;
	
	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(_sockDesc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (raw_type *) &multicastRequest, sizeof(multicastRequest)) < 0) 
	{
		throw socket_exception("Multicast group join failed (setsockopt())", true);
	}
}

void UDP_socket::leaveGroup(const pstring &multicastGroup) throw(socket_exception) 
{
	struct ip_mreq multicastRequest;
	
	multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
	multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(_sockDesc, IPPROTO_IP, IP_DROP_MEMBERSHIP, (raw_type *) &multicastRequest, sizeof(multicastRequest)) < 0) 
	{
		throw socket_exception("Multicast group leave failed (setsockopt())", true);
	}
}
