// msg_queue.h
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

#ifndef _QUARK_MSG_QUEUE_H_
#define _QUARK_MSG_QUEUE_H_

#include "config.h"
#include "sync.h"

namespace quark
{

//////////////////////////////////////////////////
// class msg_queue_to_exception
//
class msg_queue_to_exception : public quark::pruntime_error
{
public:
	explicit msg_queue_to_exception(const pstring& str)
		:	pruntime_error(str)
	{}
};

//////////////////////////////////////////////////
// class template msg_queue
//
template 
<
	class _T, 
	class _Cleaner = cleaner_ptr, 
	class _Container = plist<_T>
>
class msg_queue : public pooled_object,
				  public noncopyable
{
private:
	typedef	std::queue<_T, _Container> queue_t;

public:
	msg_queue() 
		:	_signalled(false) {}
	~msg_queue();

	void push(const _T& t);
	const _T get(int msInterval = semaphore::forever);
	size_t size() const;
	bool empty() const;

private:
	queue_t						_queue;
	mutable critical_section	_cs;
	semaphore					_sem;
	volatile bool				_signalled;
};

//////////////////////////////////////////////////
// msg_queue implementation
//////////////////////////////////////////////////

template<class _T, class _Cleaner, class _Container>
msg_queue<_T, _Cleaner, _Container>::~msg_queue()
{
	critical_scope<critical_section> guard(_cs);
	
	while (!_queue.empty())
	{	
		_Cleaner()(_queue.front());
		_queue.pop();
	}
}

template<class _T, class _Cleaner, class _Container>
void msg_queue<_T, _Cleaner, _Container>::push(const _T& t)
{
	critical_scope<critical_section> guard(_cs);
	
	_queue.push(t);
	
	if (_queue.size() == 1 && !_signalled)
	{
		_signalled = true;
		_sem.release(); // set signal
	}
}

template<class _T, class _Cleaner, class _Container>
const _T msg_queue<_T, _Cleaner, _Container>::get(int msInterval)
{
	if (empty())
	{
		if (!_sem.aquire(msInterval)) // wait for signal
		{
			throw msg_queue_to_exception("message queue timeout");
		}
		_signalled = false;
	}
	
	critical_scope<critical_section> guard(_cs);
	
	if (_queue.empty())
	{
		printf("message queue is empty\n");
		throw pruntime_error("message queue is empty");
	}
	
	if (_signalled)
	{
		_sem.aquire(); // reset signal
		_signalled = false;
	}
	
	const _T t = _queue.front();
	_queue.pop();
	return t;	
}

template<class _T, class _Cleaner, class _Container>
inline size_t msg_queue<_T, _Cleaner, _Container>::size() const
{
	critical_scope<critical_section> guard(_cs);
	return _queue.size();
}

template<class _T, class _Cleaner, class _Container>
inline bool msg_queue<_T, _Cleaner, _Container>::empty() const
{
	critical_scope<critical_section> guard(_cs);
	return _queue.empty();
}

} // namespace quark

#endif // _QUARK_MSG_QUEUE_H_
