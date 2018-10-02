// meter.cpp
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

#ifndef _WIN32_MESON_METER_IMPL_H_
#define _WIN32_MESON_METER_IMPL_H_

using namespace meson;

//////////////////////////////////////////////////////////////////////
// hires_meter
//////////////////////////////////////////////////////////////////////

#include <windows.h>

#define _COUNT_VALUE	1000

static LONGLONG divisor()
{
	static LONGLONG divisor = 0;
	if (divisor == 0)
	{
		LARGE_INTEGER freq;
		if (::QueryPerformanceFrequency(&freq))
		{
			divisor = freq.QuadPart;
		}
	}
	assert(divisor != 0);
	return divisor;
}

hires_meter::value_type hires_meter::now()
{
	LARGE_INTEGER count;
	::QueryPerformanceCounter(&count);
	return static_cast<hires_meter::value_type>(count.QuadPart) / divisor() * _COUNT_VALUE;
}

quark::pstring hires_meter::toString(value_type value)
{
	char buffer[32];
	::sprintf(buffer, "%.3f", value);
	return buffer;
}

#endif // _WIN32_MESON_METER_IMPL_H_