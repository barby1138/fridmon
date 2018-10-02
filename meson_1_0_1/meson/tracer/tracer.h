// tracer.h
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

#ifndef _MESON_TRACER_H_
#define _MESON_TRACER_H_

#include <quark/strings.h>
#include <quark/thread.h>
#include <quark/time.h>
#include <quark/sync.h>

#include "path.h"
#include <stdarg.h>
#include <time.h>

// TODO: platform independency (va_list processing)

namespace meson
{

	enum trace_level_t { tlDebug
		, tlInfo, tlNotice, tlWarning, tlError, tlNone };

///////////////////////////////////////////////////////////
// class dummy_formatter
//
class dummy_formatter
{
protected:
	~dummy_formatter() {}

	void format(int level, quark::pstring& buffer, const char* format, va_list argl)
	{
	}
};

///////////////////////////////////////////////////////////
// class dummy_writer
//
class dummy_writer
{
protected:
	~dummy_writer() {}

	void write(int level, const char* buffer)
	{
	}
};

///////////////////////////////////////////////////////////
// class time_stamp
//
class time_stamp
{
public:
	void setFormat(const char* format) 
		{ _format = format; }

	const quark::pstring getFormat() const 
		{ return _format; }

protected:
	const quark::pstring now() const
	{
		return quark::time::now().toString(getFormat().c_str());
	}
	
protected:
	time_stamp(const char* format = "%b %d %H:%M:%S:%t")
		:	_format(format) {}

	~time_stamp() {}

private:
	quark::pstring _format;
};

///////////////////////////////////////////////////////////
// class default_formatter
//
template<class _TimeStamp = time_stamp>
class default_formatter : public _TimeStamp
{
public:
	enum options_t { opNone = 0x0, opTime = 0x1, opThread = 0x2, opLevel = 0x4, opAll = 0x7 };

public:
	void setOptions(int opt) 
		{ _options = (options_t)opt; }

	int getOptions() const
		{ return _options; }

	void indent(const char* indent) {}
	void unindent() {}

protected:
	default_formatter()
		:	_options(opAll) {}

	~default_formatter() {}

	void format(int level, quark::pstring& buffer, const char* format, va_list argl)
	{
		try
		{
			options_t options = _options;

			// add timestamp
			if (options & opTime)
				buffer += quark::strings::format("%s ", _TimeStamp::now().c_str());
			// add thread id, 
			if (options & opThread)
				buffer += quark::strings::format("%5u ", quark::thread::getCurrentThreadId());
			// add level
			if (options & opLevel)
				buffer += quark::strings::format("[%c] ", getLevelChar(level));
			// add body
			buffer += quark::strings::vformat(format, argl);
			// add terminator
			buffer += '\n';
		}
		catch (std::exception& ex)
		{
			buffer += "tracer format error: ";
			buffer += ex.what();
		}
	}

	char getLevelChar(int level) const
	{
		static const char chars[] = {'D', 'I', 'N', 'W', 'E', 'Z'};
		assert(level < (int)(sizeof(chars)/sizeof(char)));
		return chars[level];
	}

private:
	volatile options_t _options;
};

///////////////////////////////////////////////////////////
// class thread_formatter
//
template<class _Formatter = default_formatter<> >
class thread_formatter : public _Formatter
{
private:
	typedef quark::pvector<quark::pstring> indentstack_t;
	typedef quark::pmap<int, indentstack_t> threadmap_t;

public:
	void indent(const char* indent)
	{
		getStack().push_back(indent);
	}

	void unindent()
	{
		getStack().pop_back();
	}

protected:
	thread_formatter() {}
	~thread_formatter() {}

	void format(int level, quark::pstring& buffer, const char* format, va_list argl)
	{
		const indentstack_t& stack = getStack();
		quark::pstring str = std::accumulate(stack.begin(), stack.end(), _empty);
		str += format;
		_Formatter::format(level, buffer, str.c_str(), argl);
	}

	indentstack_t& getStack()
	{
		return _threadMap[quark::thread::getCurrentThreadId()];
	}

private:
	threadmap_t _threadMap;
	quark::pstring _empty;
};

///////////////////////////////////////////////////////////
// class console_writer
//
class console_writer
{
protected:
	~console_writer() {}

	void write(int level, const quark::pstring& buffer)
	{
		::printf("%s", buffer.c_str());
	}
};

///////////////////////////////////////////////////////////
// class file_writer
//
class file_writer
{
public:
	enum options_t { opRewrite, opAppend };

public:
	bool setFile(const char* file, options_t openMode = opRewrite)
	{
		assert(file);

		quark::pstring path = path::getDirectory(file);
		if (!path.empty() && !path::makeDir(path.c_str()))
			return false;

		if (!openFile(file, openMode))
			return false;

		_file = file;
		return true;
	}

	const quark::pstring getFile() const
	{
		return _file;
	}
	
protected:
	file_writer()
		:	_stream(NULL) {}

	~file_writer()
		{ closeFile(); }

	void write(int level, const quark::pstring& buffer)
	{
		if (_stream != NULL)
		{
			::fwrite(buffer.c_str(), sizeof(char), buffer.length(), _stream);
			::fflush(_stream);
		}
	}

	bool openFile(const char* file, options_t openMode)
	{
		closeFile();
		_stream = ::fopen(file, openMode == opRewrite ? "wt" : "at");
		return _stream != NULL;
	}
	
	void closeFile()
	{
		if (_stream != NULL)
		{
			::fclose(_stream);
			_stream = NULL;
		}
	}

private:
	quark::pstring _file;
	FILE* _stream;
};

///////////////////////////////////////////////////////////
// class combined_writer
//
template<class _Writer1, class _Writer2>
class combined_writer :	public _Writer1,
						public _Writer2
{
protected:
	~combined_writer() {}

	void write(int level, const quark::pstring& buffer)
	{
		_Writer1::write(level, buffer);
		_Writer2::write(level, buffer);
	}
};

///////////////////////////////////////////////////////////
// class tracer
//
template
<
	class _Formatter = thread_formatter<>,
	class _Writer = console_writer,
	class _Mutex = quark::critical_section
>
class tracer :	public _Writer,
				public _Formatter,
				public quark::noncopyable
{
public:
	tracer()
		:	_Formatter(),
			_mask(tlDebug) {}

	void setMask(int level) 
		{ _mask = level; }

	int getMask() const 
		{ return _mask; }

	void print(int level, const char* format, va_list argl)
	{
		assert(format);

		if (level >= _mask)
		{
			quark::critical_scope<_Mutex> scope(_mtx);

			quark::pstring buffer;
			_Formatter::format(level, buffer, format, argl);
			_Writer::write(level, buffer);
		}
	}

private:
	volatile int _mask;
	_Mutex _mtx;
};

///////////////////////////////////////////////////////////
// class template auto_indent
//
template<class _Formatter>
struct auto_indent
{
	auto_indent(_Formatter& formatter, const char* indent = "  ")
		:	_formatter(formatter)
	{
		_formatter.indent(indent);
	}

	auto_indent(_Formatter& formatter, const quark::pstring& indent = "  ")
		:	_formatter(formatter)
	{
		_formatter.indent(indent.c_str());
	}

	~auto_indent()
	{
		_formatter.unindent();
	}

private:
	_Formatter& _formatter;
};

} // namespace meson

#endif // _MESON_TRACER_H_
