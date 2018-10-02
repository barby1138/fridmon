// AsnConvertor.h: interface for the AsnConvertor class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Serge Gaziev
*/

#ifndef SNMPFWK_ASNCONVERTOR_INC_
#define SNMPFWK_ASNCONVERTOR_INC_

#ifndef _REDHAT
#error Improper target platform. Redhat expected.
#endif // _REDHAT

#include "../SnmpTypes.h"
#include <snmp.h>

namespace snmpfwk
{

//////////////////////////////////////////////////
// class AsnConvertor
//
class AsnConvertor : public quark::noncopyable,
					 private ISnmpAnyVisitor
{
public:
	bool convert(const SnmpVarBind& winParam, SnmpParam& portParam);
	bool convert(const SnmpParam& portParam, SnmpVarBind& winParam);
	bool convert(const AsnObjectName& winOid, Oid& portOid);
	bool convert(const Oid& portOid, AsnObjectName& winOid);
	bool convert(const AsnAny& winAny, SnmpAny& portAny);
	bool convert(const SnmpAny& portAny, AsnAny& winAny);

	static AsnConvertor& instance();

private:
	AsnConvertor() {}

	// ISnmpAnyVisitor
	virtual void visit(const s32& val);
	virtual void visit(const u32& val);
	virtual void visit(const quark::pstring& val);
	virtual void visit(const Oid& val);
	virtual void visit(const u8& val);

private:
	AsnAny _winAny;
};

} // namespace snmpfwk

#endif // SNMPFWK_ASNCONVERTOR_INC_
