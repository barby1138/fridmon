// time.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Quark Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_TIME_H_
#define _QUARK_TIME_H_

#include "config.h"
#include <time.h>

namespace quark
{

// TODO: unsigned long long MUST be used
#define SEC_TO_MILLI(x)		(x * 1000UL)
#define SEC_TO_MICRO(x)		(x * 1000000UL)
#define SEC_TO_NANO(x)		(x * 1000000000UL)
#define MILLI_TO_MICRO(x)	(x * 1000UL)
#define MILLI_TO_NANO(x)	(x * 1000000UL)
#define MILLI_TO_SEC(x)		(x / 1000)
#define NANO_TO_MICRO(x)	(x / 1000)
#define NANO_TO_MILLI(x)	(x / 1000000)
#define NANO_TO_SEC(x)		(x / 1000000000)

//////////////////////////////////////////////////
// class time
//
class time : public pooled_object
{
private:
	class time_impl;

public:
	time();
	explicit time(time_t tm, size_t ms = 0);

	size_t getYear() const;
	size_t getMonth() const;
	size_t getDay() const;
	size_t getHour() const;
	size_t getMinute() const;
	size_t getSecond() const;
	size_t getMillisecond() const;
	time_t getTimeValue() const;

	const pstring toString(const char* format = getDefaultFormat()) const;
	
	static const time now();
	// NOTE: only limited formats are supported at the moment
	static const time fromString(const char* str, const char* format = getDefaultFormat());
	static u32 getTickCount();
	static long timeZone();

	friend bool operator==(time lhs, time rhs);
	friend bool operator!=(time lhs, time rhs);
	friend bool operator<(time lhs, time rhs);
	friend bool operator>(time lhs, time rhs);
	friend bool operator<=(time lhs, time rhs);
	friend bool operator>=(time lhs, time rhs);

	// "%Y-%m-%d %H:%M:%S.%t" (e.g. 2007-01-10 12:41:09.264)
	static const char* getDefaultFormat();
	// "%Y-%m-%dT%H:%M:%S.%tZ" (e.g. 2007-01-10T12:41:09.264Z)
	static const char* getUniversalSortableFormat();

private:
	time_t _tm;
	size_t _ms; // millisecond part of the time [0..999]
};

inline bool operator==(time lhs, time rhs)
{
	return lhs._tm == rhs._tm && lhs._ms == rhs._ms;
}

inline bool operator!=(time lhs, time rhs)
{
	return !operator==(lhs, rhs);
}

inline bool operator<(time lhs, time rhs)
{
	if (lhs._tm == rhs._tm)
		return lhs._ms < rhs._ms;
	else
		return lhs._tm < rhs._tm;
}

inline bool operator>(time lhs, time rhs)
{
	if (lhs._tm == rhs._tm)
		return lhs._ms > rhs._ms;
	else
		return lhs._tm > rhs._tm;
}

inline bool operator<=(time lhs, time rhs)
{
	return operator<(lhs, rhs) || operator==(lhs, rhs);
}

inline bool operator>=(time lhs, time rhs)
{
	return operator>(lhs, rhs) || operator==(lhs, rhs);
}

} // namespace quark

#endif // _QUARK_TIME_H_
