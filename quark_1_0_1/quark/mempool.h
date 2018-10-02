// mempool.h
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

#ifndef _QARK_MEMPOOL_PUBLIC_H_
#define _QARK_MEMPOOL_PUBLIC_H_

#include "config.h"
#include <quark/mempool/mempool.h>
#include <quark/mempool/user_mempool.h>
#include <quark/singleton.h>

namespace quark
{
	
typedef singleton_holder
<	
	memory_pool<>, 
	create_static, 
	default_lifetime, 
	class_level_lockable<memory_pool<> >
> default_memory_pool;

} // namespace quark

#endif // _QARK_MEMPOOL_PUBLIC_H_
