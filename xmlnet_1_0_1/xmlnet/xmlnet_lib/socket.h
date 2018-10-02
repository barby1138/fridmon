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

#ifndef _NET_SOCKET_H_
#define _NET_SOCKET_H_

#ifdef WIN32
#include <winsock.h>         // For struct fd_set, timeval
#else
#include <sys/types.h>       // For struct fd_set, timeval
#endif

// C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable:4290 )

namespace net
{

class socket_exception : public std::exception
{
public:
	/**
	*   Construct a socket_exception with a explanatory message.
	*   @param message explanatory message
	*   @param incSysMsg true if system message (from strerror(errno))
	*   should be postfixed to the user provided message
	*/
	socket_exception(const pstring &message, bool inclSysMsg = false) throw();
	
	/**
	*   Provided just to guarantee that no exceptions are thrown.
	*/
	~socket_exception() throw();
	
	/**
	*   Get the exception message
	*   @return exception message
	*/
	const char *what() const throw();
	
private:
	pstring userMessage;  // Exception message
};

/**
*   Socket super class
*/
class base_socket 
{
public:
/**
*   Closes socket
	*/   
	void close();
	/**
	*   Close and deallocate this socket
	*/
	~base_socket();
	
	/**
	*   Get the local address
	*   @return local address of socket
	*   @exception socket_exception thrown if fetch fails
	*/
	pstring getLocalAddress() throw(socket_exception);
	
	/**
	*   Get the local port
	*   @return local port of socket
	*   @exception socket_exception thrown if fetch fails
	*/
	unsigned short getLocalPort() throw(socket_exception);
	
	/**
	*   Set the local port to the specified port and the local address
	*   to any interface
	*   @param localPort local port
	*   @exception socket_exception thrown if setting local port fails
	*/
	void setLocalPort(unsigned short localPort) throw(socket_exception);
	
	/**
	*   Set the local port to the specified port and the local address
	*   to the specified address.  If you omit the port, a random port 
	*   will be selected.
	*   @param localAddress local address
	*   @param localPort local port
	*   @exception socket_exception thrown if setting local port or address fails
	*/
	void setLocalAddressAndPort(const pstring& localAddress, unsigned short localPort = 0) throw(socket_exception);
	
	/**
	*   If WinSock, unload the WinSock DLLs; otherwise do nothing.  We ignore
	*   this in our sample client code but include it in the library for
	*   completeness.  If you are running on Windows and you are concerned
	*   about DLL resource consumption, call this after you are done with all
	*   Socket instances.  If you execute this on Windows while some instance of
	*   Socket exists, you are toast.  For portability of client code, this is 
	*   an empty function on non-Windows platforms so you can always include it.
	*   @param buffer buffer to receive the data
	*   @param bufferLen maximum number of bytes to read into buffer
	*   @return number of bytes read, 0 for EOF, and -1 for error
	*   @exception socket_exception thrown WinSock clean up fails
	*/
	static void cleanUp() throw(socket_exception);
	
	/**
	*   Resolve the specified service for the specified protocol to the
	*   corresponding port number in host byte order
	*   @param service service to resolve (e.g., "http")
	*   @param protocol protocol of service to resolve.  Default is "tcp".
	*/
	static unsigned short resolveService(const pstring& service, const pstring& protocol = "tcp");
	
	int getDescr() const;

private:
	// Prevent the user from trying to use value semantics on this object
	base_socket(const base_socket& sock);
	void operator=(const base_socket& sock);
	
protected:
	int _sockDesc;              // Socket descriptor
	base_socket(int type, int protocol) throw(socket_exception);
	base_socket(int sockDesc);
};

/**
*   Socket which is able to connect, send, and receive
*/
class communicating_socket : public base_socket 
{
public:
	/**
	*   Attempt to establish a socket connection with the given foreign
	*   address and port
	*   @param foreignAddress foreign address (IP address or name)
	*   @param foreignPort foreign port
	*   @return true if connection successfully established
	*   @exception socket_exception thrown if unable to establish connection
	*/
	void connect(const pstring& foreignAddress, unsigned short foreignPort) throw(socket_exception);
	
	/**
	*   Write the given buffer to this socket.  Call connect() before
	*   calling send()
	*   @param buffer buffer to be written
	*   @param bufferLen number of bytes from buffer to be written
	*   @return true if the send is successful
	*   @exception socket_exception thrown if unable to send data
	*/
	void send(const void* buffer, int bufferLen) throw(socket_exception);
	
	/**
	*   Read into the given buffer up to bufferLen bytes data from this
	*   socket.  Call connect() before calling recv()
	*   @param buffer buffer to receive the data
	*   @param bufferLen maximum number of bytes to read into buffer
	*   @return number of bytes read, 0 for EOF, and -1 for error
	*   @exception socket_exception thrown if unable to receive data
	*/
	int recv(void* buffer, int bufferLen) throw(socket_exception);
	
