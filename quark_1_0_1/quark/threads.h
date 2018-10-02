// threads.h
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

////////////////////////////////////////////////////////////////////////////////
// Refactored from the Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_THREADS_H_
#define _QUARK_THREADS_H_

#include "sync.h"

namespace quark
{

//////////////////////////////////////////////////
// class template single_threaded
//
/*! Implements a single-threaded model; no synchronization
	Implementation of the _ThreadingModel policy used by various classes
*/
template<class _Host>
class single_threaded
{
public:
	struct lock
	{
		lock() {}
		explicit lock(const single_threaded&) {}
	};
	
public:
	static int atomicIncrement(volatile int& lval)
		{ return ++lval; }
	
	static int atomicDecrement(volatile int& lval)
		{ return --lval; }
	
	static int atomicExchange(volatile int& lval, int val)
		{ int tmp = lval; lval = val; return tmp; }
};

//////////////////////////////////////////////////
// class template object_level_lockable
//
/*! Implements a object-level locking scheme
	Implementation of the _ThreadingModel policy used by various classes
*/
template<class _Host>
class object_level_lockable
{
public:
	class lock;
	friend class lock;

	class lock : public noncopyable
	{
	private:
		object_level_lockable& _host;
    	lock(); // buggy design

	public:
		explicit lock(object_level_lockable& host)
			:	_host(host) { _host._mtx.lock(); }

		~lock()
			{ _host._mtx.unlock(); }
	};

public:
	static int atomicIncrement(volatile int& lval)
		{ return interlocked::increment(lval); }
	
	static int atomicDecrement(volatile int& lval)
		{ return interlocked::decrement(lval); }
	
	static int atomicExchange(volatile int& lval, int val)
		{ return interlocked::exchange(lval, val); }

private:
	critical_section _mtx;
};

//////////////////////////////////////////////////
// class template class_level_lockable
//
/*! Implements a class-level locking scheme
	Implementation of the _ThreadingModel policy used by various classes
*/ 
template<class _Host>
class class_level_lockable
{	
public:
	class lock;
	friend class lock;

	class lock : public noncopyable
	{
	public:
		lock()
			{ _mtx.lock(); }
		explicit lock(class_level_lockable&)
			{ _mtx.lock(); }

		~lock()
			{ _mtx.unlock(); }
	};

public:
	static int atomicIncrement(volatile int& lval)
		{ return interlocked::increment(lval); }
	
	static int atomicDecrement(volatile int& lval)
		{ return interlocked::decrement(lval); }
	
	static int atomicExchange(volatile int& lval, int val)
		{ return interlocked::exchange(lval, val); }

private:
	static critical_section _mtx;
};			

template<class _Host>
critical_section class_level_lockable<_Host>::_mtx;

} // namespace quark

#endif // _QUARK_THREADS_H_
