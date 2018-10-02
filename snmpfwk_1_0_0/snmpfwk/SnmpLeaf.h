// SnmpLeaf.h: interface for the SnmpLeaf class.
//
// TODO: Consider refactoring
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

#ifndef SNMPFWK_SNMPLEAF_INC_
#define SNMPFWK_SNMPLEAF_INC_

#include "SnmpUserTypes.h"
#include "SnmpInterfaces.h"

namespace snmpfwk
{

//////////////////////////////////////////////////
// class SnmpAnyVisitorHelper
//
struct SnmpAnyVisitorHelper : protected ISnmpAnyVisitor
{
protected:
	bool check(const SnmpAny& any) const
	{
		m_visited = true;
		any.accept(const_cast<SnmpAnyVisitorHelper&>(*this));
		return m_visited;
	}

	virtual void visit(const s32& val) { m_visited = false; }
	virtual void visit(const u32& val) { m_visited = false; }
	virtual void visit(const pstring& val) { m_visited = false; }
	virtual void visit(const Oid& val) { m_visited = false; }
	virtual void visit(const u8& val) { m_visited = false; }

private:
	mutable bool m_visited;
};

//////////////////////////////////////////////////
// class template SnmpTypeBinder
//
template<class _T>
class SnmpTypeBinder : private SnmpAnyVisitorHelper
{
public:
	typedef typename _T::UserType UserType_t;
	typedef typename _T::AsnType asn_type_t;
	typedef typename asn_type_t::value_type value_type_t;

public:
	bool checkType(const SnmpAny& any) const
	{
		return check(any);
	}

	bool depack(const SnmpAny& any, UserType_t& userValue) const
	{
		any.accept(const_cast<SnmpTypeBinder&>(*this));
		return _T::toUser(m_visitedValue, userValue);
	}

	bool pack(SnmpAny& any, const UserType_t& userValue) const
	{
		value_type_t asnValue;
		if (!_T::toAsn(userValue, asnValue))
			return false;
		any = asn_type_t(asnValue);
		return true;
	}

private:
	virtual void visit(const value_type_t& val) 
		{ m_visitedValue = val; }

protected:
	mutable value_type_t m_visitedValue;
};

//////////////////////////////////////////////////
// function template depackAny
//
template<typename _T>
bool depackAny(_T dummy, SnmpAny& any, typename _T::UserType& value)
{
	SnmpTypeBinder<_T> binder;
	if (!binder.checkType(any))
		return false;
	return binder.depack(any, value);
}

//////////////////////////////////////////////////
// class template SnmpStaticBinder
//
template<class _Host, typename _Type>
struct SnmpStaticBinder : protected SnmpTypeBinder<_Type>
{
	typedef typename _Type::UserType UserType_t;
	typedef bool (_Host::*GetFn)(UserType_t& );
	typedef bool (_Host::*SetFn)(const UserType_t& );

	SnmpStaticBinder(const Oid& oid)
		:	_oid(oid) {}

	const Oid& getOid() const 
		{ return _oid; }

	bool checkName(const Oid& name) const
		{ return _oid == name; }

	template<class _H, typename Fn>
	bool get(_H& host, Fn& fn, SnmpParam& param) const
	{
		UserType_t var;
		return (host.*fn)(var) && pack(param.value, var);
	}

	template<class _H, typename Fn>
	bool set(_H& host, Fn& fn, const SnmpParam& param) const
	{
		UserType_t var;
        return depack(param.value, var) && (host.*fn)(var);
	}
	
private:
	Oid _oid;
};

//////////////////////////////////////////////////
// class template SnmpColumnBinder
//
template<class _Host, typename _Type, class _Index>
struct SnmpColumnBinder : protected SnmpTypeBinder<_Type>
{
	typedef typename _Type::UserType UserType_t;
	typedef bool (_Host::*GetFn)(const _Index& index, UserType_t& );
	typedef bool (_Host::*SetFn)(const _Index& index, const UserType_t& );

	SnmpColumnBinder(const Oid& oid)
		:	_oid(oid) {}

	const Oid& getOid() const 
		{ return _oid; }

	bool checkName(const Oid& name) const
		{ return _oid == name; }

	template<class _H, typename Fn>
	bool get(_H& host, Fn& fn, SnmpParam& param) const
	{
		UserType_t var;
		return (host.*fn)(getIndex(param.name), var) && pack(param.value, var);
	}

