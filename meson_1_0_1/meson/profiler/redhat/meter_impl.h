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

#ifndef _REDHAT_MESON_METER_IMPL_H_
#define _REDHAT_MESON_METER_IMPL_H_

using namespace meson;

//////////////////////////////////////////////////////////////////////
// hires_meter
//////////////////////////////////////////////////////////////////////

#include <time.h>

static long long divisor()
{
	static long long divisor = 0LL;
	if (divisor == 0LL)
	{
		struct timespec tmsResolution;

		int nRetVal = clock_getres(_METER_CLOCK, &tmsResolution);
		assert(nRetVal == 0);
		
		divisor = SEC_TO_NANO((long long)tmsResolution.tv_sec) + tmsResolution.tv_nsec;
	}

	assert(divisor != 0LL);

	return divisor;
}

hires_meter::value_type hires_meter::now()
{
	struct timespec tmsCurrentTime;

	int nRetVal = clock_gettime(_METER_CLOCK, &tmsCurrentTime);
	assert(nRetVal == 0);
	
	return static_cast<hires_meter::value_type>(SEC_TO_NANO((long long)tmsCurrentTime.tv_sec) + tmsCurrentTime.tv_nsec) / divisor();
}

quark::pstring hires_meter::toString(value_type value)
{
	char buffer[32];
	::sprintf(buffer, "%.3f", value);
	return buffer;
}

#endif // _REDHAT_MESON_METER_IMPL_H_
