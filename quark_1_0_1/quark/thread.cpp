// thread.cpp
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

#include "thread.h"
#include IMPL_FILE(thread_impl.h)

using namespace quark;

//////////////////////////////////////////////////////////////////////
// thread
//////////////////////////////////////////////////////////////////////

thread::thread(const char* name, int priority)
	:	_name(name),
		_priority(priority),
		_unexpected(NULL)
{
	_pImpl = new thread_impl(*this);
}

thread::~thread()
{
	delete _pImpl;
	_pImpl = NULL;
}

const char* thread::getName() const
{
	return _name.c_str();
}

int thread::getId() const
{
	return _pImpl->getId();
}

int thread::getPriority() const
{
	return _priority;
}

bool thread::start()
{
	return _pImpl->start();
}

bool thread::stop()
{
	terminate();
	return wait();
}

void thread::terminate()
{
	_pImpl->terminate();
}

bool thread::wait()
{
	return _pImpl->wait();
}

void thread::setUnexpected(unexpected_handler func)
{
	_unexpected = func;
}

bool thread::isStarted() const
{
	return _pImpl->isStarted();
}

bool thread::isTerminating() const
{
	return _pImpl->isTerminating();
}

bool thread::onInitialize()
{
	return true;
}

void thread::onUninitialize()
{
}

//static members
void thread::sleep(int ms_interval)
{
	thread_impl::sleep(ms_interval);
}

void thread::yield()
{
	thread_impl::yield();
}

int thread::getCurrentThreadId()
{
	return thread_impl::getCurrentThreadId();
}

//////////////////////////////////////////////////////////////////////
// thread_bundle
//////////////////////////////////////////////////////////////////////

struct thread_bundle::startthread
{
	startthread() : _result(true) {}

	void operator()(thread* p)
	{
		if (!p->isStarted())
			_result &= p->start();
	}

	bool _result;
};

struct thread_bundle::stopthread
{
	stopthread() : _result(true) {}

	void operator()(thread* p)
	{
		if (p->isStarted())
			_result &= p->stop();
	}

	bool _result;
};

thread_bundle::thread_bundle()
{
}

void thread_bundle::attach(thread& t)
{
	if (std::find_if(_threads.begin(), _threads.end(), 
		std::bind2nd(std::equal_to<thread*>(), &t)) == _threads.end())
		_threads.push_back(&t);
}

void thread_bundle::detach(thread& t)
{
	_threads.erase(
		std::remove_if(_threads.begin(), _threads.end(), 
		std::bind2nd(std::equal_to<thread*>(), &t))
		);
}

bool thread_bundle::start()
{
	bool result = std::for_each(_threads.rbegin(), _threads.rend(), startthread())._result;

	if (!result)
		stop();

	return result;
}

bool thread_bundle::stop()
{
	return std::for_each(_threads.rbegin(), _threads.rend(), stopthread())._result;
}

void scheduler::setaffinity(int* handle, size_t mask_size, int* mask)
{
	scheduler_impl::setaffinity(handle, mask_size, mask);
}