	/**
	*   Get the foreign address.  Call connect() before calling recv()
	*   @return foreign address
	*   @exception socket_exception thrown if unable to fetch foreign address
	*/
	pstring getForeignAddress() throw(socket_exception);
	
	/**
	*   Get the foreign port.  Call connect() before calling recv()
	*   @return foreign port
	*   @exception socket_exception thrown if unable to fetch foreign port
	*/
	unsigned short getForeignPort() throw(socket_exception);
	
protected:
	communicating_socket(int type, int protocol) throw(socket_exception);
	communicating_socket(int newConnSD);

private:
	friend class communicating_socket_group;
};

/**
*   TCP socket for communication with other TCP sockets
*/
class TCP_socket : public communicating_socket 
{
public:
/**
*   Construct a TCP socket with no connection
*   @exception socket_exception thrown if unable to create TCP socket
	*/
	TCP_socket() throw(socket_exception);
	
	/**
	*   Construct a TCP socket with a connection to the given foreign address
	*   and port
	*   @param foreignAddress foreign address (IP address or name)
	*   @param foreignPort foreign port
	*   @exception socket_exception thrown if unable to create TCP socket
	*/
	TCP_socket(const pstring& foreignAddress, unsigned short foreignPort) throw(socket_exception);
	
private:
	// Access for TCP_server_socket::accept() connection creation
	friend class TCP_server_socket;
	TCP_socket(int newConnSD);
};

/**
*   TCP socket class for servers
*/
class TCP_server_socket : public base_socket 
{
public:
	/**
	*	Default constructor
	*/
	TCP_server_socket() throw(socket_exception);
	
	/**
	*   Blocks until a new connection is established on this socket or error
	*   @return new connection socket
	*   @exception socket_exception thrown if attempt to accept a new connection fails
	*/
	TCP_socket* accept() throw(socket_exception);

	void bind(unsigned short localPort) throw(socket_exception);

	void bind(const pstring& localAddress, unsigned short localPort) throw(socket_exception);

	void listen(int queueLen = 5) throw(socket_exception);

	void setReuseAddr() throw(socket_exception);
};

/**
*   UDP socket class
*/
class UDP_socket : public communicating_socket 
{
public:
	/**
	*   Construct a UDP socket
	*   @exception socket_exception thrown if unable to create UDP socket
	*/
	UDP_socket() throw(socket_exception);
	
	/**
	*   Construct a UDP socket with the given local port
	*   @param localPort local port
	*   @exception socket_exception thrown if unable to create UDP socket
	*/
	UDP_socket(unsigned short localPort) throw(socket_exception);
	
	/**
	*   Construct a UDP socket with the given local port and address
	*   @param localAddress local address
	*   @param localPort local port
	*   @exception socket_exception thrown if unable to create UDP socket
	*/
	UDP_socket(const pstring& localAddress, unsigned short localPort) throw(socket_exception);
	
	/**
	*   Unset foreign address and port
	*   @return true if disassociation is successful
	*   @exception socket_exception thrown if unable to disconnect UDP socket
	*/
	void disconnect() throw(socket_exception);
	
	/**
	*   Send the given buffer as a UDP datagram to the
	*   specified address/port
	*   @param buffer buffer to be written
	*   @param bufferLen number of bytes to write
	*   @param foreignAddress address (IP address or name) to send to
	*   @param foreignPort port number to send to
	*   @return true if send is successful
	*   @exception socket_exception thrown if unable to send datagram
	*/
	void sendTo(const void* buffer, int bufferLen, const pstring& foreignAddress, unsigned short foreignPort) throw(socket_exception);
	
	/**
	*   Read read up to bufferLen bytes data from this socket.  The given buffer
	*   is where the data will be placed
	*   @param buffer buffer to receive data
	*   @param bufferLen maximum number of bytes to receive
	*   @param sourceAddress address of datagram source
	*   @param sourcePort port of data source
	*   @return number of bytes received and -1 for error
	*   @exception socket_exception thrown if unable to receive datagram
	*/
	int recvFrom(void* buffer, int bufferLen, pstring& sourceAddress, unsigned short &sourcePort) throw(socket_exception);
	
	/**
	*   Set the multicast TTL
	*   @param multicastTTL multicast TTL
	*   @exception socket_exception thrown if unable to set TTL
	*/
	void setMulticastTTL(unsigned char multicastTTL) throw(socket_exception);
	
	/**
	*   Join the specified multicast group
	*   @param multicastGroup multicast group address to join
	*   @exception socket_exception thrown if unable to join group
	*/
	void joinGroup(const pstring& multicastGroup) throw(socket_exception);
	
	/**
	*   Leave the specified multicast group
	*   @param multicastGroup multicast group address to leave
	*   @exception socket_exception thrown if unable to leave group
	*/
	void leaveGroup(const pstring& multicastGroup) throw(socket_exception);
	
private:
	void setBroadcast();
};

} // namespace net

#endif // _NET_SOCKET_H_
