// time.cpp
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

#include "time.h"
#include "strings.h"
#include IMPL_FILE(time_impl.h)

using namespace quark;

//////////////////////////////////////////////////////////////////////
// time
//////////////////////////////////////////////////////////////////////

quark::time::time()
:	_tm(0),
	_ms(0)
{
}

quark::time::time(time_t tm, size_t ms)
	:	_tm(tm),
		_ms(ms)
{
}

size_t quark::time::getYear() const
{
	return time_impl::getLocalTime(_tm)->tm_year + 1900;
}

size_t quark::time::getMonth() const
{
	return time_impl::getLocalTime(_tm)->tm_mon + 1;
}

size_t quark::time::getDay() const
{
	return time_impl::getLocalTime(_tm)->tm_mday;
}

size_t quark::time::getHour() const
{
	return time_impl::getLocalTime(_tm)->tm_hour;
}

size_t quark::time::getMinute() const
{
	return time_impl::getLocalTime(_tm)->tm_min;
}

size_t quark::time::getSecond() const
{
	return time_impl::getLocalTime(_tm)->tm_sec;
}

size_t quark::time::getMillisecond() const
{
	return _ms;
}

time_t quark::time::getTimeValue() const
{
	return _tm;
}

const pstring quark::time::toString(const char* format) const
{
	assert(format);

	pstring tmp = format;
	
	// format milliseconds
	pstring ms = quark::strings::format("%03u", _ms);
	
	pstring::size_type pos;
	while ((pos = tmp.find("%t")) != quark::pstring::npos)
		tmp.replace(pos, 2, ms);
	
	// format the rest
	char buffer[256];
	struct tm* localtm = time_impl::getLocalTime(_tm);
	::strftime(buffer, sizeof(buffer), tmp.c_str(), localtm);
	return buffer;	
}

const quark::time quark::time::now()
{
	return time(::time(NULL), time_impl::getMilliseconds());
}

// NOTE: only default format is supported at the moment
const quark::time quark::time::fromString(const char* str, const char* format)
{
	if (!str || strlen(str) == 0)
		return quark::time();

	struct tm t = {0};
	size_t ms = 0;

	if (format == getDefaultFormat())
	{
		if (7 != sscanf(str, "%d-%d-%d %d:%d:%d.%d", 
			&t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec, &ms))
			throw pruntime_error(strings::format("time conversion from string: format '%s' error", format));
	}
	else if (format == getUniversalSortableFormat())
	{
		if (7 != sscanf(str, "%d-%d-%dT%d:%d:%d.%dZ", 
			&t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec, &ms))
			throw pruntime_error(strings::format("time conversion from string: format '%s' error", format));
	}
	else
	{
		throw pruntime_error("time conversion from string: unknown format");
	}

	t.tm_year -= 1900;
	t.tm_mon -= 1;
	t.tm_isdst = -1;
	time_t t2 = ::mktime(&t);
	if (-1 == t2)
		throw pruntime_error("time conversion from string: range error");

	return quark::time(t2, ms);
}

u32 quark::time::getTickCount()
{
	return time_impl::getTickCount();
}

long quark::time::timeZone()
{
	return time_impl::timezone();
}

const char* quark::time::getDefaultFormat()
{
	return "%Y-%m-%d %H:%M:%S.%t";	
}

const char* quark::time::getUniversalSortableFormat()
{
	return "%Y-%m-%dT%H:%M:%S.%tZ";
}
