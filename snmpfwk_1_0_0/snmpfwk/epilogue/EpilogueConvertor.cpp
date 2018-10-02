// EpilogueConvertor.cpp: implementation of the CEpilogueConvertor class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
*/

#include "epilogue/EpilogueConvertor.h"

//////////////////////////////////////////////////////////////////////
// Used for singleton realisation
//////////////////////////////////////////////////////////////////////

CEpilogueConvertor& CEpilogueConvertor::Instance()
{
	static CEpilogueConvertor inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////
// Converts from epilogue format item to inner format
//////////////////////////////////////////////////////////////////////

bool CEpilogueConvertor::Convert(const CEpiloguePacket& packet, u32 index, SnmpAny& portAny)
{
	switch (packet.GetDataType(index))
	{
	case asnInteger32:
		portAny = SnmpAsnInt32(packet.GetNumber(index));
		break;
	case asnUnsigned32:
		portAny = SnmpAsnUint32(packet.GetCounter(index));
		break;
	case asnCounter32:
		portAny = SnmpAsnCntr32(packet.GetCounter(index));
		break;
	case asnGauge32:
		portAny = SnmpAsnGauge32(packet.GetCounter(index));
		break;
	case asnTimeTicks:
		portAny = SnmpAsnTicks(packet.GetCounter(index));
		break;
	case asnOctetString:
		portAny = SnmpAsnOctets(packet.GetString(index));
		break;
	case asnBitString:
		portAny = SnmpAsnBits(packet.GetString(index));
		break;
	case asnOpaque:
		portAny = SnmpAsnOpaque(packet.GetString(index));
		break;
	case asnIpAddress:
		portAny = SnmpAsnIp(pstring((const char*)packet.GetIPAddress(index).getData(), IpAddress::ipSize));
		break;
	case asnObjectName:
		portAny = SnmpAsnOid(packet.GetOID(index));
		break;
	case asnNull:
		portAny = SnmpAsnNull();
		break;
	default:
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// Converts from inner format to epilogue item
//////////////////////////////////////////////////////////////////////

bool CEpilogueConvertor::Convert(const SnmpAny& portAny, CEpiloguePacket& packet, u32 index)
{
	
	//Init of temporary members
	m_pPacket = &packet;
	m_index = index;
	
	packet.SetDataType(index, portAny.getType() );
	portAny.accept(*this);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// Convert calls Accept that calls appropriate visit (Visitor pattern)
//////////////////////////////////////////////////////////////////////

void CEpilogueConvertor::visit(const s32& val)
{
	m_pPacket->SetNumber(m_index, val);
}

void CEpilogueConvertor::visit(const u32& val)
{
	m_pPacket->SetCounter(m_index, val);
}

void CEpilogueConvertor::visit(const pstring& val)
{
	if(m_pPacket->GetDataType(m_index) == asnIpAddress)
	{
		m_pPacket->SetIPAddress(m_index, IpAddress::fromString(val.c_str()));	
	}
	else
		m_pPacket->SetString(m_index,val);	
}

void CEpilogueConvertor::visit(const Oid& val)
{
	m_pPacket->SetOID(m_index,val);	
}

void CEpilogueConvertor::visit(const u8& val)
{
	m_pPacket->SetCounter(m_index,0);	//Null
}
