// sync_impl.h
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

#ifndef _VXWORKS_QUARK_SYNC_IMPL_H_
#define _VXWORKS_QUARK_SYNC_IMPL_H_

#include <quark/mempool/service/service_mempool.h>

// Platform-specific headers
namespace sys {
#include <semLib.h> // resolves name conflict
#include <sysLib.h>
#include <taskLib.h>
}

namespace quark
{

//////////////////////////////////////////////////
// class critical_section::critical_section_impl
//
class critical_section::critical_section_impl : public pimple_object
{
public:
	critical_section_impl()
	{
		_semID = sys::semMCreate(0);
	}
	
	~critical_section_impl()
	{
		sys::semDelete(_semID);
	}
	
	void lock()
	{
		sys::semTake(_semID, WAIT_FOREVER);
	}
	
	void unlock()
	{
		sys::semGive(_semID);		
	}
	
private:
	sys::SEM_ID _semID;
};

//////////////////////////////////////////////////
// class semaphore::semaphore_impl
//
class semaphore::semaphore_impl : public pimple_object
{
public:
	semaphore_impl(int initCount, const char* name)
	{
		_semID = sys::semCCreate(0, initCount);
	}
	
	~semaphore_impl()
	{
		sys::semDelete(_semID);
	}
	
	bool aquire(int msInterval)
	{
		// semTake accepts timeout in ticks!
		int ticks = (0 >= msInterval) ? WAIT_FOREVER : msInterval * sys::sysClkRateGet() / 1000;
		return sys::semTake(_semID, ticks) == OK;
	}
	
	void release()
	{
		sys::semGive(_semID);
	}
	
	bool isInitialized() const
	{
		return (NULL != _semID);
	}
	
private:
	sys::SEM_ID _semID;
};

struct task_lock : noncopyable
{
	task_lock() { sys::taskLock(); }
	~task_lock() { sys::taskUnlock(); }
};

// TODO: check the solution
inline int atomicIncrement(volatile int& lval)
{
	task_lock guard;
	++lval;
	return lval;
}

inline int atomicDecrement(volatile int& lval)
{
	task_lock guard;
	--lval;
	return lval;
}

inline int atomicExchange(volatile int& lval, int val)
{
	task_lock guard;
	int tmp = val;
	val = lval;
	lval = tmp;
	return val;
}

} // namespace quark

#endif // _VXWORKS_QUARK_SYNC_IMPL_H_
