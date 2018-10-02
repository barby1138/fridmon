// timer.h
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

#ifndef QUARK_TIMER_H_
#define QUARK_TIMER_H_

#include "thread.h"
#include "sync.h"

namespace quark
{

//------------------------------------------------------------------------------

namespace prvt
{

/**
Functor for timer callbacks
*/
struct timeout_func
{
	virtual ~timeout_func() {}
	virtual void operator()(size_t timerId) const = 0;
};

template<class _Host>
struct timeout_funcT : timeout_func
{
	timeout_funcT(_Host* host, void (_Host::*fn)(size_t ))
		:	_host(host), _fn(fn) {}

	virtual void operator()(size_t timerId) const
		{ (_host->*_fn)(timerId); }

private:
	_Host* _host;
	void (_Host::*_fn)(size_t );
};

} // namespace prvt

//------------------------------------------------------------------------------

/** 
Provides timer pool functionality
*/
class timer_pool : private thread
{
private:
	enum { tickCost = 10 };	// 10 milliseconds per one tick

	struct timer_entry : noncopyable
	{
		timer_entry(prvt::timeout_func* observer = 0, size_t tickCount = 0);
		~timer_entry();

		prvt::timeout_func* _observer;
		size_t _tickCount;
		size_t _curTickCount;
	};

	typedef quark::pmap<size_t, timer_entry*> timers_t;
	
public:
	timer_pool();
	virtual ~timer_pool();

	/**
    Register timer
		\param period in milliseconds
		\param pointer to host object which defines timer callback function
		\param function which implements timer callback
		\return timer id
	*/
	template<class _Host>
	size_t registerTimer(_Host* host, void (_Host::*fn)(size_t timerId), size_t delay)
		{ return registerTimer(delay, new prvt::timeout_funcT<_Host>(host, fn)); }
	/**
    Unregister timer
		\param timer id
	*/
	void unregisterTimer(size_t id);

	/**
	Destroy the pool manually, no more timer callbacks
	*/
	void destroy();

private:
	size_t registerTimer(size_t delay, prvt::timeout_func* observer);

	// quark::thread < 
	virtual void run();
	virtual void terminate();
	// >

	void onTick();

private:
	timers_t _timers;
	quark::critical_section _cs;
	quark::semaphore _event;
};

//------------------------------------------------------------------------------

} // namespace quark

#endif // QUARK_TIMER_H_
