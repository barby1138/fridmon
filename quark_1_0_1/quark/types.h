// types.h
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

//TODO: move to the platform directory

#ifndef _QUARK_TYPES_H_
#define _QUARK_TYPES_H_

namespace quark
{

typedef unsigned char u8;	// unsigned 8 bits
typedef unsigned short u16;	// unsigned 16 bits
typedef unsigned long u32;	// unsigned 32 bits

typedef /*signed*/ char s8;		// signed 8 bits
typedef /*signed*/ short s16;	// signed 16 bits
typedef /*signed*/ long s32;	// signed 32 bits

#define U8_MAX 	UCHAR_MAX
#define U16_MAX USHRT_MAX
#define U32_MAX ULONG_MAX

#define S8_MAX 	CHAR_MAX
#define S16_MAX SHRT_MAX
#define S32_MAX LONG_MAX

} // namespace quark

#endif // _QUARK_TYPES_H_
