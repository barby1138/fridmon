// properties.h
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

#ifndef _HYPERON_PROPERTIES_H_
#define _HYPERON_PROPERTIES_H_

#include "index.h"
#include <quark/singleton.h>

namespace hyperon
{
class dom_element;

//////////////////////////////////////////////////
// class template properties
//
template<class _RootNode, class _Storage>
class properties : public _RootNode,
				   public _Storage
{
protected:
	virtual dom_element* onSave()
	{
		return toStorage(getDocument());
	}

	virtual void onLoad()
	{
		//printf("%s\n", getRootNode()->toString().c_str());
		fromStorage(getRootNode());
	}
};

template<class _T, class _Node>
const _T getProperty(_Node& node,
					 const index& idx, 
					 const quark::pstring& prop)
{
	return any_cast<_T>(node.getElement(idx).getAttribute(prop).getValue());
}

template<class _T, class _Node>
void setProperty(_Node& node,
				 const index& idx,
				 const quark::pstring& prop,
				 const _T& value)
{
	node.getElement(idx).getAttribute(prop).setValue(value);
}

template<class _T, class _Node>
const _T getProperty(_Node& node,
					 const quark::pstring& prop)
{
	return any_cast<_T>(node.getAttribute(prop).getValue());
}

template<class _T, class _Node>
void setProperty(_Node& node,
				 const quark::pstring& prop,
				 const _T& value)
{
	node.getAttribute(prop).setValue(value);
}

template<class _Node>
const quark::pstring getProperty(_Node& node,
					 const index& idx)
{
	return node.getElement(idx).getValue();
}

template<class _Node>
void setProperty(_Node& node,
				 const index& idx,
				 const quark::pstring& value)
{
	node.getElement(idx).setValue(value);
}
//////////////////////////////////////////////////
// class template properties_sngl
//
template <class _RootNode, class _Storage>
struct properties_sngl : public quark::singleton_holder<properties<_RootNode, _Storage> >
{
};

} // namespace hyperon

#endif // _HYPERON_PROPERTIES_H_
