// SnmpTypes.h: definitions of the SNMP types
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

#ifndef SNMPFWK_SNMPTYPES_INC_
#define SNMPFWK_SNMPTYPES_INC_

#include <quark/config.h>

namespace snmpfwk
{

using quark::u32;
using quark::s32;
using quark::u16;
using quark::u8;
using quark::pstring;

//////////////////////////////////////////////////
// enum EAsnType
//
enum EAsnType
{
	// Universal
	asnInteger32	= 0x02,
	asnBitString	= 0x03,
	asnOctetString	= 0x04,
	asnNull			= 0x05,
	asnObjectName	= 0x06,

	// Application
	asnIpAddress	= 0x40,
	asnCounter32	= 0x41,
	asnGauge32		= 0x42,
	asnTimeTicks	= 0x43,
	asnOpaque		= 0x44,
	asnUnsigned32	= 0x47
};

//////////////////////////////////////////////////
// enum EPduType
//
enum EPduType
{
	pduGet			= 0xA0,
	pduGetNext		= 0xA1,
	pduSet			= 0xA3
};

//////////////////////////////////////////////////
// enum EErrorType
//
enum EErrorType
{
	// SNMPv1 errors 
	errNoError				= 0,
	errTooBig				= 1,
	errNoSuchName			= 2,
	errBadValue				= 3,
	errReadOnly				= 4,
	errGenError				= 5,

	// SNMPv2 errors 
	errNoAccess				= 6,
	errWrongType			= 7,
	errWrongLength			= 8,
	errWrongEncoding		= 9,
	errWringValue			= 10,
	errNoCreation			= 11,
	errInconsistentValue	= 12,
	errResourceUnavailable	= 13,
	errCommitFailed			= 14,
	errUndoFailed			= 15,
	errAuthorizationError	= 16,
	errNotWritable			= 17,
	errInconsistentName		= 18
};

//////////////////////////////////////////////////
// class IpAddress
//
class IpAddress
{
public:
	enum { ipSize = sizeof(u32) };

public:
	explicit IpAddress(u32 ip = 0);

/*	template<typename _T>
	explicit IpAddress(const _T* stream)
		:	_data(ipSize)
	{
		fromStream(stream);
	}*/

	template<typename _T>
	void toStream(_T* stream) const
	{
		std::copy(_data.begin(), _data.end(), stream);
	}
	template<typename _T>
	static const IpAddress fromStream(const _T* stream)
	{
		IpAddress res;
		std::copy(stream, stream + ipSize, res._data.begin());
		return res;
	}

	u32 getValue() const;
	void setValue(u32 val);
	const u8* getData() const { return &_data[0]; }

	const pstring toString() const;
	static const IpAddress fromString(const pstring& str);

	bool operator<(const IpAddress& rhs) const 
		{ return _data < rhs._data; }
	bool operator>(const IpAddress& rhs) const 
		{ return _data > rhs._data; }
	bool operator==(const IpAddress& rhs) const 
		{ return _data == rhs._data; }
	bool operator!=(const IpAddress& rhs) const 
		{ return _data != rhs._data; }

private:
	quark::pvector<u8> _data;
};

//////////////////////////////////////////////////
// class IpEndPoint
//
class IpEndPoint
{
public:
	IpEndPoint();
	IpEndPoint(const IpAddress& addr, u32 port);

	const IpAddress getAddress() const { return _addr; }
	void setAddress(const IpAddress& addr) { _addr = addr; }
	u32 getPort() const { return _port; }
	void setPort(u32 port) { _port = port; }

	const pstring toString() const;
	static const IpEndPoint fromString(const pstring& str);

	bool operator<(const IpEndPoint& rhs) const 
		{ return _addr < rhs._addr; }
	bool operator>(const IpEndPoint& rhs) const 
		{ return _addr > rhs._addr; }

private:
	IpAddress _addr;
	u32 _port;
};

//////////////////////////////////////////////////
// class Oid
//
class Oid
{
private:
	typedef quark::pvector<u32> data_t;

public:
	Oid();
	Oid(const u32* arr, size_t len);
	Oid(u32 id);
	Oid(const data_t& data);
	
	size_t size() const { return _data.size(); }
	bool empty() const { return _data.empty(); }
	u32& at(size_t index) { return _data[index]; }
	u32 at(size_t index) const { return _data[index]; }
	const Oid suboid(size_t pos, size_t count = -1) const;
	bool startsWith(const Oid& rhs) const;
	bool endsWith(const Oid& rhs) const;
	const u32* data() const { return !_data.empty() ? &_data[0] : NULL; }

