// user_mempool.cpp
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

#include <quark/mempool.h>

using namespace quark;

//////////////////////////////////////////////////////////////////////
// user_mempool
//////////////////////////////////////////////////////////////////////

user_mempool::user_mempool()
	:	_fnAlloc(0),
		_fnFree(0)
{
}

user_mempool& user_mempool::instance()
{
	static user_mempool inst;
	return inst;		
}

void user_mempool::defaultBind()
{
	binder<default_memory_pool>::bind();
}

bool user_mempool::bind(alloc_fn fnAlloc, free_fn fnFree)
{
	assert(_fnAlloc == 0 && _fnFree == 0);
	
	if (_fnAlloc != 0 || _fnFree != 0)
		return false;
	
	_fnAlloc = fnAlloc;
	_fnFree = fnFree;
	return true;
}
