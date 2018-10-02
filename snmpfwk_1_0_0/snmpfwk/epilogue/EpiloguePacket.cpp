// EpiloguePacket.cpp: implementation of the CEpiloguePacket class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
*/

#include "epilogue/EpiloguePacket.h"

extern "C"{
#include <envoy/h/snmpstat.h>
#include <envoy/h/objectid.h>
}

//////////////////////////////////////////////////////////////////////
// Macroses
//////////////////////////////////////////////////////////////////////

#define CONVERT_OID(val) (u32*)val.data()
#define CONVERT_STRING(val) (u8*)val.c_str()
#define CONVERT_IPADDRRES(val) (u8*)val.getData()

//////////////////////////////////////////////////////////////////////
// Global definitions that Epilogue needs
//////////////////////////////////////////////////////////////////////

SNMP_STATS_T    snmp_stats;

int validate_SNMP_community(SNMP_PKT_T *rp, SNMPADDR_T *pktsrc,
                                            SNMPADDR_T *pktdst)
{
    rp->mib_view = 0xFF;
    return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEpiloguePacket::CEpiloguePacket()  
{
	m_packet = 0;
}

CEpiloguePacket::~CEpiloguePacket()
{
	ClearPacket();
}

//////////////////////////////////////////////////////////////////////
// Returns buffer ready to send in SNMP format
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::Pkt2Raw(u8 *buffer, u32 &msgSize)
{
//--//Encoding from Epilogue format to SNMP packet//--//
	
	EBUFFER_T	eBuffer;
	EBufferInitialize(&eBuffer);
// BFL_IS_DYNAMIC =  Buffer was obtained by malloc    
// BFL_IS_STATIC  =  Buffer is statically allocated     
	EBufferSetup(BFL_IS_STATIC, &eBuffer, buffer, SNMP_MAX_PACKET_SIZE);
	
	if( SNMP_Encode_Packet(m_packet, &eBuffer) != 0 )
	{
//		TRACE_ERROR("Error : can't encode packet");
		return false;
	}

//--//Unwrapping EBUFFER_T to raw data//--//
	msgSize = EBufferUsed(&eBuffer);	

	return true;
}

//////////////////////////////////////////////////////////////////////
// Fills EpiloguePacket object from buffer with SNMP data
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::Raw2Pkt(const u8* buffer, const u32 msgSize)
{

	pstring buf((const char*)buffer, msgSize);
	
	//--//Wrapping raw data in EBUFFER_T//--//
	EBUFFER_T	eBuffer;
	
	EBufferInitialize(&eBuffer);

// BFL_IS_DYNAMIC =  Buffer was obtained by malloc    
// BFL_IS_STATIC  =  Buffer is statically allocated     
	
	EBufferPreLoad(BFL_IS_STATIC, &eBuffer, buf.c_str() , msgSize);
	u16 pktUsedLen = EBufferUsed(&eBuffer);

//--//Decoding SNMP packet to Epilogue format//--//

	SetPacket(SNMP_Decode_Packet(
			eBuffer.start_bp,//location of the buffer
			pktUsedLen,       //length of the buffer
			NULL,
			NULL));
	
	
	if( (SNMP_PKT_T*)m_packet == 0 )
	{
//		TRACE_ERROR("Error : packet is empty");
		return false;
	}	
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// Get methods
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Returns error status and index
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::GetError(u32& error_index, u32& error_status) const
{
	error_index = m_packet->pdu.std_pdu.error_index;
	error_status = m_packet->pdu.std_pdu.error_status;	
}

//////////////////////////////////////////////////////////////////////
// Returns version of protocol
//////////////////////////////////////////////////////////////////////

u32 CEpiloguePacket::GetVersion() const 
{
	return m_packet->snmp_version;
}

//////////////////////////////////////////////////////////////////////
// Returns id of message
//////////////////////////////////////////////////////////////////////

u32 CEpiloguePacket::GetId() const
{
	return m_packet->pdu.std_pdu.request_id;
}

//////////////////////////////////////////////////////////////////////
// Returns type of PDU
//////////////////////////////////////////////////////////////////////

u16	CEpiloguePacket::GetType() const
{
	return (m_packet->pdu_type | 0xA0);
}

//////////////////////////////////////////////////////////////////////
// Returns type of PDU as string
//////////////////////////////////////////////////////////////////////

pstring CEpiloguePacket::GetTypeName() const
{
	switch ( m_packet->pdu_type ) 
	{
		case GET_REQUEST_PDU :
			return pstring("GET_REQUEST_PDU");
		case GET_NEXT_REQUEST_PDU :
			return pstring("GET_NEXT_REQUEST_PDU");
		case GET_RESPONSE_PDU :
			return pstring("GET_RESPONSE_PDU");
		case SET_REQUEST_PDU :
			return pstring("SET_REQUEST_PDU");
		case TRAP_PDU :
			return pstring("TRAP_PDU");
		case GET_BULK_REQUEST_PDU :
			return pstring("GET_BULK_REQUEST_PDU");
		case INFORM_REQUEST_PDU :
			return pstring("INFORM_REQUEST_PDU");
		case TRAP2_PDU :
			return pstring("TRAP2_PDU");
		case REPORT_PDU :
			return pstring("REPORT_PDU");
//		case MAX_V1_PDU :
		//	return pstring("MAX_V1_PDU");
	//	case MAX_PDU :
	//		return pstring("MAX_PDU");
		case NO_PDU :
			return pstring("NO_PDU");
		default :	
			return pstring();
	}			
}

//////////////////////////////////////////////////////////////////////
// Returns length of VarBindList
//////////////////////////////////////////////////////////////////////

int CEpiloguePacket::GetVblLength() const 
{
	return m_packet->pdu.std_pdu.std_vbl.vbl_count;
}

//////////////////////////////////////////////////////////////////////
// Returns OID length of item with given index in VarBindList
//////////////////////////////////////////////////////////////////////

Oid CEpiloguePacket::GetMainOID (u32 index) const
{
	unsigned long* OIDbegin = m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_obj_id.component_list;
	int OIDlength = m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_obj_id.num_components;
	return Oid((u32*)OIDbegin, OIDlength);			
}

//////////////////////////////////////////////////////////////////////
// Returns length of data in item
//////////////////////////////////////////////////////////////////////

u16 CEpiloguePacket::GetDataLength (u32 index) const
{
	return m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_data_length;
	
}

//////////////////////////////////////////////////////////////////////
// Returns data type of value in item
//////////////////////////////////////////////////////////////////////

u8 CEpiloguePacket::GetDataType(u32 index) const
{
	return m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_data_flags_n_type;
}

//////////////////////////////////////////////////////////////////////
// Returns value of item in VarBind list (for inner usage)
//////////////////////////////////////////////////////////////////////

s32 CEpiloguePacket::GetNumber (u32 index) const
{
	return m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_number;
}

u32 CEpiloguePacket::GetCounter (u32 index) const
{
	return m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_counter;
}

u64 CEpiloguePacket::GetCounter64 (u32 index) const
{
	u64 tmp;
	tmp.high = m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_counter64.high;
	tmp.low = m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_counter64.low;
	return tmp;
}


IpAddress CEpiloguePacket::GetIPAddress (u32 index) const
{
	IpAddress address;
	u8* IPbegin = m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_network_address;
	address.fromStream(IPbegin);	
	return address;	
}

Oid CEpiloguePacket::GetOID (u32 index) const
{
	
	unsigned long* OIDbegin = m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_object.component_list;
	int OIDlength = m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_object.num_components;
	return Oid((u32*)OIDbegin, OIDlength);	
}

pstring CEpiloguePacket::GetString (u32 index) const
{
	EBUFFER_T* eBuffer = &m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_string;
	u16 length = EBufferUsed(eBuffer);
	return pstring ((const char*)eBuffer->start_bp, length);	
}

//////////////////////////////////////////////////////////////////////
// Returns data type of value in item as string
//////////////////////////////////////////////////////////////////////

pstring CEpiloguePacket::GetDataTypeName( u32 index ) const
{
	switch ( GetDataType(index) ) 
		{
		case VT_NUMBER :
			return pstring("NUMBER");
		//case VT_BITS :
		case VT_STRING :
			return pstring("OCTET STRING");
		case VT_OBJECT :
			return pstring("OBJECT");
		case VT_EMPTY :
			return pstring("EMPTY");
		case VT_IPADDRESS :
			return pstring("IPADDRESS");
		case VT_COUNTER :
			return pstring("COUNTER");
		//case VT_GAUGE :
		case VT_UNSIGNED32 :
			return pstring("GAUGE or UNSIGNED32");
		case VT_TIMETICKS :
			return pstring("TIMETICKS");
		case VT_OPAQUE :
			return pstring("OPAQUE");
		case VT_COUNTER64 :
			return pstring("COUNTER64");
		case VT_UINTEGER32 :
			return pstring("UINTEGER32");
		default:
			return pstring();
		}	
}

//////////////////////////////////////////////////////////////////////
// Set methods
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Sets error status and index
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::SetError(const u32 error_index, const u32 error_status)
{
	m_packet->pdu.std_pdu.error_index = error_index;
	m_packet->pdu.std_pdu.error_status = error_status;
}

//////////////////////////////////////////////////////////////////////
// Sets PDU type
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::SetType(const u16 type)
{
	m_packet->pdu_type = type;
}

//////////////////////////////////////////////////////////////////////
// Sets type of value of item with given index
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::SetDataType(const u32 index,const u8 type)
{
	m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_data_flags_n_type = type;
}

//////////////////////////////////////////////////////////////////////
// Sets value of item in VarBind list (for inner usage)
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::SetMainOID (u32 index, const Oid& val)
{
	Clean_Obj_ID(&m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_obj_id);
	m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_obj_id.num_components = val.size();
	m_packet->pdu.std_pdu.std_vbl.vblist[index].vb_obj_id.component_list = CopyOid(val);
} 

//////////////////////////////////////////////////////////////////////
// Sets value of item in VarBind list (for inner usage)
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::SetNumber (u32 index, s32 value)
{
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_number = (INT_32_T) value;
}

void CEpiloguePacket::SetCounter (u32 index, u32 value)
{
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_counter = (UINT_32_T) value;
}

void CEpiloguePacket::SetCounter64 (u32 index, const u64& value)
{
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_counter64.high = value.high;
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_counter64.low = value.low;
}

bool CEpiloguePacket::SetIPAddress (u32 index, const IpAddress& value)
{
//	TRACE_DEBUG(value.ToString().c_str());
	u8* p_IPbegin = m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_network_address;
	value.toStream(p_IPbegin);
	return true;
}

void CEpiloguePacket::SetOID (u32 index, const Oid& value)
{
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_object.num_components = value.size();
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_object.component_list = CopyOid(value);
}

void CEpiloguePacket::SetString (u32 index, const pstring& value)
{
	EBUFFER_T eBuffer;
	EBufferInitialize(&eBuffer);
	u8* ptr = CopyString(value);
	EBufferPreLoad(BFL_IS_DYNAMIC, &eBuffer, ptr, value.length());
	m_packet->pdu.std_pdu.std_vbl.vblist[index].value_u.v_string = eBuffer;
}

//////////////////////////////////////////////////////////////////////
// Allocates GET, GET NEXT, SET REQUEST packets for SNMP v1 & v2
// and GET BULK, INFORM, TRAP2 REQUEST packets only for SNMP v2
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::CreateRequest2(u32 ptype, u32 version, const pstring& community, s32 request_id, u32 num_vb, u32 nonreps, u32 maxreps)
{
	SetPacket(SNMP_Create_Request2(
		ptype, 
		version, 
		community.length(), 
		(char*)CONVERT_STRING(community), 
		request_id, 
		num_vb, 
		nonreps, 
		maxreps));

	if (m_packet == 0)
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////
// Binds value of type VT_NUMBER 
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::BindInteger(u32 index, const Oid& compl, s32 value)
{
	if (SNMP_Bind_Integer(m_packet, index, compl.size(), CONVERT_OID(compl), value))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Binds value of types VT_COUNTER, VT_GAUGE or VT_TIMETICKS
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::BindUnsignedInteger(u32 index, const Oid& compl, u8 type ,u32 value)
{
	if (SNMP_Bind_Unsigned_Integer(m_packet, index, compl.size(), CONVERT_OID(compl), type ,value))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Binds value of type VT_IPADDRESS
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::BindIPAddress(u32 index, const Oid& compl, const IpAddress& value)
{
	//u8 ipaddress[4];
	//value.ToStream(ipaddress);
	if (SNMP_Bind_IP_Address(m_packet, index, compl.size(), CONVERT_OID(compl), CONVERT_IPADDRRES(value)))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Binds value of type VT_OBJECT
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::BindObjectID(u32 index, const Oid& compl, const Oid& value)
{
	if (SNMP_Bind_Object_ID(m_packet, index, compl.size(), CONVERT_OID(compl), value.size(), CONVERT_OID(value) ))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Binds value of types VT_STRING and VT_OPAQUE
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::BindString(u32 index, const Oid& compl, u8 type , const pstring& value)
{
	bool copy_flag = 1; //don't copy string, use as is
	if (SNMP_Bind_String(m_packet, index, compl.size(), CONVERT_OID(compl), type, value.size(), CONVERT_STRING(value), copy_flag))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Binds value of type VT_COUNTER64
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::Bind64UnsignedInteger(u32 index, const Oid& compl, u64 value)
{
	if (SNMP_Bind_64_Unsigned_Integer(m_packet, index, compl.size(), CONVERT_OID(compl), VT_COUNTER64, value.high, value.low))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Binds value of type VT_NULL
//////////////////////////////////////////////////////////////////////

bool CEpiloguePacket::BindNull(u32 index, const Oid& compl)
{
	if (SNMP_Bind_Null(m_packet, index, compl.size(), CONVERT_OID(compl)))
		return false;
	return true;	
}

//////////////////////////////////////////////////////////////////////
// Utilities
//////////////////////////////////////////////////////////////////////

void CEpiloguePacket::SetPacket(SNMP_PKT_T* packet)
{
	ClearPacket();
	m_packet = packet;
}

void CEpiloguePacket::ClearPacket()//deleting packet's data
{
	if(m_packet) 
		SNMP_Free(m_packet);
}

u32* CEpiloguePacket::CopyOid(const Oid& value)
{
	if (value.empty())
		return NULL;
	u32* data = (u32*)SNMP_memory_alloc (value.size() * sizeof(u32));
	std::copy(value.data(), value.data() + value.size(), data);
	return data;
}

u8* CEpiloguePacket::CopyString(const pstring& value)
{
	if (value.empty())
		return NULL;
	u8* data = (u8*)SNMP_memory_alloc (value.length() * sizeof(u8));
	std::copy(value.begin(), value.end(), data);
	return data;
}


