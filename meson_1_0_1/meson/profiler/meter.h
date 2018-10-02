// meter.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Meson Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESON_METER_H_
#define _MESON_METER_H_

#include <quark/time.h>
#include <quark/strings.h>

namespace meson
{

///////////////////////////////////////////////////////////
// class simple_meter
//
class simple_meter
{
public:
	typedef quark::u32 value_type;

public:
	static value_type now()
	{
		return quark::time::getTickCount();
	}
	
	static const quark::pstring toString(value_type value)
	{
		return quark::strings::toString(value);
	}

	static value_type getMin() { return 0; }
	static value_type getMax() { return U32_MAX; }
	static const char* getName() { return "Low-Resolution Meter"; }
	static const char* getUnitType() { return "ms"; }
	
private:
	simple_meter();
};

///////////////////////////////////////////////////////////
// class hires_meter
//
class hires_meter
{
public:
	typedef double value_type;

public:
	static value_type now();
	static quark::pstring toString(value_type value);

	static value_type getMin() { return 0; }
	static value_type getMax() { return F64_MAX; }
	static const char* getName() { return "High-Resolution Timer"; }
	static const char* getUnitType() { return "ms"; }
};

} // namespace meson

#endif // _MESON_METER_H_
