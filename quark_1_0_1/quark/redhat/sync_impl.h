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

#ifndef _REDHAT_QUARK_SYNC_IMPL_H_
#define _REDHAT_QUARK_SYNC_IMPL_H_

#include <quark/mempool/service/service_mempool.h>
#include <quark/time.h>

#include "atomic_impl.h"

// Platform-specific headers
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/times.h>

namespace quark
{

// in milisec
#define WAIT_RETRY_INTERVAL	1

//////////////////////////////////////////////////
// class critical_section::critical_section_impl
//
class critical_section::critical_section_impl : public pimple_object
{
public:
	critical_section_impl()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		int nRet = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
		assert(nRet == 0);
		
		nRet = pthread_mutex_init(&_mutex, &attr);
		assert(nRet == 0);

		pthread_mutexattr_destroy(&attr);
	}
	
	~critical_section_impl()
	{
		int nRet = pthread_mutex_destroy(&_mutex);
		assert(nRet == 0);
	}
	
	void lock()
	{
		int nRet = pthread_mutex_lock(&_mutex);
		assert(nRet == 0);
	}
	
	void unlock()
	{		
		int nRet = pthread_mutex_unlock(&_mutex);
		assert(nRet == 0);
	}
	
private:
	pthread_mutex_t _mutex;
};

//////////////////////////////////////////////////
// class semaphore::semaphore_impl
//
class semaphore::semaphore_impl : public pimple_object
{
public:
	semaphore_impl(int initCount, const char* name)
	{
		int nRet = sem_init(&_sem, 0, initCount);
		assert(nRet == 0);
	}
	
	~semaphore_impl()
	{
		int nRet = sem_destroy(&_sem);
		assert(nRet == 0);
	}
	
	bool aquire(int msInterval)
	{		
		if (msInterval == forever)
		{
			sem_wait(&_sem);
			return true;
		}
		else
		{
#ifdef __USE_XOPEN2K
				struct timespec ts;
				int nRet = clock_gettime(CLOCK_REALTIME, &ts);
				assert(nRet == 0);
				
				ts.tv_sec += MILLI_TO_SEC(msInterval);
				ts.tv_nsec += MILLI_TO_NANO(msInterval % SEC_TO_MILLI(1));

				long extrasec = NANO_TO_SEC(ts.tv_nsec);
				ts.tv_sec += extrasec;
				ts.tv_nsec -= SEC_TO_NANO(extrasec);
				
				nRet = sem_timedwait(&_sem, &ts);
				return (nRet == 0);
#else
				unsigned long ulInterval = MILLI_TO_MICRO(msInterval);
				unsigned long ulIntervalWaited = 0UL;
				unsigned long ulSleepInterval = MILLI_TO_MICRO(WAIT_RETRY_INTERVAL);
				do
				{
					int nRet = sem_trywait(&_sem);
					if (nRet == 0) 
					{
						return true;
					}
					else if (errno == EAGAIN) 
					{
							usleep(ulSleepInterval);
							ulIntervalWaited += ulSleepInterval;
							
							if (ulIntervalWaited >= ulInterval)
							{
								break;
							}	
					}
					else
					{
						assert(false && "this shoud not happen");
						return false;
					}
				} 
				while (true);
				
				return false;
#endif	
		}
	}
	
	void release()
	{
		int nRet = sem_post(&_sem);
		assert(nRet == 0);
	}
	
	bool isInitialized() const
	{
		return true;
	}
	
private:
	sem_t _sem;
};

inline int atomicIncrement(volatile int& lval)
{
	atomic_inc((atomic_t*) &lval);
	return lval;
}

inline int atomicDecrement(volatile int& lval)
{
	
	atomic_dec((atomic_t*) &lval);
	return lval;
}

inline int atomicExchange(volatile int& lval, int val)
{
	atomic_exchange(&lval, val);
	return val;
}

} // namespace quark

#endif //_WIN32_QUARK_SYNC_IMPL_H_
