// SnmpInterfaces.h: Snmp Framework interfaces.
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

#ifndef SNMPFWK_SNMPINTERFACES_INC_
#define SNMPFWK_SNMPINTERFACES_INC_

#include "SnmpTypes.h"

namespace snmpfwk
{

//////////////////////////////////////////////////
// interface ISnmpObject
//
struct ISnmpObject
{
	virtual ~ISnmpObject() {}

	virtual const Oid& getOid() const = 0;
	virtual u32 query(u8 request, SnmpParam& param) = 0;
};

} // namespace snmpfwk

#endif // SNMPFWK_SNMPINTERFACES_INC_
