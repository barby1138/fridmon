// SnmpSenderImpl.cpp: implementation for the SnmpSenderImpl class.
//
//////////////////////////////////////////////////////////////////////

/*
Author: Serg Masyutin
*/

#include "SnmpSenderImpl.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// SnmpSenderImpl Construction/Destruction
//////////////////////////////////////////////////////////////////////

SnmpSenderImpl::SnmpSenderImpl()
	:	_session(NULL)
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
	_community = community;

	init_snmp("SnmpSenderImpl_redhat");

	return true;
}

void SnmpSenderImpl::close()
{
	if (_session != NULL) 
	{
		snmp_close(_session);
		_session = NULL;
	}
}

bool SnmpSenderImpl::send(const IpEndPoint& addr, const SnmpParamList& params)
{
	struct snmp_session init_session;

	snmp_sess_init(&init_session);
    init_session.version = SNMP_VERSION_2c;
    init_session.community = strdup(_community);
    init_session.community_len = strlen(init_session.community);

    init_session.peername = strdup(addr.getAddress().toString().c_str());
	init_session.remote_port = addr.getPort();

	_session = snmp_open(init_session);
	if (!_session)
	{
		//SNMP_TRACE("CSnmpTrapSender::Open(): snmp_open failed\n");
		return false;
	}
	
	struct netsnmp_variable_list vblist;
	memset(&vblist, 0, sizeof(vblist));

	// create variable binding entries
	for (SnmpParamList::const_iterator itor = params.begin(); itor != params.end(); ++itor)
	{
		const SnmpParam& param = *itor;

		// bind WinSNMP entities to framework entities
		SmiOidBind smiOid(param.name);
		SmiValueBind smiValue(param.value);

		snmp_set_var_objid(&vblist, &param.name, param.name.size());
		snmp_set_var_value(&vblist, param.value, );
	}

	struct snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_TRAP);
	if (!pdu)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): snmp_pdu_create failed\n");
		return false;
	}
	
	if (snmp_send(_session, pdu) == 0)
	{
		//SNMP_TRACE("SnmpSenderImpl::Send(): snmp_send failed, code=%d\n",
		//	SnmpGetLastError(m_session));
		return false;
	}
	
	snmp_free_pdu(pdu);

	snmp_close(_session);
	_session = NULL;

	
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
		//SNMP_TRACE("SnmpSenderImpl::Send(): SnmpSendMsg failed, code=%d\n",
		//	SnmpGetLastError(m_session));
		return false;
	}

	return true;
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