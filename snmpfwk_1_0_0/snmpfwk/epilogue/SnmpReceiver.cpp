// SnmpReceiver.cpp: implementation of the CSnmpReceiver class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
*/

#include "epilogue/SnmpReceiver.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSnmpReceiver::CSnmpReceiver(u16 port) 
	:	m_sock(port), 
		m_pCustomer(NULL),
		m_terminating(false)
{
}

//////////////////////////////////////////////////////////////////////
// Runs receiver
//////////////////////////////////////////////////////////////////////

void CSnmpReceiver::Run()
{
	while (!m_terminating) // run until stop
	{ 
		u32 recvMsgSize;			// size of received message
		std::string sourceAddress;  // address of datagram source
		u16 sourcePort;				// port of datagram source

		try 
		{
			// block until receive message from a client
			recvMsgSize = m_sock.recvFrom(m_buffer, SNMP_MAX_PACKET_SIZE, sourceAddress, 
				sourcePort);
			
//			TRACE_DEBUG("CSnmpReceiver::Run(): received packet from %s:%d, size: %d", 
//				sourceAddress.c_str(), sourcePort, recvMsgSize);
			
			Process(m_buffer, recvMsgSize);
			
			// sending reply message to a client
//			TRACE_DEBUG("CSnmpReceiver::Run(): sending answer to %s:%d, size: %d", 
//				sourceAddress.c_str(), sourcePort, recvMsgSize);
			m_sock.sendTo(m_buffer, recvMsgSize, sourceAddress, sourcePort);			
		}
		catch (.../*std::exception& ex*/)
		{
			// exit on closing a socket
			if (m_terminating)
				return;
//			TRACE_ERROR("CSnmpReceiver::Run(): exception: %s", ex.what());
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Stage of processing received message 
//////////////////////////////////////////////////////////////////////

void CSnmpReceiver::Process(u8 *buffer, u32& msgSize)
{
	// decoding SNMP packet
	if (!m_packet.Raw2Pkt(buffer, msgSize)) 
		throw std::exception(/*"SNMP packet decoding failed"*/);
	
	// query
	if (m_pCustomer) 
		m_pCustomer->Query(m_packet);
	else 
		m_packet.SetError(0, RESOURCE_UNAVAILABLE);
	
	// encoding to SNMP packet
	m_packet.SetType(GET_RESPONSE_PDU);	// change type to response
	if (!m_packet.Pkt2Raw(buffer, msgSize)) 
		throw std::exception(/*"SNMP packet encoding failed"*/);
}

//////////////////////////////////////////////////////////////////////
// Used to register client (Observer pattern)
//////////////////////////////////////////////////////////////////////

void CSnmpReceiver::Register(IQueryHandler* customer)
{
	m_pCustomer = customer;
}

//////////////////////////////////////////////////////////////////////
// Stops receiver = closes socket and thread
//////////////////////////////////////////////////////////////////////

void CSnmpReceiver::Stop()
{
	m_terminating = true;
	m_sock.Close(); // closing a socket
}
