// sync.h
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

#ifndef _QUARK_SYNC_H_
#define _QUARK_SYNC_H_

namespace quark
{

//////////////////////////////////////////////////
// class dummy_mutex
//
class dummy_mutex : public noncopyable
{
public:
	void lock() {}
	void unlock() {}
};

//////////////////////////////////////////////////
// class light_mutex
//
// Note: Do not call recursively
class spin_lock : public noncopyable
{
private:
	enum { locked, unlocked };

public:
	spin_lock();

	void lock();
	void unlock()
		{ _lock = unlocked; }

private:
	void contendedLock();

private:
	volatile int _lock;
};

//////////////////////////////////////////////////
// class critical_section
//
class critical_section : public noncopyable
{
public:
	class critical_section_impl;
	
public:
	critical_section();
	~critical_section();
	void lock();
	void unlock();

private:
	critical_section_impl* _pImpl;
};

//////////////////////////////////////////////////
// class semaphore
//
class semaphore : public noncopyable
{
public:
	enum { forever = U32_MAX };

private:
	class semaphore_impl;

public:
	explicit semaphore(int initCount = 0, const char* name = NULL);
	~semaphore();

	bool aquire(int msInterval = (int)forever);
	void release();

private:
	semaphore_impl* _pImpl;
};

//////////////////////////////////////////////////
// class template critical_scope
//
template<class _Mutex = critical_section>
class critical_scope : public noncopyable
{
public:
	critical_scope(_Mutex& mutex) 
		:	_mutex(mutex) { _mutex.lock(); }
	~critical_scope()
		{ _mutex.unlock(); }

private:
	_Mutex& _mutex;
};

//////////////////////////////////////////////////
// class interlocked
//
class interlocked
{
public:
	static int increment(volatile int& lval);
	static int decrement(volatile int& lval);
	static int exchange(volatile int& lval, int val);
};

} // namespace quark

#endif // _QUARK_SYNC_H_
