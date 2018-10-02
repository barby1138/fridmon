// SnmpSenderImpl.h: interface for the SnmpSenderImpl class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Serg Masyutin
*/

#ifndef SNMPFWK_SENDERIMPL_INC_
#define SNMPFWK_SENDERIMPL_INC_

#ifndef _REDHAT
#error Improper target platform. Redhat expected.
#endif // _REDHAT

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "SnmpSender.h"

namespace snmpfwk
{

//////////////////////////////////////////////////
// class SnmpSenderImpl
//
class SnmpSenderImpl
{
public:
	SnmpSenderImpl();
	~SnmpSenderImpl();

	bool open(const char* community);
	void close();
	bool send(const IpEndPoint& addr, const SnmpParamList& params);

private:	
	struct snmp_session *_session;
	quark::pstring _community;
};
//
////////////////////////////////////////////////////
//// class SmiOidBind
////
//class SmiOidBind
//{
//public:
//	SmiOidBind(const Oid& oid);
//
//	const smiOID* getData() const
//		{ return &_smiOid; }
//
//private:
//	smiOID _smiOid;
//};
//
////////////////////////////////////////////////////
//// class SmiValueBind
////
//class SmiValueBind : private ISnmpAnyVisitor
//{
//public:
//	SmiValueBind(const SnmpAny& any);
//
//	const smiVALUE* getData() const
//		{ return &_smiValue; }
//
//private:
//	virtual void visit(const s32& val);
//	virtual void visit(const u32& val);
//	virtual void visit(const quark::pstring& val);
//	virtual void visit(const Oid& val);
//	virtual void visit(const u8& val);
//
//private:
//	smiVALUE _smiValue;
//};

} // namespace snmp

#endif // SNMPFWK_SENDERIMPL_INC_
