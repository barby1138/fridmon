// event_dispatcher.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Nucleon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _NUCLEON_EVENT_DISPATCHER_H_
#define _NUCLEON_EVENT_DISPATCHER_H_

#include <quark/singleton.h>
#include "assoc_vector.h"

// NOTE: Under construction!
// TODO: Finish dispatch map policy.
// TODO: Merge GNU with general version.

namespace nucleon
{

struct logarithmic_dispatch_map
{
	template<typename _HandlerType>
	struct map_type : assoc_vector<size_t, _HandlerType>
	{
	};
};

//////////////////////////////////////////////////
// class template event_dispatcher
//
//! Generic static dispatcher
/*!
	Implements a constant time dispatcher for class member functions.
	Note: do not use for hierarchy with virtual or multiple inheritance!
*/
#if defined(__GNUC__) || defined(__DCC__)
template
<
	class _Host,
	class _BaseEvent,
	typename _FnResult,
	class _HandlerMap = logarithmic_dispatch_map
>
class event_dispatcher : private _HandlerMap
{
public:
	typedef _Host host_type;
	typedef _FnResult (_Host::*fn_type)(_BaseEvent&);

	template<class _Result, class _H>
	struct result_t
	{
		bool disp_result;
		_Result fn_result;
	};

	template<class _H>
	struct result_t<void, _H>
	{
		bool disp_result;
	};

	typedef result_t<_FnResult, _Host> result_type;

	template<class _Event>
	struct handler_maker
	{
		handler_maker(_FnResult (_Host::*fn)(_Event&) )
			:	_fn(fnTypeCast(fn)) {}

		handler_maker(_FnResult (_Host::*fn)(const _Event&) )
			:	_fn(fnTypeCast(fn)) {}

		fn_type _fn;
	};

protected:
	template<typename _Result, class _H>
	struct handler_t
	{
		handler_t(fn_type fn = 0)
			:	_fn(fn) {}

		const result_type operator()(_Host* host, _BaseEvent& arg) const
		{
			result_type res = { false };
			if (res.disp_result = _fn)
				res.fn_result = (host->*_fn)(arg);
			return res;
		}

		fn_type _fn;
	};

	template<class _H>
	struct handler_t<void, _H>
	{
		handler_t(fn_type fn = 0)
			:	_fn(fn) {}

		const result_type operator()(_Host* host, _BaseEvent& arg) const
		{
			result_type res = { false };
			if (res.disp_result = _fn)
				(host->*_fn)(arg);
			return res;
		}

		fn_type _fn;
	};

	typedef handler_t<_FnResult, _Host> handler_type;
	logarithmic_dispatch_map::map_type<handler_type> _handlerMap;

public:
	const result_type dispatch(_Host* host, size_t id, _BaseEvent& arg)
	{
		checkId(id);
		return _handlerMap[id](host, arg);
	}

	template<class _Event>
	void addHandler(size_t id, _FnResult (_Host::*fn)(_Event) )
	{
		checkId(id);
		_handlerMap[id] = fnTypeCast(fn);
	}

	// Use for resolution of ambiguous functions by signature
	template<class _Event>
	void addHandler(size_t id, const handler_maker<_Event>& maker)
	{
		checkId(id);
		_handlerMap[id] = maker._fn;
	}

	template<typename _Event>
	static fn_type fnTypeCast(_FnResult (_Host::*fn)(_Event) )
	{
		return reinterpret_cast<fn_type>(fn);
	}

	static void checkId(size_t id)
	{
		/*assert(id < _MaxId);
		if (id >= _MaxId)
			throw quark::pruntime_error("event_dispatcher: id is out of range");*/
	}

protected:
	event_dispatcher() {}
	~event_dispatcher() {}
};

} // namespace nucleon

#define DECLARE_DISPATCHER(name) \
	class name##_impl; friend name##_impl;

