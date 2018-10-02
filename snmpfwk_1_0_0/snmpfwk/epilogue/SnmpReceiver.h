// SnmpReceiver.h: interface for the CSnmpReceiver class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
Project: SIU520 SNMP Proxy
Module: SNMP Core Framework
Copyright: (c) 2003-2004, RadVision.
License: see License.txt in the top level directory.
*/

#if !defined(AFX_RECEIVER_H__2E76ADC1_A36F_4ACA_8EB2_D278A3D26FCD__INCLUDED_)
#define AFX_RECEIVER_H__2E76ADC1_A36F_4ACA_8EB2_D278A3D26FCD__INCLUDED_


#include "EpiloguePacket.h"
#include "PracticalSocket.h"

//////////////////////////////////////////////////
// struct IQueryHandler
//
//! Defines interface of client  
/*! 
	Here is used pattern observer.    	
*/
struct IQueryHandler
{
	virtual void Query(CEpiloguePacket& packet) = 0;

protected:
	~IQueryHandler() {}
};

//////////////////////////////////////////////////
// class CSnmpReciever
//
//! Recieves Snmp requests and responds 
/*! 
*/
class CSnmpReceiver  
{
public:
	explicit CSnmpReceiver(u16 port = 161);

	void Register(IQueryHandler* customer);
	void Run();
	void Stop();

private:
	void Process(u8* buffer, u32& msgSize);	

private:	
	IQueryHandler* m_pCustomer;
	CEpiloguePacket m_packet;
	UDPSocket m_sock;
	u8 m_buffer[SNMP_MAX_PACKET_SIZE];
	volatile bool m_terminating;
};

#endif // !defined(AFX_RECEIVER_H__2E76ADC1_A36F_4ACA_8EB2_D278A3D26FCD__INCLUDED_)
