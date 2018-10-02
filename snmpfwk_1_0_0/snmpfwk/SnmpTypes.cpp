// SnmpTypes.cpp: implementations of the SNMP types.
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

#include "SnmpTypes.h"
#include <quark/strings.h>

using namespace snmpfwk;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// IpAddress Construction/Destruction
//////////////////////////////////////////////////////////////////////

IpAddress::IpAddress(u32 ip)
	:	_data(ipSize)
{
	setValue(ip);
}

//////////////////////////////////////////////////////////////////////
// IpAddress Methods
//////////////////////////////////////////////////////////////////////

#ifdef __LITTLE_ENDIAN__

u32 IpAddress::getValue() const 
{ 
	u32 res;
	std::copy(_data.begin(), _data.end(), (u8*)&res);
	return res;
}

void IpAddress::setValue(u32 val)
{
	std::copy((u8*)&val, (u8*)&val + sizeof(u32), _data.begin());
}

#endif // _LITTLE_ENDIAN

#ifdef __BIG_ENDIAN__

u32 IpAddress::getValue() const 
{ 
	u32 res;
	std::reverse_copy(_data.begin(), _data.end(), (u8*)&res);
	return res;
}

void IpAddress::setValue(u32 val)
{
	std::reverse_copy((u8*)&val, (u8*)&val + sizeof(u32), _data.begin());
}	

#endif // _BIG_ENDIAN	

const pstring IpAddress::toString() const
{
	return strings::format("%d.%d.%d.%d", _data[0], _data[1], _data[2], _data[3]);
}

const IpAddress IpAddress::fromString(const pstring& str)
{
	u32 addr = 0;
	strings::packIPAddress(str, &addr);
	return IpAddress(addr);
}

//////////////////////////////////////////////////////////////////////
// IpEndPoint Construction/Destruction
//////////////////////////////////////////////////////////////////////

IpEndPoint::IpEndPoint()
	:	_port(0)
{
}

IpEndPoint::IpEndPoint(const IpAddress& addr, u32 port)
	:	_addr(addr),
		_port(port)
{
}

//////////////////////////////////////////////////////////////////////
// IpEndPoint Methods
//////////////////////////////////////////////////////////////////////

const pstring IpEndPoint::toString() const
{
	return _addr.toString() + strings::format(":%u", _port);
}

const IpEndPoint IpEndPoint::fromString(const pstring& str)
{
	pstring::size_type pos = str.find(':');
	if (pos == pstring::npos)
		return IpEndPoint(IpAddress(), 0); // return empty
	// parse IP address
	IpAddress address = IpAddress::fromString(str.substr(0, pos));
	// parse port
	u32 port = 0;
	strings::fromString(str.substr(pos + 1), port);
	// create the instance
	return IpEndPoint(address, port);
}

//////////////////////////////////////////////////////////////////////
// Oid Construction/Destruction
//////////////////////////////////////////////////////////////////////

Oid::Oid()
{
}

Oid::Oid(const u32* arr, size_t len)
{
	_data.assign(arr, arr + len);
}

Oid::Oid(u32 id)
	:	_data(1, id)
{
}

Oid::Oid(const data_t& oid)
	:	_data(oid)
{
}
	
//////////////////////////////////////////////////////////////////////
// Oid Methods
//////////////////////////////////////////////////////////////////////

const Oid Oid::suboid(size_t pos, size_t count) const
{
	if (pos >= _data.size())
		return Oid();
	if (count != -1 && pos + count > _data.size())
		return Oid();			

	data_t::const_iterator itor = _data.begin() + pos;
	data_t::const_iterator endtor = (count != -1) ? itor + count : _data.end();
	data_t tmp(itor, endtor);
	return Oid(tmp);
}

bool Oid::startsWith(const Oid& rhs) const
{
	if (_data.size() < rhs._data.size())
		return false;
	return std::equal(rhs._data.begin(), rhs._data.end(), _data.begin());
}

bool Oid::endsWith(const Oid& rhs) const
{
	if (_data.size() < rhs._data.size())
		return false;
	return std::equal(rhs._data.rbegin(), rhs._data.rend(), _data.rbegin());
}

Oid& Oid::operator+=(const Oid& rhs)
{
	std::copy(rhs._data.begin(), rhs._data.end(), std::back_inserter(_data));
	return *this;
}

const pstring Oid::toString() const
{
	pstring res;
	for (data_t::const_iterator itor = _data.begin(); itor != _data.end(); ++itor)
	{
		res += strings::toString(*itor);
		if (itor != _data.end() - 1)
			res += '.';
	}
	return res;
}
