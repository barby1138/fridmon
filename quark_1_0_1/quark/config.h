// config.h
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

#ifndef _QUARK_CONFIG_H_
#define _QUARK_CONFIG_H_

// if we don't have a user config, then use the default location:
#if !defined(QUARK_USER_CONFIG) && !defined(QUARK_NO_USER_CONFIG)
#  define QUARK_USER_CONFIG <quark/config/user.h>
#endif
// include it first:
#ifdef QUARK_USER_CONFIG
#  include QUARK_USER_CONFIG
#endif

// if we don't have a compiler config set, try and find one:
#if !defined(QUARK_COMPILER_CONFIG) && !defined(QUARK_NO_COMPILER_CONFIG) && !defined(QUARK_NO_CONFIG)
#  include <quark/config/compiler_config.h>

#  define QUARK_COMPILER_CONFIG <quark/config/compiler/TARGET_COMPILER.h>
#endif
// if we have a compiler config, include it now:
#ifdef QUARK_COMPILER_CONFIG
#  include QUARK_COMPILER_CONFIG
#endif

// if we don't have types config set, try and find one:
#if !defined(QUARK_TYPE_CONFIG) && !defined(QUARK_NO_TYPE_CONFIG) && !defined(QUARK_NO_CONFIG)
#  include <quark/config/type_config.h>
#endif
// if we have a std library config, include it now:
#ifdef QUARK_TYPE_CONFIG
#  include QUARK_TYPE_CONFIG
#endif


// if we don't have a std library config set, try and find one:
#if !defined(QUARK_STL_CONFIG) && !defined(QUARK_NO_STL_CONFIG) && !defined(QUARK_NO_CONFIG)
#  include <quark/config/stl_config.h>

#  define QUARK_STL_CONFIG <quark/config/stl/TARGET_STL.h>
#endif
// if we have a std library config, include it now:
#ifdef QUARK_STL_CONFIG
#  include QUARK_STL_CONFIG
#endif

// if we don't have a platform config set, try and find one:
#if !defined(QUARK_PLATFORM_CONFIG) && !defined(QUARK_NO_PLATFORM_CONFIG) && !defined(QUARK_NO_CONFIG)
#  include <quark/config/platform_config.h>

#  define QUARK_PLATFORM_CONFIG <quark/config/platform/TARGET_PLATFORM.h>
#  define IMPL_FILE(file) <quark/TARGET_PLATFORM/file>

#endif
// if we have a platform config, include it now:
#ifdef QUARK_PLATFORM_CONFIG
#  include QUARK_PLATFORM_CONFIG
#endif

#include <quark/common_templates.h>

// get config suffix code:

#endif  // _QUARK_CONFIG_H_
