// index.cpp
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The Hyperon Library
// Copyright (c) 2004-2005 by Library Dream Team (Oli Tsisaruk, Serge Gaziev)
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// Authors make no representations about the suitability of this software for 
//     any purpose. It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#include "index.h"

using namespace hyperon;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// index_item
//////////////////////////////////////////////////////////////////////

const pstring index_item::toString() const
{ 
	return getName(); 
}

//////////////////////////////////////////////////////////////////////
// index
//////////////////////////////////////////////////////////////////////

const pstring hyperon::index::toString() const
{
	pstring res;
	for (items_t::const_iterator itor = _items.begin(); itor != _items.end(); ++itor)
	{
		res += itor->toString();
		if (itor != _items.end() - 1)
			res += '.';
	}
	return res;
}
