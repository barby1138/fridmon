// SnmpSenderImpl.h: interface for the SnmpSenderImpl class.
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

#ifndef SNMPFWK_SENDERIMPL_INC_
#define SNMPFWK_SENDERIMPL_INC_

#ifndef WIN32
#error Win32 compiler required
#endif // WIN32

#include "SnmpSender.h"
#include <WinSnmp.h>

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
	static SNMPAPI_STATUS CALLBACK cbFunc(HSNMP_SESSION hSession, HWND hWnd, UINT wMsg,
										  WPARAM wParam, LPARAM lParam, LPVOID lpClientData);
private:	
	HSNMP_SESSION _session;
	quark::pstring _community;
};

//////////////////////////////////////////////////
// class SmiOidBind
//
class SmiOidBind
{
public:
	SmiOidBind(const Oid& oid);

	const smiOID* getData() const
		{ return &_smiOid; }

private:
	smiOID _smiOid;
};

//////////////////////////////////////////////////
// class SmiValueBind
//
class SmiValueBind : private ISnmpAnyVisitor
{
public:
	SmiValueBind(const SnmpAny& any);

	const smiVALUE* getData() const
		{ return &_smiValue; }

private:
	virtual void visit(const s32& val);
	virtual void visit(const u32& val);
	virtual void visit(const quark::pstring& val);
	virtual void visit(const Oid& val);
	virtual void visit(const u8& val);

private:
	smiVALUE _smiValue;
};

} // namespace snmp

#endif // SNMPFWK_SENDERIMPL_INC_
