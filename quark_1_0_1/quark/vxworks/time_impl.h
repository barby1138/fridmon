// time_impl.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Quark Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _VXWORKS_QUARK_TIME_IMPL_H_
#define _VXWORKS_QUARK_TIME_IMPL_H_

#include <quark/mempool/service/service_mempool.h>

// Platform-specific headers
#include "time.h"
#include "sysLib.h"
#include "tickLib.h"

namespace quark
{

//////////////////////////////////////////////////
// class time::time_impl
//	
class time::time_impl : public pimple_object
{
public:
	static u32 getTickCount();
	static long timezone();
	static u32 getMilliseconds();
	static struct tm* getLocalTime(time_t time);
	static struct tm* getGmTime(time_t time);
};

//////////////////////////////////////////////////////////////////////
// time_impl
//////////////////////////////////////////////////////////////////////

u32 time::time_impl::getTickCount()
{
	// return milliseconds from start
	return (::tickGet() * 1000) / sysClkRateGet();
}

// Returns millisecond part of the time [0..999]
u32 time::time_impl::getMilliseconds() 
{ 
	return (getTickCount() % 1000); 
}

struct tm* time::time_impl::getLocalTime(time_t time) 
{ 
	return ::localtime(&time); 
}

struct tm* time::time_impl::getGmTime(time_t time)
{ 
	return ::gmtime(&time); 
}

long time::time_impl::timezone() 
{ 
	// TODO: implement
	return 0; 
}

} // namespace quark

#endif // _VXWORKS_QUARK_TIME_IMPL_H_
