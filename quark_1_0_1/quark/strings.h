// strings.h
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

#ifndef _QUARK_STRINGS_H_
#define _QUARK_STRINGS_H_

#include "config.h"

namespace quark
{

namespace strings
{

void packIPAddress(const pstring& pIn, u32* pValue);
void unPackIPAddress(pstring& pOut, const u32& pInValue);

const pstring toString(const u32& val);
const pstring toString(const s32& val);

void fromString(const pstring& str, u32& val);
void fromString(const pstring& str, s32& val);

const pstring format(const char* format, ...);
const pstring vformat(const char* format, va_list argl);

template<typename _Str, typename _Out>
void split(const _Str& str, const char* sep, _Out dest)
{
	typename _Str::size_type left = str.find_first_not_of(sep);
	typename _Str::size_type right = str.find_first_of(sep, left);
	
	while (left < right) 
	{
		*dest = str.substr(left, right - left);
		++dest;
		left = str.find_first_not_of(sep, right);
		right = str.find_first_of(sep, left);
	}
}

// TODO: check performance of both ways and choose the best one
template<typename _Str, class _Itor>
const _Str join(_Itor first, _Itor last, const char* sep)
{
	_Str res;
	for (_Itor itor = first; itor != last; ++itor)
	{
		res += *itor;
		if (itor != last - 1)
			res	+= sep;
	}
	return res;

/*
	struct local {
		static pstring pred(const pstring& str1, const pstring& str2)
		{ return str1 + ", " + str2; }
	};
	return std::accumulate(states.begin(), states.end(), pstring(""), &local::pred);
 */
}

template<typename _Str>
const _Str trim(const _Str& str, const char* toTrim = " \t\r\n")
{
	typename _Str::size_type left = str.find_first_not_of(toTrim);
	if (left == _Str::npos)
		return "";
	
	typename _Str::size_type right = str.find_last_not_of(toTrim);
	return str.substr(left, right - left + 1);
}

} // namespace strings

} // namespace quark

#endif // _QUARK_STRINGS_H_
