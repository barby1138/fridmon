// SnmpSenderImpl.cpp: implementation for the SnmpSenderImpl class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
*/

#include "epilogue/SnmpSenderImpl.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// SnmpSenderImpl Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpSenderImpl::SnmpSenderImpl()
	:	m_request_id(0)
{
}

//////////////////////////////////////////////////////////////////////
// Sets community  
//////////////////////////////////////////////////////////////////////

bool SnmpSenderImpl::open(const char* community) 
{
	m_community = community;
	return true;
}

//////////////////////////////////////////////////////////////////////
// Just for compatibility with windows version  
//////////////////////////////////////////////////////////////////////

void SnmpSenderImpl::close()
{
}

//////////////////////////////////////////////////////////////////////
// Sends trap
//////////////////////////////////////////////////////////////////////

bool SnmpSenderImpl::send(const IpEndPoint& addr, const SnmpParamList& params) 
{
	u32 msgsize;
	{
	//BenchScopeLow scope("trap converting");
	
	m_packet.CreateRequest2(TRAP2_PDU, 1, m_community, m_request_id++, params.size() , 0, 0);

	u32 i = 0;
	for (SnmpParamList::const_iterator itor = params.begin(); itor != params.end(); ++itor, ++i)
	{
		const SnmpParam& param = *itor;

		// bind SNMP entities to framework entities
		
		//oid.assign(param.name.GetData(), param.name.GetData() + param.name.GetSize());		
		CSmiValueBind smiValue(param.value, param.name , &m_packet, i);		
	}

	m_packet.Pkt2Raw(m_buffer, msgsize);
	}
	
	try
	{
	//	BenchScopeLow scope("trap sending");
		m_sock.sendTo(m_buffer, msgsize, addr.getAddress().toString().c_str(), addr.getPort());
//		TRACE_DEBUG("SnmpSenderImpl::Send(): sending packet to %s:%d", 
//			addr.GetAddress().ToString().c_str(), addr.GetPort());
	}
	catch(.../*const std::exception& ex*/)
	{
//		TRACE_DEBUG("SnmpSenderImpl::Send(): %s", ex.what());
	}
	
	//TRACE_DEBUG("profiler dump: %s", ProfilerLow::Instance().Dump().c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////
// CSmiValueBind constructor
//////////////////////////////////////////////////////////////////////

CSmiValueBind::CSmiValueBind(const SnmpAny& any, const Oid& oid, CEpiloguePacket* packet_ptr, u32 index) 
	:	m_oid(oid), m_packet_ptr(packet_ptr), m_index(index), m_type(any.getType())
{
	// bind
	any.accept(*this);
}

//////////////////////////////////////////////////////////////////////
// Accept from CSmiValueBind constructor calls appropriate visit 
// (Visitor pattern)
//////////////////////////////////////////////////////////////////////

void CSmiValueBind::visit(const s32& val)
{
	m_packet_ptr->BindInteger(m_index, m_oid, val);
}

void CSmiValueBind::visit(const u32& val)
{
	m_packet_ptr->BindUnsignedInteger(m_index, m_oid, m_type, val);	
}

void CSmiValueBind::visit(const pstring& val)
{
	if(m_type == asnIpAddress)
	{
		m_packet_ptr->BindIPAddress(m_index, m_oid, IpAddress::fromString(val.c_str()));
	}
	else
		m_packet_ptr->BindString(m_index, m_oid, m_type, val);	
}

void CSmiValueBind::visit(const Oid& val)
{
	m_packet_ptr->BindObjectID(m_index, m_oid, val);	
}

void CSmiValueBind::visit(const u8& val)
{
	m_packet_ptr->BindNull(m_index, m_oid);
}
