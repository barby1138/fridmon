// profiler.h
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

#ifndef _MESON_PROFILER_H_
#define _MESON_PROFILER_H_

#include "meter.h"
#include <quark/threads.h>
#include <quark/strings.h>

namespace meson
{

///////////////////////////////////////////////////////////
// class template profiler_entry
//
template<class _Meter>
struct profiler_entry
{	
	typedef typename _Meter::value_type value_type;

	profiler_entry()
		:	_count(0),
			_total(_Meter::getMin()),
			_min(_Meter::getMax()),
			_max(_Meter::getMin())
	{
	}

	size_t _count;
	value_type _total;
	value_type _min;
	value_type _max;
};

///////////////////////////////////////////////////////////
// class template profiler
//
template<class _Meter>
class profiler : public quark::object_level_lockable<profiler<_Meter> >,
			     public quark::noncopyable
{
public:
	typedef _Meter meter_type;

private:
	typedef typename _Meter::value_type value_type;
	typedef profiler<_Meter> profiler_type;
	typedef typename profiler<_Meter>::lock lock_type;
	typedef profiler_entry<_Meter> entry_type;
	typedef quark::pmap<quark::pstring, entry_type> entries_t;
	typedef typename entries_t::const_iterator entries_t_const_iterator;

public:
	value_type start()
	{
		return _Meter::now();
	}

	void finish(const quark::pstring& name, const value_type& startTime)
	{
		lock_type quard(*this);
		
		value_type ticks = _Meter::now() - startTime;
		entry_type& entry = _entries[name];
		entry._total += ticks;
		if (entry._min > ticks) entry._min = ticks;
		if (entry._max < ticks) entry._max = ticks;
		++entry._count;
	}

	const quark::pstring dump();

	void reset()
	{
		lock_type quard(*this);
		_entries.clear();
	}

private:
	entries_t _entries;
};

template<class _Meter>
const quark::pstring profiler<_Meter>::dump()
{
	lock_type quard(*this);
	
	quark::pstring strDump = "Profiling dump, used: ";
	strDump += _Meter::getName();
	strDump += ", units: ";
	strDump += _Meter::getUnitType();
	strDump += "\n";
	strDump +=  "--------------------------------------------------------------------------\n"\
				"Name                 Count       Total     Average         Min         Max\n"\
				"------------------- ------ ----------- ----------- ----------- -----------\n";
	
	entries_t_const_iterator itor = _entries.begin();
	entries_t_const_iterator endtor = _entries.end();
	for (; itor != endtor; ++itor)
	{
		const entry_type& entry = itor->second;
		if (entry._count > 0)
		{
			strDump += quark::strings::format("%-19s %6d %11s %11s %11s %11s\n",
				itor->first.c_str(),
				entry._count,
				_Meter::toString(entry._total).c_str(),
				_Meter::toString(entry._total / (static_cast<meson::simple_meter::value_type> (entry._count))).c_str(),
				_Meter::toString(entry._min).c_str(),
				_Meter::toString(entry._max).c_str());
			
		}
	}

	strDump += "--------------------------------------------------------------------------\n";
	return strDump;
}

///////////////////////////////////////////////////////////
// class bench_scope
//
template<class _Profiler>
class bench_scope : public quark::noncopyable
{
public:
	typedef typename _Profiler::holdee_type profiler_type;

public:
	bench_scope(const quark::pstring& name)
		:	_name(name)
	{
		_startTime = _Profiler::instance().start();
	}

	bench_scope(const char* name)
		:	_name(name)
	{
		_startTime = _Profiler::instance().start();
	}

	~bench_scope()
	{
		_Profiler::instance().finish(_name, _startTime);
	}

private:
	quark::pstring _name;
	typename profiler_type::meter_type::value_type _startTime;
};

} // namespace meson

#endif // _MESON_PROFILER_H_
