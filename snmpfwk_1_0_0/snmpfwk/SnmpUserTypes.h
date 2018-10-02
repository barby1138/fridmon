// SnmpUserTypes.h: definitions of the user types
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

#ifndef SNMPFWK_SNMPUSERTYPES_INC_
#define SNMPFWK_SNMPUSERTYPES_INC_

#include "SnmpTypes.h"

namespace snmpfwk
{

//////////////////////////////////////////////////
// enum ETruthValue
//
enum ETruthValue
{
	enTrue = 1,
	enFalse = 2
};

//////////////////////////////////////////////////
// class template SnmpUserType
//
struct DefaultConvertor;

template
<
	class SnmpAsn, 
	typename UserTypeParam = TEMPLATEPARAM_TYPE SnmpAsn::value_type,
	class Convertor = DefaultConvertor
>
struct SnmpUserType
{
	typedef SnmpAsn AsnType;
	typedef typename AsnType::value_type asn_value_t;
	typedef UserTypeParam UserType;

	static bool toUser(const asn_value_t& asn, UserType& user)
		{ return Convertor::toUser(asn, user); }
	static bool toAsn(const UserType& user, asn_value_t& asn)
		{ return Convertor::toAsn(user, asn); }
};

//////////////////////////////////////////////////
// Converters
//
struct DefaultConvertor
{
	template<typename AsnType, typename UserType>
	static bool toUser(const AsnType& asn, UserType& user)
		{ user = asn; return true; }

	template<typename AsnType, typename UserType>
	static bool toAsn(const UserType& user, AsnType& asn)
		{ asn = user; return true; }
};

struct Int2Bool
{
	static bool toUser(s32 asn, bool& user);
	static bool toAsn(bool user, s32& asn);
};

struct Str2Ip
{
	static bool toUser(const pstring& asn, IpAddress& user);
	static bool toAsn(const IpAddress& user, pstring& asn);
};

struct Bits2Int
{
	static bool toUser(const pstring& asn, u32& user);
	static bool toAsn(const u32& user, pstring& asn);
};

//////////////////////////////////////////////////
// Snmp user types
//
typedef SnmpUserType<SnmpAsnInt32, s32> SnmpInt;
typedef SnmpUserType<SnmpAsnCntr32, u32> SnmpU32;
typedef SnmpUserType<SnmpAsnOctets> SnmpStr;
typedef SnmpUserType<SnmpAsnIp, IpAddress, Str2Ip> SnmpIp;
typedef SnmpUserType<SnmpAsnInt32, bool, Int2Bool> SnmpBool;
typedef SnmpUserType<SnmpAsnOctets, u32, Bits2Int> SnmpBits;

} // namespace snmpfwk

#endif // SNMPFWK_SNMPUSERTYPES_INC_
