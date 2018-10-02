// dispatcher.h
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

#ifndef _NUCLEON_DISPATCHER_H_
#define _NUCLEON_DISPATCHER_H_

#include <quark/config.h>

namespace nucleon
{

//////////////////////////////////////////////////
// class template static_dispatcher
//
//! Generic static dispatcher
/*!
	Implements a constant time dispatcher for class member functions.
	Note: do not use for hierarchy with virtual or multiple inheritance!
*/
#if defined(__GNUC__) || defined(__DCC__) 
template<class _Host, 
		const size_t _Max, 
		class _BaseArg, 
		typename _Result = bool, 
		const int _Inst = 1>
class static_dispatcher
{
public:
	typedef _Result (_Host::*call_type)(_BaseArg&);

protected:
	template<typename _R, class _H, class _A>
	struct handler
	{
	};

	template<class _H, class _A>
	struct handler<bool, _H, _A>
	{
		handler<bool, _H, _A>(call_type fn = 0)
			:	_fn(fn) {}

		bool operator()(_H* host, _A& arg) const
		{
			if (_fn == 0)
				return false;
			return (host->*_fn)(arg);
		}

	private:
		call_type _fn;
	};

	template<class _H, class _A>
	struct handler<void, _H, _A>
	{
		handler<void, _H, _A>(call_type fn = 0)
			:	_fn(fn) {}

		bool operator()(_H* host, _A& arg) const
		{
			if (_fn == 0)
				return false;
			(host->*_fn)(arg);
			return true;
		}

	private:
		call_type _fn;
	};

	typedef handler<_Result, _Host, _BaseArg> handler_type;

	template<class _Arg>
	struct handler_maker
	{
		typedef _Result (_Host::*handler_maker_fn_type)(_Arg&);
		typedef _Result (_Host::*handler_maker_fn_constArg_type)(const _Arg&);

		const handler_type operator()(handler_maker_fn_type fn)
			{ return handler_type(reinterpret_cast<call_type>(fn)); }

		const handler_type operator()(handler_maker_fn_constArg_type fn)
			{ return handler_type(reinterpret_cast<call_type>(fn)); }
	};

	typedef handler_type handler_map[_Max];
	typedef static_dispatcher<_Host, _Max, _BaseArg, _Result, _Inst> dispatcher_type;
	typedef _Host host_type;

protected:
	static_dispatcher() {}
	virtual ~static_dispatcher() {}

	bool dispatch(size_t id, _BaseArg& msg)
	{
		assert(id < _Max);
		return s_map[id](static_cast<_Host*>(this), msg);
	}

private:
	static const handler_type* s_map;
};

#else

template<class _Host, 
		const size_t _Max, 
		class _BaseArg, 
		typename _Result = bool, 
		const int _Inst = 1>
class static_dispatcher
{
public:
	typedef _Result (_Host::*call_type)(_BaseArg&);

protected:
	template<typename _T>
	struct handler
	{
	};

	template<>
	struct handler<bool>
	{
		handler<bool>(call_type fn = 0)
			:	_fn(fn) {}

		bool operator()(_Host* host, _BaseArg& arg) const
		{
			if (_fn == 0)
				return false;
			return (host->*_fn)(arg);
		}

	private:
		call_type _fn;
	};

	template<>
	struct handler<void>
	{
		handler<void>(call_type fn = 0)
			:	_fn(fn) {}

		bool operator()(_Host* host, _BaseArg& arg) const
		{
			if (_fn == 0)
				return false;
			(host->*_fn)(arg);
			return true;
		}

	private:
		call_type _fn;
	};

	typedef handler<_Result> handler_type;

	template<class _Arg>
	struct handler_maker
	{
		typedef _Result (_Host::*handler_maker_fn_type)(_Arg&);
		typedef _Result (_Host::*handler_maker_fn_constArg_type)(const _Arg&);

		const handler_type operator()(handler_maker_fn_type fn)
			{ return handler_type(reinterpret_cast<call_type>(fn)); }

		const handler_type operator()(handler_maker_fn_constArg_type fn)
			{ return handler_type(reinterpret_cast<call_type>(fn)); }
	};

	typedef handler_type handler_map[_Max];
	typedef static_dispatcher<_Host, _Max, _BaseArg, _Result, _Inst> dispatcher_type;
	typedef _Host host_type;

protected:
	static_dispatcher() {}
	virtual ~static_dispatcher() {}

	bool dispatch(size_t id, _BaseArg& msg)
	{
		assert(id < _Max);
		return s_map[id](static_cast<_Host*>(this), msg);
	}

private:
	static const handler_type* s_map;
};
#endif

// Initialize the static handler map
template<class _Host, const size_t _Max, class _BaseArg, typename _Result, const int _Inst>
const typename static_dispatcher<_Host, _Max, _BaseArg, _Result, _Inst>::handler_type* 
	static_dispatcher<_Host, _Max, _BaseArg, _Result, _Inst>::s_map = 
	static_dispatcher<_Host, _Max, _BaseArg, _Result, _Inst>::host_type::
	initHandlerMap(quark::type2type<dispatcher_type>());

//! Marks the beginning of the handler map
/*!
	In the class declaration, start the handler map with the BEGIN_DISP_MAP macro,
	then add macro entries (DISP_ENTRY) for each of your handler functions,
	and complete the handler map with the END_DISP_MAP macro.
	param\ Specifies the name of the class that owns this handler map
*/

#define BEGIN_DISP_MAP(dispatcher) \
	FRIEND_CLASS dispatcher; \
	static const dispatcher::handler_map& initHandlerMap(quark::type2type<dispatcher >) { \
		static dispatcher::handler_map map; \
		typedef dispatcher disp;

//! Defines an entry in a handler map
/*!
	It must be between BEGIN_DISP_MAP and END_DISP_MAP.
	\param type identifier
	\param type
	\param handler function
*/
#define DISP_ENTRY(id, type, fn) \
	disp::handler_type handler##type = disp::handler_maker<type>()(&fn); \
	map[id] = handler##type;

//! Marks the end of a handler map
/*!
	It must be used in conjunction with BEGIN_DISP_MAP.
*/
#define END_DISP_MAP() \
	return map; }

} // namespace nucleon

#endif // _NUCLEON_DISPATCHER_H_
