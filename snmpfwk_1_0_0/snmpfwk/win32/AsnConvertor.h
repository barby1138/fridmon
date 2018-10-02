// AsnConvertor.h: interface for the AsnConvertor class.
//
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Hyperon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef SNMPFWK_ASNCONVERTOR_INC_
#define SNMPFWK_ASNCONVERTOR_INC_

#ifndef WIN32
#error Win32 compiler required
#endif // WIN32

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
