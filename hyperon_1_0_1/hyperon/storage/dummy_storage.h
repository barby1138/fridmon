// dummy_storage.h
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

#ifndef _HYPERON_EXPAT_H_
#define _HYPERON_EXPAT_H_

namespace hyperon
{

//////////////////////////////////////////////////
// class dummy_storage
//
class dummy_storage
{
public:
	dom_document* getDocument() { return NULL; }
	dom_element* getRootNode() { return NULL; }
};

} // namespace hyperon

#endif // _HYPERON_EXPAT_H_
