// vc.h
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Quark Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Copyright (c) 2006, InterObject Ltd
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4291)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
// disable VC2005 secure deprecation
#pragma warning(disable:4996)

// for std::realock, std::atexit
#if _MSC_VER <= 1200
#define VC_BROKEN_STD
#else
#define VC_BROKEN_STD std
#endif

#define TEMPLATEPARAM_TYPE	
#define FRIEND_CLASS friend
