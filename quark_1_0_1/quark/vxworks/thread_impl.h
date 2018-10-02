// thread_impl.h
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

#ifndef _VXWORKS_QUARK_THREAD_IMPL_H_
#define _VXWORKS_QUARK_THREAD_IMPL_H_

#include <quark/mempool/service/service_mempool.h>
#include <quark/sync.h>

// Platform-specific headers
#include <taskLib.h>
#include <sysLib.h>

namespace quark
{

//////////////////////////////////////////////////
// class thread::thread_impl
//
class thread::thread_impl : public pimple_object
{
private:
	enum result_t { error = 1, success = 0 };

public:
	thread_impl(thread& owner);
	~thread_impl();
	
	bool start();
	void terminate() { _bTerminate = true; }
	bool wait();
	bool isStarted() const { return _id != -1;}
	bool isTerminating() const { return _bTerminate; }
	int getId() const { return _id; }

	static void yield();
	static void sleep(int msInterval);
	static int getCurrentThreadId();
	
private:
	static int threadLoop(void* ptr);
	void cleanup();
	
private:
	thread&				_owner;
	semaphore			_startUpEvent;
	critical_section	_runSection;
	volatile int		_id;
	volatile bool 		_bTerminate;
	result_t			_result;
};

//////////////////////////////////////////////////////////////////////
// thread::thread_impl
//////////////////////////////////////////////////////////////////////

thread::thread_impl::thread_impl(thread& owner)
	:	_owner(owner),
		_id(-1),
		_bTerminate(false),
		_result(error)
{
}

thread::thread_impl::~thread_impl()
{
	cleanup();
}

bool thread::thread_impl::start()
{
	// check if active object is already started
	if (isStarted())
		return true;
	
	// clear thread variables (in case if this active object was started once)
	cleanup();

	_id = taskSpawn(const_cast<char*>(_owner.getName()),
		_owner.getPriority(),
		0,
		100*1024,
		(FUNCPTR)&threadLoop,
		(int)this,0,0,0,0,0,0,0,0,0);
	
	if (ERROR == _id)
		return false;
	
	// wait for initialization
	_startUpEvent.aquire();
	
	// check the result
	if (_result == error)
	{
		wait();
		return false;
	}
	
	return true;
}

bool thread::thread_impl::wait()
{
	assert(getCurrentThreadId() != _id);

	critical_scope<> waitScope(_runSection);
	// clear thread variables
	cleanup();
	return true;
}

void thread::thread_impl::yield()
{
	sleep(0);
}

void thread::thread_impl::sleep(int msInterval)
{
	taskDelay(msInterval * sysClkRateGet() / 1000);
}

int thread::thread_impl::getCurrentThreadId()
{
	return taskIdSelf();
}

int thread::thread_impl::threadLoop(void* ptr)
{
	thread_impl* self = reinterpret_cast<thread_impl*>(ptr);
	assert(self != NULL);

	critical_scope<> runScope(self->_runSection);
	
	if (!self->_owner.onInitialize())
	{
		self->_id = -1;
		self->_startUpEvent.release();
		return error;
	}
	else
	{
		self->_result = success;
		self->_startUpEvent.release();
		// run active object
		self->_owner.run();
		// uninitialize active object members
		self->_owner.onUninitialize();
		// thread is stopped
		self->_id = -1;
		return success;
	}
}

void thread::thread_impl::cleanup()
{
	assert(!isStarted());
	_bTerminate = false;
}

} // namespace quark

#endif // _VXWORKS_QUARK_THREAD_IMPL_H_