	template<class _H, typename Fn>
	bool set(_H& host, Fn& fn, SnmpParam& param)
	{
		UserType_t var;
		return depack(param.value, var) && (host.*fn)(getIndex(param.name), var);
	}

private:
	const _Index getIndex(const Oid& name) const
	{
		if (name.size() < _Index::size)
			assert(false);
		return _Index::fromOid(name.suboid(name.size() - _Index::size));
	}

private:
	Oid _oid;
};

//////////////////////////////////////////////////
// interface ISnmpLeaf
//
struct ISnmpLeaf : public ISnmpObject
{
	virtual ISnmpLeaf* clone(const Oid& oid) const = 0;
};

//////////////////////////////////////////////////
// class template SnmpLeaf
//
template
<
	class _Host, 
	typename _Type, 
	class _BindPolicy = SnmpStaticBinder<_Host, _Type> 
>
class SnmpLeaf : public ISnmpLeaf,
				 protected _BindPolicy
{
public:
	typedef typename _BindPolicy::GetFn GetFn;
	typedef typename _BindPolicy::SetFn SetFn;

public:
	SnmpLeaf(_Host& host, const Oid& oid, GetFn getFn, SetFn setFn);

	virtual const Oid& getOid() const { return _BindPolicy::getOid(); }
	virtual u32 query(u8 request, SnmpParam& param);
	virtual ISnmpLeaf* clone(const Oid& oid) const;

private:
	u32 get(SnmpParam& param);
	u32 set(SnmpParam& param);
	u32 getNext(SnmpParam& param);

private:
	_Host& _host;
	GetFn _getFn;
	SetFn _setFn;
};

template<class _Host, typename _Type, class _BindPolicy>
SnmpLeaf<_Host, _Type, _BindPolicy>::SnmpLeaf(_Host& host, const Oid& oid, GetFn getFn, SetFn setFn)
	:	_BindPolicy(oid),
		_host(host),
		_getFn(getFn),
		_setFn(setFn)
{
}

template<class _Host, typename _Type, class _BindPolicy>
u32 SnmpLeaf<_Host, _Type, _BindPolicy>::query(u8 request, SnmpParam& param)
{
	//TRACE_DEBUG("SnmpLeaf::query(): pdu: %d, name: %s, name requsted: %s", 
	//	request, getOid().toString().c_str(), param.name.toString().c_str());
	
	// perform action
	try
	{
		if (request == pduGet && _getFn != 0)
			return get(param);
		else if (request == pduSet && _setFn != 0)
			return set(param);
		else if (request == pduGetNext)
			return getNext(param);
		else
			return errNoSuchName;
	}
	catch (std::exception& )
	{
		return errGenError;
	}
}

template<class _Host, typename _Type, class _BindPolicy>
ISnmpLeaf* SnmpLeaf<_Host, _Type, _BindPolicy>::clone(const Oid& oid) const
{ 
	return new SnmpLeaf(_host, oid, _getFn, _setFn);
}

template<class _Host, typename _Type, class _BindPolicy>
u32 SnmpLeaf<_Host, _Type, _BindPolicy>::get(SnmpParam& param)
{
	// check oid
	if (!checkName(param.name))
		return errNoSuchName;
	// get value
	return _BindPolicy::get(_host, _getFn, param) ? errNoError : errGenError;
}

template<class _Host, typename _Type, class _BindPolicy>
u32 SnmpLeaf<_Host, _Type, _BindPolicy>::set(SnmpParam& param)
{
	// check oid
	if (!checkName(param.name))
		return errNoSuchName;
	// check type
	if (!checkType(param.value)) 
		return errWrongType;
	// set value
	return _BindPolicy::set(_host, _setFn, param) ? errNoError : errGenError;
}

template<class _Host, typename _Type, class _BindPolicy>
u32 SnmpLeaf<_Host, _Type, _BindPolicy>::getNext(SnmpParam& param)
{
	// if provided oid is less
	if (param.name < _BindPolicy::getOid() && _getFn != 0)
	{
		param.name = _BindPolicy::getOid();
		return get(param);
	} 
	return errNoSuchName;
}

} // namespace snmpfwk

#endif // SNMPFWK_SNMPLEAF_INC_
