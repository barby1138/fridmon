// SnmpUserTypes.cpp: implementations of the user types.
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

#include "SnmpUserTypes.h"

using namespace snmpfwk;

//////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////

static const unsigned long bits [32] = { 
  0x00000001UL, 0x00000002UL, 0x00000004UL,
  0x00000008UL, 0x00000010UL, 0x00000020UL, 
  0x00000040UL, 0x00000080UL, 0x00000100UL, 
  0x00000200UL, 0x00000400UL, 0x00000800UL,
  0x00001000UL, 0x00002000UL, 0x00004000UL,
  0x00008000UL, 0x00010000UL, 0x00020000UL,
  0x00040000UL, 0x00080000UL, 0x00100000UL,
  0x00200000UL, 0x00400000UL, 0x00800000UL,
  0x01000000UL, 0x02000000UL, 0x04000000UL,
  0x08000000UL, 0x10000000UL, 0x20000000UL,
  0x40000000UL, 0x80000000UL};

static u32 reverse_bits(u32 value)
{
	enum { bitcount = sizeof(u32)*8 };
	u32 result = 0;
	for (size_t i = 0; i < bitcount; ++i)
	{
		if ((value & bits[i]) != 0)
			result |= bits[bitcount - i - 1];
	}
	return result;
}

//////////////////////////////////////////////////////////////////////
// Int2Bool Methods
//////////////////////////////////////////////////////////////////////

bool Int2Bool::toUser(s32 asn, bool& user)
{ 
	user = asn == enTrue; 
	return true; 
}

bool Int2Bool::toAsn(bool user, s32& asn)
{ 
	asn = user ? enTrue : enFalse; 
	return true; 
}

//////////////////////////////////////////////////////////////////////
// Str2Ip Methods
//////////////////////////////////////////////////////////////////////

bool Str2Ip::toUser(const pstring& asn, IpAddress& user)
{ 
	if (asn.length() != IpAddress::ipSize)
		return false;
	user = IpAddress::fromStream(asn.c_str());
	return true;
}

bool Str2Ip::toAsn(const IpAddress& user, pstring& asn)
{
	char buffer[IpAddress::ipSize];
	user.toStream(buffer);
	asn.assign(buffer, IpAddress::ipSize);
	return true; 
}

//////////////////////////////////////////////////////////////////////
// Bits2Int Methods
//////////////////////////////////////////////////////////////////////

bool Bits2Int::toUser(const pstring& asn, u32& user)
{
	return false; // not implemented yet
}

bool Bits2Int::toAsn(const u32& user, pstring& asn)
{
	u32 val = reverse_bits(user);
	char buffer[sizeof(u32)] = { val >> 24 , val >> 16, val >> 8,  val};
	asn.assign(buffer, sizeof(u32));
	return true;
}
