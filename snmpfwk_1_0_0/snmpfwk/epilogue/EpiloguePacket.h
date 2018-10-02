// EpiloguePacket.h: interface for the CEpiloguePacket class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Denis Garyachy
Project: SIU520 SNMP Proxy
Module: SNMP Core Framework
Copyright: (c) 2003-2004, RadVision.
License: see License.txt in the top level directory.
*/

#if !defined(AFX_EpiloguePacket_H__A308FB21_728C_46D0_8714_2F0E4AB0F809__INCLUDED_)
#define AFX_EpiloguePacket_H__A308FB21_728C_46D0_8714_2F0E4AB0F809__INCLUDED_

#include <SnmpTypes.h>

extern "C"{
#include <envoy/h/buildpkt.h>
}

using namespace snmpfwk;
using namespace quark;

typedef struct unsigned64
        {
        u32  high;        /* the high (msb) 4 bytes of the uint */
        u32  low;         /* the low (lsb) 4 bytes of the uint  */
        } u64;


//////////////////////////////////////////////////
// class CEpiloguePacket
//
//! Wrapper class for Epilogue Snmp packet  
/*! 
*/
class CEpiloguePacket  
{
		
public:	
	CEpiloguePacket();
	virtual ~CEpiloguePacket();
	
	///////// Convert methods /////////
	bool Pkt2Raw(u8 *buffer, u32 &msgSize);
	bool Raw2Pkt(const u8 *buffer, const u32 msgSize);
	///////// Utilities ///////////////
	void ClearPacket();
	u32* CopyOid(const Oid& compl);
	u8* CopyString(const pstring& value);
					
	///////// Create methods /////////	
	bool CreateRequest2(u32 ptype, u32 version, const pstring& community, s32 request_id, u32 num_vb, u32 nonreps, u32 maxreps);
	bool BindInteger(u32 index, const Oid& compl, s32 value);
	bool BindUnsignedInteger(u32 index, const Oid& compl, u8 type ,u32 value);
	bool BindIPAddress(u32 index, const Oid& compl, const IpAddress& value);
	bool BindObjectID(u32 index, const Oid& compl, const Oid& value);
	bool BindString(u32 index, const Oid& compl, u8 type , const pstring& value);
	bool Bind64UnsignedInteger(u32 index, const Oid& compl, u64 value);
	bool BindNull(u32 index, const Oid& compl);
	
	///////// Get methods /////////	
	u32 GetVersion() const;
	u32 GetId() const;
	u16	GetType() const;
	void GetError(u32& error_index, u32& error_status) const;
	int GetVblLength() const; 
	Oid GetMainOID (u32 index) const; 
	u16 GetDataLength (u32 index) const;
	u8 GetDataType(u32 index) const;
	pstring GetDataTypeName(u32 index) const;
	pstring GetTypeName() const;
	///////// for values //
	s32 GetNumber (u32 index) const;
	u32 GetCounter (u32 index) const;
	u64 GetCounter64 (u32 index) const;
	IpAddress GetIPAddress (u32 index) const;
	Oid GetOID (u32 index) const;
	pstring GetString (u32 index) const;
	
	///////// Set methods /////////
	void SetError(const u32 error_index, const u32 error_status);
	void SetType(const u16 type);
	void SetDataType(const u32 index,const u8 type);
	void SetMainOID (u32 index, const Oid& val); 
	///////// for values //
	void SetNumber (u32 index, s32 value);
	void SetCounter (u32 index, u32 value);
	void SetCounter64 (u32 index, const u64& value);
	bool SetIPAddress (u32 index, const IpAddress& value);
	void SetOID (u32 index, const Oid& value);
	void SetString (u32 index, const pstring& value);

protected:
	void SetPacket(SNMP_PKT_T* packet);

private:
	SNMP_PKT_T* m_packet;
};

#endif // !defined(AFX_EpiloguePacket_H__A308FB21_728C_46D0_8714_2F0E4AB0F809__INCLUDED_)
