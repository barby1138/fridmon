// sync_impl.h
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

#ifndef _WIN32_QUARK_SYNC_IMPL_H_
#define _WIN32_QUARK_SYNC_IMPL_H_

#include <quark/mempool/service/service_mempool.h>

// Platform-specific headers
#include <wtypes.h>
#include <winbase.h>

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
		::InitializeCriticalSectionAndSpinCount(&_cs, 4000);
	}
	
	~critical_section_impl()
	{
		::DeleteCriticalSection(&_cs);
	}
	
	void lock()
	{
		::EnterCriticalSection(&_cs);
	}
	
	void unlock()
	{		
		::LeaveCriticalSection(&_cs);
	}
	
private:
	CRITICAL_SECTION _cs;
};

//////////////////////////////////////////////////
// class semaphore::semaphore_impl
//
class semaphore::semaphore_impl : public pimple_object
{
public:
	semaphore_impl(int initCount, const char* name)
	{
		_handle = ::CreateSemaphore(NULL, initCount, LONG_MAX, name);
		// TODO: verify with Adan if this necessary
		/*if (_handle)
			::SetSecurityInfo(_handle, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, NULL, NULL);*/
	}
	
	~semaphore_impl()
	{
		::CloseHandle(_handle);
	}
	
	bool aquire(int msInterval)
	{
		// WaitForSingleObject accepts value in millisec
		return (::WaitForSingleObject(_handle, 
			(0 > msInterval) ? INFINITE : msInterval) == WAIT_OBJECT_0);
	}
	
	void release()
	{
		::ReleaseSemaphore(_handle, 1, NULL);
	}
	
	bool isInitialized() const
	{
		return (NULL != _handle);
	}
	
private:
	HANDLE _handle;
};

inline int atomicIncrement(volatile int& lval)
{
	return ::InterlockedIncrement((long*)&lval);
}

inline int atomicDecrement(volatile int& lval)
{
	return ::InterlockedDecrement((long*)&lval);
}

inline int atomicExchange(volatile int& lval, int val)
{
	return ::InterlockedExchange((long*)&lval, val);
}

} // namespace quark

#endif //_WIN32_QUARK_SYNC_IMPL_H_
