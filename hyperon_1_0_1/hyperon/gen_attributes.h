// gen_attributes.h
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

#ifndef _HYPERON_GEN_ATTRIBUTES_H_
#define _HYPERON_GEN_ATTRIBUTES_H_

#include "attribute.h"

namespace hyperon
{

//////////////////////////////////////////////////
// class u32_attribute
//
class u32_attribute : public attributeT<quark::u32>
{
public:
	explicit u32_attribute(const quark::pstring& name)
		:	attributeT<quark::u32>(name) {}

	explicit u32_attribute(const quark::pstring& name, const quark::u32& value)
		:	attributeT<quark::u32>(name, value) {}

protected:
	virtual const quark::pstring toString() const;
	virtual void fromString(const quark::pstring& str);
};

//////////////////////////////////////////////////
// class str_attribute
//
class str_attribute : public attributeT<quark::pstring>
{
public:
	explicit str_attribute(const quark::pstring& name)
		:	attributeT<quark::pstring>(name) {}

	explicit str_attribute(const quark::pstring& name, const quark::pstring& value)
		:	attributeT<quark::pstring>(name, value) {}

protected:
	virtual const quark::pstring toString() const;
	virtual void fromString(const quark::pstring& str);
};


//////////////////////////////////////////////////
// class bool_attribute
//
class bool_attribute : public attributeT<bool> 
{
public:
	explicit bool_attribute(const char* name) 
		:	attributeT<bool>(name) {}
	
	explicit bool_attribute(const quark::pstring& name, const bool& value)
		:	attributeT<bool>(name, value) {}

protected:
	virtual const quark::pstring toString() const; // throw(std::exception)
	virtual void fromString(const quark::pstring& ); // throw(std::exception)
};

//////////////////////////////////////////////////
// class ip_attribute
//
class ip_attribute : public u32_attribute
{
public:
	explicit ip_attribute(const char* name) 
		:	u32_attribute(name) 
	{}
		
protected:
	virtual const quark::pstring toString() const; // throw(std::exception)
	virtual void fromString(const quark::pstring& ); // throw(std::exception)
};

} // namespace hyperon

#endif // _HYPERON_GEN_ATTRIBUTES_H_
