// SnmpTextCon.cpp: implementation of the SNMPv2 Textual Convention entities.
//
//////////////////////////////////////////////////////////////////////

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

#include "SnmpTextCon.h"
#include <quark/strings.h>
#include <quark/time.h>
#include <nucleon/bstream.h>

using namespace snmpfwk;
using namespace quark;
using namespace nucleon;

typedef fixed_streambuf<char> bstreambuf_t;
typedef bstream<bstreambuf_t> bstrea_t;

//////////////////////////////////////////////////////////////////////
// DateAndTime Construction/Destruction
//////////////////////////////////////////////////////////////////////

DateAndTime::DateAndTime()
	:	_year(0),
		_month(1),
		_day(1),
		_hour(0),
		_minutes(0),
		_seconds(0),
		_deciseconds(0),
		_direction('+'),
		_utcHours(0),
		_utcMinutes(0)
{
}

//////////////////////////////////////////////////////////////////////
// DateAndTime Methods
//////////////////////////////////////////////////////////////////////

const SnmpAsnOctets DateAndTime::toOctets() const
{
	char buffer[maxSize] = "";
	bstreambuf_t streambuf(buffer, sizeof(buffer));
	bstrea_t stream(streambuf);

	stream << _year << _month << _day << _hour << _minutes << 
		_seconds << _deciseconds << _direction << _utcHours << _utcMinutes;
	
	return SnmpAsnOctets(pstring(buffer, sizeof(buffer)));
}

const pstring DateAndTime::toString() const
{
	return strings::format("%d-%d-%d,%d:%d:%d.%d,%c%d:%d",
		_year, _month, _day, _hour, _minutes, _seconds, _deciseconds,
		_direction, _utcHours, _utcMinutes);
}

bool DateAndTime::fromString(const pstring& str)
{
	pstring instr = str;

	//////////////////////////////////////////////////////////////////////////
	pstring::size_type pos = instr.find('-');
	if (pos == pstring::npos)
		return false;

	u32 value;

	pstring subs = instr.substr(0,pos);

	strings::fromString(subs,value);
	_year = value;

	instr = instr.substr(++pos, pstring::npos);

	//////////////////////////////////////////////////////////////////////////
	pos = instr.find('-');
	if (pos == pstring::npos)
		return false;

	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_month = value;
	instr = instr.substr(++pos, pstring::npos);
	
	//////////////////////////////////////////////////////////////////////////
	pos = instr.find(',');
	if (pos == pstring::npos)
		return false;

	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_day = value;
	
	instr = instr.substr(++pos, pstring::npos);

	//////////////////////////////////////////////////////////////////////////
	pos = instr.find(':');
	if (pos == pstring::npos)
		return false;
	
	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_hour = value;

	instr = instr.substr(++pos, pstring::npos);
	
	//////////////////////////////////////////////////////////////////////////
	pos = instr.find(':');
	if (pos == pstring::npos)
		return false;
	
	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_minutes = value;

	instr = instr.substr(++pos, pstring::npos);
	
	//////////////////////////////////////////////////////////////////////////
	pos = instr.find('.');
	if (pos == pstring::npos)
		return false;
	
	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_seconds = value;
	instr = instr.substr(++pos, pstring::npos);
	
	//////////////////////////////////////////////////////////////////////////
	pos = instr.find(',');
	if (pos == pstring::npos)
		return false;
	
	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_deciseconds = value;
	instr = instr.substr(++pos, pstring::npos);

	//////////////////////////////////////////////////////////////////////////
	if (instr.at(0) == '+')
		_direction = '+';
	else if (instr.at(0) == '-')
		_direction = '-';
	else
		return false;

	instr = instr.substr(1, pstring::npos);

	//////////////////////////////////////////////////////////////////////////
	pos = instr.find(':');
	if (pos == pstring::npos)
		return false;
	
	subs = instr.substr(0,pos);
	strings::fromString(subs,value);
	_utcHours = value;
	instr = instr.substr(++pos, pstring::npos);

	//////////////////////////////////////////////////////////////////////////
	strings::fromString(subs,value);
	_utcMinutes;
	
	//////////////////////////////////////////////////////////////////////////
	return true;
}

// TODO: move to the platform specific place
const DateAndTime DateAndTime::now()
{
	DateAndTime res;

	// get current time
	quark::time tm = quark::time::now();

	// get local time
	res._year = tm.getYear();
	res._month = tm.getMonth();
	res._day = tm.getDay();
	res._hour = tm.getHour();
	res._minutes = tm.getMinute();
	res._seconds = tm.getSecond();
	
	long timezone = quark::time::timeZone();

	res._direction = timezone <= 0 ? '+' : '-';
	res._utcHours = ::abs(timezone) / (3600);
	res._utcMinutes = (::abs(timezone) / 60) % 60;
	
	return res;
}
