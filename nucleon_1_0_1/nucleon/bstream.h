// bstream.h
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

#ifndef _NUCLEON_BSTREAM_H_
#define _NUCLEON_BSTREAM_H_

#include <quark/config.h>

namespace nucleon
{

//////////////////////////////////////////////////
// class template fixed_streambuf
//
template<typename _Elem>
class fixed_streambuf
{
public:
	typedef _Elem elem_type;

public:
	fixed_streambuf(_Elem* buffer, size_t size)
		:	_buffer(buffer), _size(size), _pos(0)
	{
	}

	void sputc(_Elem ch)
	{
		if (above(1)) overflow();
		*data() = ch;
		_pos += 1;
	}

	void sputn(const _Elem* ptr, size_t size)
	{
		if (above(size)) overflow();
		std::copy(ptr, ptr + size, data());
		_pos += size;
	}

	_Elem sgetc()
	{
		if (above(1)) underflow();
		_Elem res = *data();
		_pos += 1;
		return res;
	}

	void sgetn(_Elem* ptr, size_t size)
	{
		if (above(size)) underflow();
		std::copy(data(), data() + size, ptr);
		_pos += size;
	}

	_Elem peek() { return *data(); }
	_Elem nextc() { sgetc(); return sgetc(); }
	bool eof() const { return _pos == _size; }
	bool bof() const { return _pos == 0; }
	size_t getpos() { return _pos; }

	size_t seekoff(int off)
	{
		int pos = (int)_pos + off;
		_pos = std::max(pos, 0);
		_pos = std::min(_pos, _size - 1);
		return _pos;
	}

	size_t seekpos(size_t pos)
	{
		_pos = std::min(pos, _size - 1);
		return _pos;
	}

private:
	bool above(size_t size) { return size > _size - _pos; }
	bool below(size_t size) { return size > _pos; }
	_Elem* data() { return _buffer + _pos; }
	void overflow() { throw quark::pruntime_error("buffer overflow"); }
	void underflow() { throw quark::pruntime_error("buffer underflow"); }

private:
	_Elem* _buffer;
	size_t _size;
	size_t _pos;
};

//////////////////////////////////////////////////
// class template to_big_endian
//
template<class _Streambuf>
class to_big_endian
{
private:
	typedef typename _Streambuf::elem_type elem_type;

public:
	to_big_endian(_Streambuf& streambuf)
		:	_streambuf(streambuf)
	{
	}

	void putch(elem_type val)
	{
		_streambuf.sputc(val);
	}

	elem_type getch()
	{
		return _streambuf.sgetc();
	}

	template<typename _Val>
	void putn(_Val val)
	{
		// reverse
		reverse(val);
		// put into the stream buffer
		_streambuf.sputn((const elem_type*)&val, sizeof(_Val));
	}

	template<typename _Val>
	void getn(_Val& val)
	{
		// get from the stream buffer
		_streambuf.sgetn((elem_type*)&val, sizeof(_Val));
		// reverse
		reverse(val);
	}

#ifdef __LITTLE_ENDIAN__

	template<typename _Val>
	void reverse(_Val& val)
	{
		std::reverse((elem_type*)&val, (elem_type*)&val + sizeof(_Val));
	}

#endif // _LITTLE_ENDIAN

#ifdef __BIG_ENDIAN__

	template<typename _Val>
	void reverse(_Val& val)
	{
	}

#endif // _BIG_ENDIAN

private:
	_Streambuf& _streambuf;
};

//////////////////////////////////////////////////
// class template bstream
//
template<class _Streambuf, class _Convertor = to_big_endian<_Streambuf> >
class bstream : private _Convertor
{
public:
	typedef _Streambuf streambuf;
	typedef typename _Streambuf::elem_type elem_type;

public:
	bstream(_Streambuf& streambuf)
		:	_Convertor(streambuf),
			_streambuf(streambuf)
	{
	}

	bstream& operator<<(char val) { return sputc(val); }
	bstream& operator<<(unsigned char val) { return sputc(val); }
	bstream& operator<<(short val) { return sputn(val); }
	bstream& operator<<(unsigned short val) { return sputn(val); }
	bstream& operator<<(int val) { return sputn(val); }
	bstream& operator<<(unsigned int val) { return sputn(val); }
	bstream& operator<<(long val) { return sputn(val); }
	bstream& operator<<(unsigned long val) { return sputn(val); }

	bstream& operator>>(char& val) { return sgetc(val); }
	bstream& operator>>(unsigned char& val) { return sgetc(val); }
	bstream& operator>>(short& val) { return sgetn(val); }
	bstream& operator>>(unsigned short& val) { return sgetn(val); }
	bstream& operator>>(int& val) { return sgetn(val); }
	bstream& operator>>(unsigned int& val) { return sgetn(val); }
	bstream& operator>>(long& val) { return sgetn(val); }
	bstream& operator>>(unsigned long& val) { return sgetn(val); }

	elem_type get() { return _streambuf.sgetc(); }
	bstream& put(elem_type val) { return sputn(val); }
	size_t tellp() { return _streambuf.getpos(); }
	_Streambuf* rdbuf() const { return &_streambuf; }
	bool eof() const { return _streambuf.eof(); }

private:
	template<typename Val>
	bstream& sputn(const Val& val)
	{
		putn(val); return *this;
	}
	template<typename Val>
	bstream& sputc(Val val)
	{
		putch(val); return *this;
	}

	template<typename Val>
	bstream& sgetn(Val& val)
	{
		_Convertor::getn(val); return *this;
	}
	template<typename Val>
	bstream& sgetc(Val& val)
	{
		val = _Convertor::getch(); return *this;
	}

private:
	_Streambuf& _streambuf;
};

} // namespace nucleon

#endif // _NUCLEON_BSTREAM_H_
