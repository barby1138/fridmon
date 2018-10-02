// platform_config.h
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

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// Win32:
#  define TARGET_PLATFORM win32

#elif defined(__VX_WORKS__)
// VxWorks
#  define TARGET_PLATFORM vxworks

#elif defined(_REDHAT) || defined(__REDHAT__)
// RedHat Linux
#  define TARGET_PLATFORM redhat

#elif defined (QUARK_ASSERT_CONFIG)
// this must come last - generate an error if we don't
// recognise the platform:
#  error "Unknown platform - please configure"

#endif
