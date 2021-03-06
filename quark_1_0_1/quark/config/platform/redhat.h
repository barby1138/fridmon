// redhat.h
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

enum { default_thread_priority = 0 };
enum { driver_thread_priority = 20 };
enum { highest_thread_priority = 99 };

// nonreducable clock used to get time from machime start
// must be CLOCK_MONOTONIC, but it's not supported on redHat
#define _METER_CLOCK	CLOCK_REALTIME
