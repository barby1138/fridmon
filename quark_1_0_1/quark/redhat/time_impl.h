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

#ifndef _REDHAT_QUARK_TIME_IMPL_H_
#define _REDHAT_QUARK_TIME_IMPL_H_

#include <quark/mempool/service/service_mempool.h>

// Platform-specific headers
#include <time.h>
#include <sys/times.h>

namespace quark
{
	
//////////////////////////////////////////////////
// class time::time_impl
//	
class time::time_impl : public pimple_object
{
public:
	static u32 getMilliseconds();
	static u32 getTickCount();
	static long timezone();
    static struct tm* getLocalTime(time_t time);
    static struct tm* getGmTime(time_t time);
};

//////////////////////////////////////////////////////////////////////
// time_impl
//////////////////////////////////////////////////////////////////////

inline u32 time::time_impl::getTickCount()
{
	struct timespec ts;
	
	int nRet = clock_gettime(_METER_CLOCK, &ts);
	assert(nRet == 0);
	
	return SEC_TO_MILLI(ts.tv_sec) + NANO_TO_MILLI(ts.tv_nsec);
}

inline long time::time_impl::timezone()
{
	return __timezone;
}

// Returns millisecond part of the time [0..999]
inline u32 time::time_impl::getMilliseconds()
{
	return getTickCount() % 1000;
}

inline struct tm* time::time_impl::getLocalTime(time_t time)
{
	return localtime(&time);
}

inline struct tm* time::time_impl::getGmTime(time_t time)
{
	return gmtime(&time);
}

} // namespace quark

#endif // _REDHAT_QUARK_TIME_IMPL_H_
