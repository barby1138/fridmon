// gen_attributes.cpp
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

#include "gen_attributes.h"
#include <quark/strings.h>

using namespace hyperon;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// u32_attribute
//////////////////////////////////////////////////////////////////////

const quark::pstring u32_attribute::toString() const
{
	return strings::toString(_value);
}

void u32_attribute::fromString(const quark::pstring& str)
{
	strings::fromString(str, _value);
}

//////////////////////////////////////////////////////////////////////
// str_attribute
//////////////////////////////////////////////////////////////////////

const quark::pstring str_attribute::toString() const
{
	return _value;
}

void str_attribute::fromString(const quark::pstring& str)
{
	_value = str;
}

//////////////////////////////////////////////////////////////////////
// bool_attribute
//////////////////////////////////////////////////////////////////////

const pstring bool_attribute::toString() const 
{
	return _value ? "true" : "false";
};

void bool_attribute::fromString(const pstring& str)
{
	_value = str == "true";
}

//////////////////////////////////////////////////////////////////////
// ip_attribute
//////////////////////////////////////////////////////////////////////

const pstring ip_attribute::toString() const 
{
	pstring res;
	quark::strings::unPackIPAddress(res, _value);

	return res;
};

void ip_attribute::fromString(const pstring& str)
{
	_value = 0;
	quark::strings::packIPAddress(str, &_value);
}
