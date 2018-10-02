// thread_impl.h
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

#ifndef _WIN32_QUARK_THREAD_IMPL_H_
#define _WIN32_QUARK_THREAD_IMPL_H_

#include <quark/mempool/service/service_mempool.h>
#include "sync.h"

// Platform-specific headers
#include <windows.h>
#include <process.h>

#include <functional>

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
	bool isStarted() const { return _id != -1; }
	bool isTerminating() const { return _bTerminate; }
	int getId() const { return _id; }
	
	static void sleep(unsigned int ms_seconds);
	static void yield();
	static int getCurrentThreadId();

private:
	static unsigned int __stdcall threadLoop(void* ptr);
	void cleanup();

private:
	thread&					_owner;
	semaphore				_startUpEvent;
	HANDLE					_handle;
	volatile int			_id;
	volatile bool			_bTerminate;
	result_t				_result;
};

//////////////////////////////////////////////////////////////////////
// thread::thread_impl
//////////////////////////////////////////////////////////////////////

thread::thread_impl::thread_impl(thread& owner)
	:	_owner(owner),
		_handle(INVALID_HANDLE_VALUE),
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

	// create the thread
	//unsigned long 
	uintptr_t handle = ::_beginthreadex(NULL, 0, &threadLoop, this, 0, (unsigned int*)&_id);
	if (handle <= 0)
		return false;

	_handle = reinterpret_cast<HANDLE>(handle);
	::SetThreadPriority(_handle, _owner._priority);

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

	if (INVALID_HANDLE_VALUE != _handle)
	{
		DWORD res = ::WaitForSingleObject(_handle, INFINITE);
		if (res != WAIT_OBJECT_0)
			return false;

		// clear thread variables
		cleanup();
	}
	return true;
}

inline void thread::thread_impl::sleep(unsigned int ms_seconds)
{
	::Sleep(ms_seconds);
}

inline void thread::thread_impl::yield()
{
	::SwitchToThread();
}

inline int thread::thread_impl::getCurrentThreadId()
{
	return ::GetCurrentThreadId();
}

unsigned int __stdcall thread::thread_impl::threadLoop(void* ptr)
{
	thread_impl* self = reinterpret_cast<thread_impl*>(ptr);
	assert(NULL != self);
	self->_result = error;

	// set unexpected handler
	unexpected_handler oldUnexpectedHandler;
	if (self->_owner._unexpected != NULL)
	{
		oldUnexpectedHandler = set_unexpected(self->_owner._unexpected);
	}

	// initialize active object members
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
		try
		{
			self->_owner.run();
		}
#ifdef _DEBUG
		catch (std::exception& )
#else
		catch (...)
#endif // _DEBUG
		{
			unexpected();
		}
		// uninitialize active object members
		self->_owner.onUninitialize();
		// thread is stopped
		self->_id = -1;

		if (self->_owner._unexpected != NULL)
		{
			set_unexpected(oldUnexpectedHandler);
		}

		return success;
	}
}

void thread::thread_impl::cleanup()
{
	assert(!isStarted());

	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
		_bTerminate = false;
	}
}

class scheduler::scheduler_impl : public pimple_object
{
public:
	static void setaffinity(int* handle, size_t mask_size, int* mask);
};

inline void scheduler::scheduler_impl::setaffinity(int* handle, size_t mask_size, int* mask)
{
	if (mask_size > 0)
	{
		HANDLE process = (handle == NULL) ? GetCurrentProcess() : static_cast<HANDLE>(handle);
		DWORD_PTR processAffinityMask = 0;
		for (int i = 0; i < mask_size; ++i)
		{
			processAffinityMask |= 1 << mask[i];
		}

		BOOL success = SetProcessAffinityMask(process, processAffinityMask);
	}
}

} // namespace quark

#endif // _WIN32_QUARK_THREAD_IMPL_H_

