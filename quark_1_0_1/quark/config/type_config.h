// types_config.h
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

#include <utility>

#if defined(__GLIBCPP__) || defined(__GLIBCXX__)
// GNU libstdc++ 3
#  define TARGET_TYPE glib_types
#else
#  define TARGET_TYPE default_types
#endif

#define QUARK_TYPE_CONFIG <quark/config/type/TARGET_TYPE.h>