	bool operator<(const Oid& rhs) const { return _data < rhs._data; }
	bool operator>(const Oid& rhs) const { return _data > rhs._data; }
	bool operator==(const Oid& rhs) const { return _data == rhs._data; }
	bool operator!=(const Oid& rhs) const { return _data != rhs._data; }
	Oid& operator+=(const Oid& rhs);

	const pstring toString() const;
    
private:
	data_t _data;
};

inline const Oid operator+(const Oid& lhs, const Oid& rhs)
{
	Oid res(lhs);
	res += rhs;
	return res;
}

#define MAKE_OID(x) Oid(x, sizeof(x)/sizeof(u32))

//////////////////////////////////////////////////
// interface ISnmpAnyVisitor
//
struct ISnmpAnyVisitor
{
	virtual void visit(const s32& val) = 0;
	virtual void visit(const u32& val) = 0;
	virtual void visit(const pstring& val) = 0;
	virtual void visit(const Oid& val) = 0;
	virtual void visit(const u8& val) = 0;

protected:
	~ISnmpAnyVisitor() {}
};

//////////////////////////////////////////////////
// interface ISnmpAny
//
struct ISnmpAny
{
	virtual EAsnType getType() const = 0;
	virtual void accept(ISnmpAnyVisitor& visitor) const = 0;
	virtual ISnmpAny* clone() const = 0;
	virtual ~ISnmpAny() {}
};

//////////////////////////////////////////////////
// class template SnmpAnyT
//
template<const EAsnType _Type, typename _Val>
class SnmpAnyT : public ISnmpAny
{
public:
	typedef _Val value_type;

public:
	explicit SnmpAnyT(const _Val& val = _Val())
		:	_value(val) {}
	
	const _Val getValue() const
		{ return _value; }
	void setValue(const _Val& val)
		{ _value = val; }

	virtual EAsnType getType() const
		{ return _Type; }
	virtual void accept(ISnmpAnyVisitor& visitor) const
		{ visitor.visit(_value); }
	virtual ISnmpAny* clone() const
		{ return new SnmpAnyT(*this); }

private:
	_Val _value;
};

//////////////////////////////////////////////////
// SNMP ASN types
//
typedef SnmpAnyT<asnInteger32, s32> SnmpAsnInt32;
typedef SnmpAnyT<asnUnsigned32, u32> SnmpAsnUint32;
typedef SnmpAnyT<asnCounter32, u32> SnmpAsnCntr32;
typedef SnmpAnyT<asnGauge32, u32> SnmpAsnGauge32;
typedef SnmpAnyT<asnTimeTicks, u32> SnmpAsnTicks;
typedef SnmpAnyT<asnOctetString, pstring> SnmpAsnOctets;
typedef SnmpAnyT<asnBitString, pstring> SnmpAsnBits;
typedef SnmpAnyT<asnOpaque, pstring> SnmpAsnOpaque;
typedef SnmpAnyT<asnIpAddress, pstring> SnmpAsnIp;
typedef SnmpAnyT<asnObjectName, Oid> SnmpAsnOid;
typedef SnmpAnyT<asnNull, u8> SnmpAsnNull;

//////////////////////////////////////////////////
// class SnmpAny
//
class SnmpAny
{
public:
	SnmpAny()
		:	_pAny(new SnmpAsnNull(0)) {}

	template<class Asn>
	SnmpAny(const Asn& asn)
		:	_pAny(new Asn(asn)) {}

	SnmpAny(const SnmpAny& rhs)
		:	_pAny(rhs._pAny->clone()) {}

	~SnmpAny()
		{ delete _pAny; }

	SnmpAny& operator=(const SnmpAny& rhs)
		{ SnmpAny tmp(rhs); return swap(tmp); }

	EAsnType getType() const 
		{ return _pAny->getType(); }

	void accept(ISnmpAnyVisitor& visitor) const
		{ _pAny->accept(visitor); }

	SnmpAny& swap(SnmpAny& rhs)
	{
		std::swap(_pAny, rhs._pAny);
		return *this;
	}

private:
	ISnmpAny* _pAny;
};

//////////////////////////////////////////////////
// struct SnmpParam
//
struct SnmpParam
{
	SnmpParam() {}

	template<class _Asn>
	SnmpParam(const Oid& oid, const _Asn& val)
		:	name(oid), value(val) {}

	Oid name;
	SnmpAny value;
};

typedef quark::pvector<SnmpParam> SnmpParamList;

} // namespace snmpfwk

#endif // SNMPFWK_SNMPTYPES_INC_
