// SnmpSenderImpl.h: interface for the SnmpSenderImpl class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
Project: SIU520 SNMP Proxy
Module: SNMP Core Framework
Copyright: (c) 2003-2004, RadVision.
License: see License.txt in the top level directory.
*/

#ifndef SNMP_FRMWRK_SENDERIMPL_EPILOGUE_INC_
#define SNMP_FRMWRK_SENDERIMPL_EPILOGUE_INC_


#include "epilogue/EpiloguePacket.h"
#include "epilogue/PracticalSocket.h"

//////////////////////////////////////////////////
// class SnmpSenderImpl
//
//! Sends traps(Epilogue version)  
/*! 
*/
namespace snmpfwk
{

class SnmpSenderImpl
{
public:
	SnmpSenderImpl();	
	bool open(const char* community);
	void close();
	bool send(const IpEndPoint& addr, const SnmpParamList& params);
private:
	CEpiloguePacket m_packet;
	pstring m_community;
	u8 m_buffer[SNMP_MAX_PACKET_SIZE];
	UDPSocket m_sock;
	s32 m_request_id; //signed!!!
};

//////////////////////////////////////////////////
// class CSmiValueBind
//
class CSmiValueBind : private ISnmpAnyVisitor
{
public:
	CSmiValueBind(const SnmpAny& any, const Oid& oid, CEpiloguePacket* packet_ptr, u32 index); 

private:
	virtual void visit(const s32& val);
	virtual void visit(const u32& val);
	virtual void visit(const pstring& val);
	virtual void visit(const Oid& val);
	virtual void visit(const u8& val);
	
private:
	CEpiloguePacket* m_packet_ptr;	
	Oid m_oid;
	u8 m_type;
	u32 m_index;
};

} // namespace snmpfwk

#endif // SNMP_FRMWRK_SENDERIMPL_EPILOGUE_INC_
