// SnmpSenderImpl.cpp: implementation for the SnmpSenderImpl class.
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

#include "SnmpSenderImpl.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// SnmpSenderImpl Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpSenderImpl::SnmpSenderImpl()
	:	_session(SNMPAPI_FAILURE)
{
}

SnmpSenderImpl::~SnmpSenderImpl()
{
	close();
}

//////////////////////////////////////////////////////////////////////
// SnmpSenderImpl Methods
//////////////////////////////////////////////////////////////////////

bool SnmpSenderImpl::open(const char* community)
{
	smiUINT32 majorVersion, minorVersion, level, translateMode, retransmitMode;
		
	if (::SnmpStartup(&majorVersion, &minorVersion, &level, &translateMode, &retransmitMode)
		!= SNMPAPI_SUCCESS)
	{
		//SNMP_TRACE("CSnmpTrapSender::Open(): SnmpStartup failed\n");
		return false;
	}
			
	if (::SnmpSetTranslateMode(SNMPAPI_UNTRANSLATED_V2) != SNMPAPI_SUCCESS)
	{
		//SNMP_TRACE("CSnmpTrapSender::Open(): SnmpSetTranslateMode failed\n");
		return false;
	}
		
	if (::SnmpSetRetransmitMode(SNMPAPI_ON) != SNMPAPI_SUCCESS)
	{
		//SNMP_TRACE("CSnmpTrapSender::Open(): SnmpSetRetransmitMode failed\n");
		return false;
	}
		
	_session = ::SnmpCreateSession(NULL, 0, &cbFunc, NULL);
	if (_session == SNMPAPI_FAILURE)
	{
		//SNMP_TRACE("CSnmpTrapSender::Open(): SnmpCreateSession failed\n");
		return false;
	}

	_community = community;
	return true;
}

void SnmpSenderImpl::close()
{
	if (_session == SNMPAPI_FAILURE)
		return;

	if (::SnmpClose(_session) != SNMPAPI_SUCCESS)
	{
		//SNMP_TRACE("CSnmpTrapSender::Close(): SnmpClose failed\n");
	}

	if (::SnmpCleanup() != SNMPAPI_SUCCESS)
	{
		//SNMP_TRACE("CSnmpTrapSender::Close(): SnmpCleanup failed\n");
	}

	_session = SNMPAPI_FAILURE;
}

bool SnmpSenderImpl::send(const IpEndPoint& addr, const SnmpParamList& params)
{
	// create a destination entry
	HSNMP_ENTITY hdest = ::SnmpStrToEntity(_session, addr.getAddress().toString().c_str());
	if (hdest == SNMPAPI_FAILURE)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): the destination entry creation failed\n");
		return false;
	}
	if (::SnmpSetPort(hdest, addr.getPort()) == SNMPAPI_FAILURE)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): SnmpSetPort failed\n");
		return false;
	}

	// create a context entry
	smiOCTETS context;
	context.len = _community.length();
	context.ptr = (smiLPBYTE)_community.c_str();
	HSNMP_CONTEXT hcontext = ::SnmpStrToContext(_session, &context);
	if (hcontext == SNMPAPI_FAILURE)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): the context entry creation failed\n");
		return false;
	}

	// create a variable bindings list
	HSNMP_VBL vblist = ::SnmpCreateVbl(_session, NULL, NULL);
	if (vblist == SNMPAPI_FAILURE)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): SnmpCreateVbl failed\n");
		return false;
	}

	// create variable binding entries
	for (SnmpParamList::const_iterator itor = params.begin(); itor != params.end(); ++itor)
	{
		const SnmpParam& param = *itor;

		// bind WinSNMP entities to framework entities
		SmiOidBind smiOid(param.name);
		SmiValueBind smiValue(param.value);

		// add a variable to the variable bindings list
		if (::SnmpSetVb(vblist, 0, smiOid.getData(), smiValue.getData()) == SNMPAPI_FAILURE)
		{
			//SNMP_TRACE("SnmpSenderImpl::Send(): SnmpSetVb failed\n");
			return false;
		}
	}

	// create the protocol data unit
	HSNMP_PDU hpdu = ::SnmpCreatePdu(_session, SNMP_PDU_TRAP, 0, 0, 0, vblist);
	if (hpdu == SNMPAPI_FAILURE)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): SnmpCreatePdu failed\n");
		return false;
	}
					
	// send a message
	if (::SnmpSendMsg(_session, 0, hdest, hcontext, hpdu) != SNMPAPI_SUCCESS)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): SnmpSendMsg failed, code=%d\n",	SnmpGetLastError(_session));
		return false;
	}

	return true;
}

SNMPAPI_STATUS CALLBACK SnmpSenderImpl::cbFunc(HSNMP_SESSION hSession, HWND hWnd, UINT wMsg,
												WPARAM wParam, LPARAM lParam, LPVOID lpClientData)
{
	return SNMPAPI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// SmiOidBind
//////////////////////////////////////////////////////////////////////

SmiOidBind::SmiOidBind(const Oid& oid)
{
	_smiOid.len = oid.size();
	_smiOid.ptr = const_cast<u32*>(oid.data());	
}

//////////////////////////////////////////////////////////////////////
// SmiValueBind
//////////////////////////////////////////////////////////////////////

SmiValueBind::SmiValueBind(const SnmpAny& any)
{
	// convert
	_smiValue.syntax = any.getType();
	any.accept(*this);
}

void SmiValueBind::visit(const s32& val)
{
	_smiValue.value.sNumber = val;
}

void SmiValueBind::visit(const u32& val)
{
	_smiValue.value.uNumber = val;
}

void SmiValueBind::visit(const pstring& val)
{
	_smiValue.value.string.len = val.length();
	_smiValue.value.string.ptr = val.length() ? (u8*)val.c_str() : NULL;
}

void SmiValueBind::visit(const Oid& val)
{
	_smiValue.value.oid.len = val.size();
	_smiValue.value.oid.ptr = val.size() ? (u32*)val.data() : NULL;
}

void SmiValueBind::visit(const u8& val)
{
	_smiValue.value.empty = val;
}