#define BEGIN_DISPATCHER(name, host, baseArg, result) \
	typedef quark::singleton_holder \
	< \
		host##::name##_impl, quark::create_static, \
		quark::default_lifetime, quark::class_level_lockable<host##::name##_impl> \
	> name; \
    class host##::name##_impl : public nucleon::event_dispatcher<host, baseArg, result> \
	{ \
	public: \
		name##_impl() \
		{

#define DISPATCH_ENTRY(id, fn) \
	addHandler(id, fn);

#define DISPATCH_ENTRY2(id, fn, arg) \
	addHandler(id, handler_maker<arg>(fn));

#define END_DISPATCHER() \
		} \
	}; \

#else

template
<
	class _Host,
	class _BaseEvent,
	typename _FnResult,
	class _HandlerMap = logarithmic_dispatch_map
>
class event_dispatcher : private _HandlerMap
{
public:
	typedef _Host host_type;
	typedef _FnResult (_Host::*fn_type)(_BaseEvent&);

	template<class _FnResult>
	struct result_t
	{
		bool disp_result;
		_FnResult fn_result;
	};

	template<>
	struct result_t<void>
	{
		bool disp_result;
	};

	typedef result_t<_FnResult> result_type;

	template<class _Event>
	struct handler_maker
	{
		handler_maker(_FnResult (_Host::*fn)(_Event&) )
			:	_fn(fnTypeCast(fn)) {}

		handler_maker(_FnResult (_Host::*fn)(const _Event&) )
			:	_fn(fnTypeCast(fn)) {}

		fn_type _fn;
	};

protected:
	template<typename _FnResult>
	struct handler_t
	{
		handler_t(fn_type fn = 0)
			:	_fn(fn) {}

		const result_type operator()(_Host* host, _BaseEvent& arg) const
		{
			result_type res = { false };
			if (res.disp_result = _fn)
				res.fn_result = (host->*_fn)(arg);
			return res;
		}

		fn_type _fn;
	};

	template<>
	struct handler_t<void>
	{
		handler_t<void>(fn_type fn = 0)
			:	_fn(fn) {}

		const result_type operator()(_Host* host, _BaseEvent& arg) const
		{
			result_type res = { false };
			if (res.disp_result = _fn)
				(host->*_fn)(arg);
			return res;
		}

		fn_type _fn;
	};

	typedef _Host host_type;
	typedef handler_t<_FnResult> handler_type;
	logarithmic_dispatch_map::map_type<handler_type> _handlerMap;

public:
	const result_type dispatch(_Host* host, size_t id, _BaseEvent& arg)
	{
		checkId(id);
		return _handlerMap[id](host, arg);
	}

	template<class _Event>
	void addHandler(size_t id, _FnResult (_Host::*fn)(_Event) )
	{
		checkId(id);
		_handlerMap[id] = fnTypeCast(fn);
	}

	// Use for resolution of ambiguous functions by signature
	template<class _Event>
	void addHandler(size_t id, const handler_maker<_Event>& maker)
	{
		checkId(id);
		_handlerMap[id] = maker._fn;
	}

	template<typename _Event>
	static fn_type fnTypeCast(_FnResult (_Host::*fn)(_Event) )
	{
		return reinterpret_cast<fn_type>(fn);
	}

	static void checkId(size_t id)
	{
		/*assert(id < _MaxId);
		if (id >= _MaxId)
			throw quark::pruntime_error("event_dispatcher: id is out of range");*/
	}

protected:
	event_dispatcher() {}
	~event_dispatcher() {}
};

} // namespace nucleon

#define DECLARE_DISPATCHER(name) \
	class name##_impl; friend name##_impl;

#define BEGIN_DISPATCHER(name, host, baseArg, result) \
	typedef quark::singleton_holder \
	< \
		host##::name##_impl, quark::create_static, \
		quark::default_lifetime, quark::class_level_lockable<host##::name##_impl> \
	> name; \
    class host##::name##_impl : public nucleon::event_dispatcher<host, baseArg, result> \
	{ \
	public: \
		name##_impl() \
		{

#define DISPATCH_ENTRY(id, fn) \
	addHandler(id, fn);

#define DISPATCH_ENTRY2(id, fn, arg) \
	addHandler(id, handler_maker<arg>(fn));

#define END_DISPATCHER() \
		} \
	}; \

#endif // __GNUC__

#endif // _NUCLEON_EVENT_DISPATCHER_H_
