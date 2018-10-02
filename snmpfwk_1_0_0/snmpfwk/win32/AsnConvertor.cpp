// AsnConvertor.cpp: implementation of the CAsnConvertor class.
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

#include <win32/AsnConvertor.h>

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// AsnConvertor
//////////////////////////////////////////////////////////////////////

AsnConvertor& AsnConvertor::instance()
{
	static AsnConvertor inst;
	return inst;
}

bool AsnConvertor::convert(const SnmpVarBind& winParam, SnmpParam& portParam)
{
	return convert(winParam.name, portParam.name) && 
		convert(winParam.value, portParam.value);
}

bool AsnConvertor::convert(const SnmpParam& portParam, SnmpVarBind& winParam)
{
	return convert(portParam.name, winParam.name) && 
		convert(portParam.value, winParam.value);
}

bool AsnConvertor::convert(const AsnObjectName& winOid, Oid& portOid)
{
	portOid = Oid((const u32*)winOid.ids, winOid.idLength);
	return true;
}

bool AsnConvertor::convert(const Oid& portOid, AsnObjectName& winOid)
{
	::SnmpUtilOidFree(&winOid);
	winOid.ids = (unsigned int*)::SnmpUtilMemAlloc(portOid.size() * sizeof(u32));
	std::copy(portOid.data(), portOid.data() + portOid.size(), winOid.ids);
	winOid.idLength = portOid.size();
	return true;
}

bool AsnConvertor::convert(const AsnAny& winAny, SnmpAny& portAny)
{
	switch (winAny.asnType)
	{
	case ASN_INTEGER32:
		portAny = SnmpAsnInt32(winAny.asnValue.number);
		break;
	case ASN_UNSIGNED32:
		portAny = SnmpAsnUint32(winAny.asnValue.unsigned32);
		break;
	case ASN_COUNTER32:
		portAny = SnmpAsnCntr32(winAny.asnValue.counter);
		break;
	case ASN_GAUGE32:
		portAny = SnmpAsnGauge32(winAny.asnValue.gauge);
		break;
	case ASN_TIMETICKS:
		portAny = SnmpAsnTicks(winAny.asnValue.ticks);
		break;
	case ASN_OCTETSTRING:
		portAny = SnmpAsnOctets(
			quark::pstring((const char*)winAny.asnValue.string.stream, 
			winAny.asnValue.string.length));
		break;
	case ASN_BITS:
		portAny = SnmpAsnBits(
			quark::pstring((const char*)winAny.asnValue.bits.stream, 
			winAny.asnValue.bits.length));
		break;
	case ASN_OPAQUE:
		portAny = SnmpAsnOpaque(
			quark::pstring((const char*)winAny.asnValue.arbitrary.stream, 
			winAny.asnValue.arbitrary.length));
		break;
	case ASN_IPADDRESS:
		portAny = SnmpAsnIp(
			quark::pstring((const char*)winAny.asnValue.address.stream, 
			winAny.asnValue.address.length));
		break;
	case ASN_OBJECTIDENTIFIER:
		portAny = SnmpAsnOid(
			Oid((const u32*)winAny.asnValue.object.ids, 
			winAny.asnValue.object.idLength));
		break;
	case ASN_NULL:
		portAny = SnmpAsnNull();
		break;
	default:
		return false;
	}
	return true;
}

bool AsnConvertor::convert(const SnmpAny& portAny, AsnAny& winAny)
{
	::SnmpUtilAsnAnyFree(&winAny);
	portAny.accept(*this);
	winAny.asnType = portAny.getType();
	winAny.asnValue = _winAny.asnValue;
	return true;
}

void AsnConvertor::visit(const s32& val)
{
	_winAny.asnValue.number = val;
}

void AsnConvertor::visit(const u32& val)
{
	_winAny.asnValue.unsigned32 = val;
}

void AsnConvertor::visit(const quark::pstring& val)
{
	_winAny.asnValue.string.stream = (u8*)::SnmpUtilMemAlloc(val.length());
	_winAny.asnValue.string.dynamic = TRUE;
	std::copy(val.begin(), val.end(), _winAny.asnValue.string.stream);
	_winAny.asnValue.string.length = val.length();
}

void AsnConvertor::visit(const Oid& val)
{
	_winAny.asnValue.object.ids = (unsigned int*)::SnmpUtilMemAlloc(val.size() * sizeof(u32));
	std::copy(val.data(), val.data() + val.size(), _winAny.asnValue.object.ids);
	_winAny.asnValue.object.idLength = val.size();
}

void AsnConvertor::visit(const u8& val)
{
	_winAny.asnValue.unsigned32 = val;
}
