// user_tracer.cpp
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

#include "user_tracer.h"
#include <meson/tracer.h>

using namespace meson;

#define DO_PRINT(level, format) { \
	va_list argl; \
	va_start(argl, format); \
	print(level, format, argl); \
	va_end(argl); }

//////////////////////////////////////////////////////////////////////
// user_tracer
//////////////////////////////////////////////////////////////////////

user_tracer::user_tracer()
	:	_fnPrint(0)
{
}

void user_tracer::debug(const char* format, ...)
{
	DO_PRINT(tlDebug, format);
}

void user_tracer::info(const char* format, ...)
{
	DO_PRINT(tlInfo, format);
}

void user_tracer::notice(const char* format, ...)
{
	DO_PRINT(tlNotice, format);
}

void user_tracer::warning(const char* format, ...)
{
	DO_PRINT(tlWarning, format);
}

void user_tracer::error(const char* format, ...)
{
	DO_PRINT(tlError, format);
}

void user_tracer::print(int level, const char* format, va_list argl)
{
	if (_fnPrint == 0)
		defaultBind();
	assert(_fnPrint);
	_fnPrint(level, format, argl);
}

void user_tracer::indent(const char* indent)
{
	if (_fnIndent == 0)
		defaultBind();
	assert(_fnIndent);
	_fnIndent(indent);
}

void user_tracer::unindent()
{
	if (_fnUnindent == 0)
		defaultBind();
	assert(_fnUnindent);
	_fnUnindent();
}

void user_tracer::defaultBind()
{
	tracer_binder<default_tracer>::bind();
}

bool user_tracer::bind(print_fn fnPrint, indent_fn fnIndent, unindent_fn fnUnindent)
{
	quark::critical_scope<> lock(_mtx);
	_fnPrint = fnPrint;
	_fnIndent = fnIndent;
	_fnUnindent = fnUnindent;
	return true;
}

user_tracer& user_tracer::instance()
{
	static user_tracer inst;
	return inst;
}
