// attribute.h
//
////////////////////////////////////////////////////////////////////////////////

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

#ifndef _HYPERON_ATTRIBUTE_H_
#define _HYPERON_ATTRIBUTE_H_

#include "any.h"
#include "dom.h"

namespace hyperon
{

//////////////////////////////////////////////////
// class attr_exception
//
class attr_exception : public quark::pruntime_error
{
public:
	attr_exception(const char* name, const char* descr);
};

//////////////////////////////////////////////////
// class attribute
//
class attribute : public quark::pooled_object
{
public:
	virtual ~attribute() {}

	const char* getName() const 
		{ return _name.c_str(); }

	virtual const any getValue() const = 0;
	virtual void setValue(const any& val) = 0;

	virtual dom_attribute* toStorage(dom_document* ) const = 0;
	virtual void fromStorage(dom_attribute* ) = 0;

protected:
	explicit attribute(const quark::pstring& name)
		:	_name(name) {}

	virtual const quark::pstring toString() const = 0;
	virtual void fromString(const quark::pstring& ) = 0;

private:
	quark::pstring _name;
};

//////////////////////////////////////////////////
// class template attributeT
//
template<typename _T>
class attributeT : public attribute
{
public:
	virtual const any getValue() const;
	virtual void setValue(const any& val);

	virtual dom_attribute* toStorage(dom_document* domDoc) const;
	virtual void fromStorage(dom_attribute* domAttr);

protected:
	explicit attributeT(const quark::pstring& name)
		:	attribute(name), _value() {}

	explicit attributeT(const quark::pstring& name, const _T& val)
		:	attribute(name), _value(val) {}

protected:
	_T _value;

private:
	quark::pstring _name;
};

template<typename _T>
const any attributeT<_T>::getValue() const
{
	return any(_value);
}

template<typename _T>
void attributeT<_T>::setValue(const any& val)
{
	try
	{
		_value = any_cast<_T>(val);
	}
	catch (const std::exception& )
	{
		throw attr_exception(getName(), "method setValue(): attribute type mismatch");
	}
}

template<typename _T>
dom_attribute* attributeT<_T>::toStorage(dom_document* domDoc) const
{
	try
	{
		// getting value as string
		return domDoc->createAttribute(getName(), toString());
	}
	catch (const std::exception& ex)
	{
		throw attr_exception(getName(), ex.what());
	}
}

template<typename _T>
void attributeT<_T>::fromStorage(dom_attribute* domAttr)

{
	try
	{
		// updating value from string
		fromString(domAttr->getValue());
	}
	catch(const std::exception& ex)
	{
		throw attr_exception(getName(), ex.what());
	}
}
} // namespace hyperon

#endif // _HYPERON_ATTRIBUTE_H_
