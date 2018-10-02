// user_tracer.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Meson Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESON_USER_TRACER_H_
#define _MESON_USER_TRACER_H_

#include <quark/config.h>
#include <quark/sync.h>
#include <stdarg.h>

namespace meson
{

//////////////////////////////////////////////////
// class user_tracer
//
class user_tracer
{
private:
	typedef void (*print_fn) (int level, const char* format, va_list argl);
	typedef void (*indent_fn) (const char* indent);
	typedef void (*unindent_fn) ();

public:
	void debug(const char* format, ...);
	void info(const char* format, ...);
	void notice(const char* format, ...);
	void warning(const char* format, ...);
	void error(const char* format, ...);
	void print(int level, const char* format, va_list argl);

	void indent(const char* indent);
	void unindent();

	bool bind(print_fn fnPrint, indent_fn fnIndent, unindent_fn fnUnindent);

	static user_tracer& instance();

private:
	user_tracer();
	void defaultBind();

private:
	volatile print_fn _fnPrint;
	volatile indent_fn _fnIndent;
	volatile unindent_fn _fnUnindent;
	quark::critical_section _mtx;
};

//////////////////////////////////////////////////
// class template binder
//
template<class _Tracer>
class tracer_binder
{
public:
	static bool bind()
		{ return user_tracer::instance().bind(onPrint, onIndent, onUnindent); }

private:
	tracer_binder();
	~tracer_binder();

	static void onPrint(int level, const char* format, va_list argl)
		{ _Tracer::instance().print(level, format, argl); }

	static void onIndent(const char* indent)
		{ _Tracer::instance().indent(indent); }

	static void onUnindent()
		{ _Tracer::instance().unindent(); }
};

template<class _T>
bool bindTracer()
{
	return tracer_binder<_T>::bind();
}

//#ifndef TRACE_DEBUG
#define TRACE_DEBUG meson::user_tracer::instance().debug
//#endif // TRACE_DEBUG

#ifndef TRACE_INFO
#define TRACE_INFO meson::user_tracer::instance().info
#endif // TRACE_INFO

#ifndef TRACE_NOTICE
#define TRACE_NOTICE meson::user_tracer::instance().notice
#endif // TRACE_INFO

#ifndef TRACE_WARNING
#define TRACE_WARNING meson::user_tracer::instance().warning
#endif // TRACE_WARNING

#ifndef TRACE_ERROR
#define TRACE_ERROR meson::user_tracer::instance().error
#endif // TRACE_ERROR

#ifndef TRACE_INDENT
#define TRACE_INDENT(x) meson::auto_indent<meson::user_tracer> indent(meson::user_tracer::instance(), x)
#endif // TRACE_INDENT

} // namespace meson

#endif // _MESON_USER_TRACER_H_
