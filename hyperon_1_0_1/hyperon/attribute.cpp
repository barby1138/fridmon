// attribute.cpp
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

#include "attribute.h"
#include <quark/strings.h>

using namespace hyperon;
using namespace quark;

//////////////////////////////////////////////////////////////////////
// attr_exception
//////////////////////////////////////////////////////////////////////

attr_exception::attr_exception(const char* name, const char* descr)
	:	pruntime_error(strings::format("attribute: %s; %s", name, descr))
{
}
