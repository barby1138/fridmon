// Timer.cpp: implementation of the Timer class.
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

#include "timer.h"

using namespace quark;

//------------------------------------------------------------------------------

timer_pool::timer_entry::timer_entry(prvt::timeout_func* observer, size_t tickCount)
	:	_observer(observer),
		_tickCount(tickCount),
		_curTickCount(tickCount)
{
}

timer_pool::timer_entry::~timer_entry()
{
	delete _observer;
}

//------------------------------------------------------------------------------

timer_pool::timer_pool()
	:	thread("timer pool")
{
	start();
}

timer_pool::~timer_pool()
{
	if (isStarted())
		stop();
}

size_t timer_pool::registerTimer(size_t delay, prvt::timeout_func* observer)
{
	critical_scope<> scope(_cs);
	
	// find first appropriate timer id
	size_t id = 1;
	while (_timers.find(id) != _timers.end()) ++id;

	_timers[id] = new timer_entry(observer, delay / tickCost);
	
	return id;
}

void timer_pool::unregisterTimer(size_t id)
{
	critical_scope<> scope(_cs);

	timers_t::iterator itor = _timers.find(id);
	if (itor == _timers.end())
		return; // illegal timer id
	
	_timers.erase(itor); // remove timer from the map
}

void timer_pool::destroy()
{
	stop();
}

void timer_pool::terminate()
{
	thread::terminate();
	_event.release();
}

void timer_pool::run()
{
	while (!isTerminating())
	{
		if (!_event.aquire(tickCost)) // timeout
			onTick();
	}
}

void timer_pool::onTick()
{
	critical_scope<> scope(_cs);

	timers_t firedTimers;

	timers_t::iterator itor;
	for (itor = _timers.begin(); itor != _timers.end(); ++itor)
	{
		// decrement tick count
		if (--itor->second->_curTickCount == 0)
		{
			// timer period expired
			itor->second->_curTickCount = itor->second->_tickCount;
			firedTimers.insert(*itor);
		}
	}

	for (itor = firedTimers.begin(); itor != firedTimers.end(); ++itor)
	{
		// fire event
		try
		{
			(*itor->second->_observer)(itor->first);
		}
		catch (std::exception& )
		{
			assert(false && "it's bad idea to throw exception from timeout handler");
		}
	}
}

//------------------------------------------------------------------------------
