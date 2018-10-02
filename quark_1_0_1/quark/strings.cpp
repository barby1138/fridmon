// strings.cpp
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

#include "strings.h"
#include <stdarg.h>
#include <errno.h>

//TODO: separate platform dependent code if necessary
//#include IMPL_FILE(strings_impl.h)

using namespace quark;

//////////////////////////////////////////////////////////////////////
// strings
//////////////////////////////////////////////////////////////////////

const pstring strings::toString(const u32& val)
{
	char buf[12];
	sprintf(buf, "%u", val);
	return buf;	
}

const pstring strings::toString(const s32& val)
{
	char buf[12];
	sprintf(buf, "%d", val);
	return buf;
}

void strings::fromString(const pstring& str, u32& val)
{
	char* end = 0;
	errno = 0;
	val = strtoul(str.c_str(), &end, 0);
	if ((errno == ERANGE) || (errno == EINVAL) || end == str.c_str())
		throw pruntime_error("conversion to u32 failed");
}

void strings::fromString(const pstring& str, s32& val)
{
	char* end = 0;
	errno = 0;
	val = strtol(str.c_str(), &end, 0);
	if ((errno == ERANGE) || (errno == EINVAL) || end == str.c_str())
		throw pruntime_error("conversion to s32 failed");
}

const pstring strings::format(const char* format, ...)
{
	va_list	argl;
	va_start(argl, format);
	pstring res = strings::vformat(format, argl);
	va_end(argl);

	return res;
}

const pstring strings::vformat(const char* format, va_list argl)
{
	enum { bufSize = 2048 };
	char buffer[bufSize];

	try
	{
		::vsprintf(buffer, format, argl);
	}
	catch (...)
	{
		throw pruntime_error("format procedure buffer overflow or bad format parameters");
	}

	return buffer;
}

// TODO review

static const u32 IP_OCTETS_COUNT = 4;
static const u32 BITS_PER_BYTE = 8;
const u32 MAX_IP_OCTET_VALUE = 255;
const char IP_DOT_SEPARATOR = '.';
static const u32 DECIMAL_RADIX = 10;

pstring convertNumber(u32 iNum)
{
	char x[20];
	sprintf(x, "%d", iNum);
	return x;
}

bool getU32Number(const pstring& sNum, u32* pValue)
{		
	char * pEndPtr = NULL;

	const char * pStringBase = sNum.c_str();

	*pValue = strtoul(pStringBase,&pEndPtr,DECIMAL_RADIX);

	return ((pEndPtr - pStringBase) == (sNum.size()));
}

void strings::packIPAddress(const pstring& pIn, u32* pValue)
{
	pstring::size_type iStartPos = 0;
	pstring::size_type iPrevPos = 0;

	*pValue = 0;

	u32 iValue;

	for (u32 i = 0; i < IP_OCTETS_COUNT; ++i)
	{
		iStartPos = pIn.find(IP_DOT_SEPARATOR,iPrevPos);

		if ((iStartPos != pstring::npos) || (i == (IP_OCTETS_COUNT - 1)))
		{
			pstring sTempSubstr;
			
			if (iStartPos != pstring::npos)
				sTempSubstr = pIn.substr(iPrevPos,iStartPos - iPrevPos);
			else
				sTempSubstr = pIn.substr(iPrevPos);

			if (getU32Number(sTempSubstr, &iValue))
			{
				if (iValue <= MAX_IP_OCTET_VALUE)
				{
					*pValue |= (iValue << (BITS_PER_BYTE * i));

					iPrevPos = iStartPos + 1;

					continue;
				}
			}
		}

		throw pruntime_error("invalid IP format");
	}
}

void strings::unPackIPAddress(pstring& pOut, const u32& pInValue)
{
	pstring sTemp = "";
	
	for (int i = 0; i < IP_OCTETS_COUNT; ++i)
	{
		u8 part = static_cast<u8>(pInValue >> i*BITS_PER_BYTE);

		sTemp += convertNumber(part);

		if (i != IP_OCTETS_COUNT - 1) sTemp += IP_DOT_SEPARATOR;
	}

	pOut = sTemp;
}
