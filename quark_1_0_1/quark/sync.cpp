// sync.cpp
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

#include "config.h"
#include "sync.h"
#include "thread.h"
#include IMPL_FILE(sync_impl.h)

using namespace quark;

//////////////////////////////////////////////////////////////////////
// spin_lock (not used in quark, copy to client code if needed)
//////////////////////////////////////////////////////////////////////

/*
spin_lock::spin_lock()
	:	_lock(unlocked)
{
}

void spin_lock::lock()
{
	if (atomicExchange(_lock, locked) != unlocked)
		contendedLock();
}

// TODO: add check for multiprocessor system and hide intel specific
void spin_lock::contendedLock()
{
    int spinCount = 1;
    do 
	{
		if (atomicExchange(_lock, locked) == unlocked)
			return;

		__asm {_emit 0xf3}; __asm {_emit 0x90};

		for (volatile int q = 0; q < spinCount; q++) {}

		spinCount <<= 1;

		if (spinCount > 1024)
		{
			thread::yield();
			spinCount = 1;
		}
    } 
	while (1);
}
*/

//////////////////////////////////////////////////////////////////////
// critical_section
//////////////////////////////////////////////////////////////////////

critical_section::critical_section()
	:	_pImpl(new critical_section_impl)
{
}

critical_section::~critical_section()
{
	delete _pImpl;
	_pImpl = NULL;
}

void critical_section::lock()
{
	_pImpl->lock();
}

void critical_section::unlock()
{		
	_pImpl->unlock();
}

//////////////////////////////////////////////////////////////////////
// semaphore
//////////////////////////////////////////////////////////////////////

semaphore::semaphore(int initCount, const char* name)
	:	_pImpl(new semaphore_impl(initCount, name))
{
	if (!_pImpl->isInitialized())
		throw pruntime_error("semaphore creation failure");
}

semaphore::~semaphore()
{
	delete _pImpl; 
	_pImpl = NULL;
}

void semaphore::release()
{
	_pImpl->release();
}

bool semaphore::aquire(int msInterval)
{
	return _pImpl->aquire(msInterval);
}

//////////////////////////////////////////////////////////////////////
// interlocked
//////////////////////////////////////////////////////////////////////

int interlocked::increment(volatile int& lval)
{
	return atomicIncrement(lval);
}

int interlocked::decrement(volatile int& lval)
{
	return atomicDecrement(lval);
}

int interlocked::exchange(volatile int& lval, int val)
{
	return atomicExchange(lval, val);
}
