// thread.h
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

#ifndef _QUARK_THREAD_H_
#define _QUARK_THREAD_H_

#include "config.h"

#include <atomic>
#include <thread>
#include <utility>

namespace quark
{
//////////////////////////////////////////////////
// class thread
//
class thread : public pooled_object,
			   public noncopyable
{
private:
	class thread_impl;
	friend class thread_impl;

public:
	typedef void (*unexpected_handler) ();

public:
	bool start();
	bool stop();
	bool wait();
	virtual void terminate();

	const char* getName() const;
	int getId() const;
	int getPriority() const;
	bool isStarted() const;
	void setUnexpected(unexpected_handler func);
		
	static void sleep(int ms_interval);
	static void yield();
	static int getCurrentThreadId();

protected:
	thread(const char* name = "noname", int priority = default_thread_priority);
	~thread();

	virtual void run() = 0;
	virtual bool onInitialize();
	virtual void onUninitialize();
	bool isTerminating() const;

private:
	thread_impl*		_pImpl;
	pstring 			_name;
	int 				_priority;
	unexpected_handler	_unexpected;
};

//////////////////////////////////////////////////
// class thread_bundle
//
//! Manages a bunch of threads
/*!
	Stack model is used for starting/stopping threads
*/
class thread_bundle : public pooled_object,
					  public noncopyable
{
private:
	typedef pvector<thread*> threads_t;
	struct startthread;
	struct stopthread;
	
public:
	thread_bundle();

	void attach(thread& t);
	void detach(thread& t);
	bool start();
	bool stop();

private:
	threads_t _threads;
};

//////////////////////////////////////////////////
// class scheduler
//
class scheduler : public pooled_object,
	public noncopyable
{
private:
	class scheduler_impl;
	friend class scheduler_impl;

public:
	static void setaffinity(int* handle, size_t mask_size, int* mask);
};

} // namespace quark

#endif // _QUARK_THREAD_H_

